#include "GameServerConnection.h"
#include "NetworkCommunicationSignals.h"

#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

GameServerConnection::GameServerConnection() : failedLobbyConnectAttempts(0),
                                               networkOperationAttempts(0),
                                               serverTcpSocket(new sf::TcpSocket()),
                                               gameRunningUdpSocket(new sf::UdpSocket()) {
    serverTcpSocket->setBlocking(false);
    gameRunningUdpSocket->setBlocking(false);
}

GameServerConnection::~GameServerConnection() {
    serverTcpSocket->disconnect();
};

NonBlockingNetOpStatus GameServerConnection::connectToGameLobby() {
    if (!connectingToLobby) {
        const sf::Socket::Status status = serverTcpSocket->connect(serverIp, serverLobbyPort);
        std::cout << "Failed to connect to game lobby." << std::endl;
        if (status == sf::Socket::Error) {
            return NonBlockingNetOpStatus::ERROR;
        } else {
            connectingToLobby = true;
        }
    } else {
        sf::Packet pingPacket;
        auto status = serverTcpSocket->receive(pingPacket);
        if (status == sf::Socket::Done) {
            sf::Packet pongPacket;
            pongPacket << "pong";
            serverTcpSocket->send(pongPacket);
            return NonBlockingNetOpStatus::COMPLETE;
        }
    }

    if (failedLobbyConnectAttempts % 120 == 0) {
        std::cout << "Attempting to connect to game lobby..." << std::endl;
    }
    failedLobbyConnectAttempts++;
    return NonBlockingNetOpStatus::NOT_READY;
}

NonBlockingNetOpStatus GameServerConnection::pingCheck() {
    sf::Packet pingCheckPacket;
    auto status = serverTcpSocket->receive(pingCheckPacket);

    if (status == sf::Socket::Error) {
        std::cout << "Failed to received a ping check result packet. Assuming connection is poor." << std::endl;
        return NonBlockingNetOpStatus::ERROR;
    } else if (status == sf::Socket::NotReady) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signal;
    pingCheckPacket >> signal;

    if (signal != ServerSignal::PING_OK) {
        std::cout << "Failed to received a ping OK message from the server. Assuming connection is poor." << std::endl;
        return NonBlockingNetOpStatus::ERROR;
    }

    return NonBlockingNetOpStatus::COMPLETE;
}

void GameServerConnection::disconnectFromGameLobby() {
    serverTcpSocket->disconnect();
}

NonBlockingNetOpStatus GameServerConnection::findGame() {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Waiting for a game match..." << std::endl;
    }

    sf::Packet matchFoundPacket;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(matchFoundPacket);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        networkOperationAttempts++;
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signal;
    matchFoundPacket >> signal;

    if (signal != ServerSignal::FOUND_GAME_MATCH) {
        networkOperationAttempts++;
        std::cout << "Received an unexpected signal from the server. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    }

    std::cout << "Game match found." << std::endl;
    networkOperationAttempts = 0;

    return NonBlockingNetOpStatus::COMPLETE;
}

NonBlockingNetOpStatus GameServerConnection::acceptGame() {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Signaling acceptance of game match..." << std::endl;
    }

    unsigned short udpPort;
    if (!bindGameRunningConnection(udpPort)) {
        std::cout << "Waiting to bind a local UDP socket." << std::endl;
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Packet acceptMatchPacket;
    acceptMatchPacket << static_cast<sf::Int8>(ClientSignal::READY_FOR_MATCH) << udpPort;
    const auto clientDataSentStatus = serverTcpSocket->send(acceptMatchPacket);

    if (clientDataSentStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (clientDataSentStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (clientDataSentStatus != sf::Socket::Done) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    std::cout << "Acceptance signaled." << std::endl;
    networkOperationAttempts = 0;

    return NonBlockingNetOpStatus::COMPLETE;
}

NonBlockingNetOpStatus GameServerConnection::verifyGameLaunch(bool* gameOn) {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Waiting for a game match to be verified..." << std::endl;
    }

    *gameOn = false;

    sf::Packet gameOnOrOffPacket;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(gameOnOrOffPacket);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        networkOperationAttempts++;
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signal;
    gameOnOrOffPacket >> signal;

    if (signal == ServerSignal::GAME_OFF) {
        std::cout << "Game has been called off. Opponent may have disconnected." << std::endl;
        gameRunningUdpSocket->unbind();
        return NonBlockingNetOpStatus::COMPLETE;
    } else if (signal != ServerSignal::GAME_ON) {
        std::cout << "Received an unexpected signal from the server." << std::endl;
        return NonBlockingNetOpStatus::ERROR;
    }

    std::cout << "Game match verified. Launching game." << std::endl;
    *gameOn = true;
    networkOperationAttempts = 0;

    return NonBlockingNetOpStatus::COMPLETE;
}

bool GameServerConnection::getGameInitParameters(bool& isPlayer1Local, unsigned short& serverUdpPort) {
    serverTcpSocket->setBlocking(true);
    sf::Packet signalPacket;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(signalPacket);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return false;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return false;
    }

    sf::Int8 signalType;
    signalPacket >> signalType;
    if (signalType != ServerSignal::GAME_INIT) {
        std::cout << "Received an unexpected signal from the server." << std::endl;
        return false;
    }

    signalPacket >> isPlayer1Local;
    signalPacket >> serverUdpPort;

    return true;
}

NonBlockingNetOpStatus GameServerConnection::getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate) {
    sf::Packet signalPacket;
    sf::IpAddress senderIp;
    unsigned short senderPort;
    const auto serverDataReceiveStatus = gameRunningUdpSocket->receive(signalPacket, senderIp, senderPort);

    //todo error handling?
    if (serverDataReceiveStatus != sf::Socket::Done) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signalType;
    signalPacket >> signalType;
    gameUpdate.type = AuthoritativeGameUpdate::determineUpdateType(signalType);
    switch (gameUpdate.type) {
    case AuthoritativeGameUpdate::Type::PLAYER_POSITION_UPDATE:
        gameUpdate.playerPosition = readPlayerPositionUpdate(signalPacket);
        break;
    case AuthoritativeGameUpdate::Type::MOVE_COMMAND_ACK:
        gameUpdate.moveCommandAck = readMoveCommandAckUpdate(signalPacket);
        break;
    case AuthoritativeGameUpdate::Type::PROJECTILE_UPDATE:
        gameUpdate.projectile = readProjectileUpdate(signalPacket);
        ackProjectileUpdate(gameUpdate.projectile);
        break;
    case AuthoritativeGameUpdate::Type::UNKNOWN:
    default:
        std::cout << "Received an unexpected signal from the server." << std::endl;
        return NonBlockingNetOpStatus::ERROR;
    }

    return NonBlockingNetOpStatus::COMPLETE;
}

bool GameServerConnection::bindGameRunningConnection(unsigned short& udpSocketPort) {
    if (gameRunningUdpSocket->bind(sf::Socket::AnyPort) == sf::Socket::Done) {
        udpSocketPort = gameRunningUdpSocket->getLocalPort();
        std::cout << "Bound local UDP socket to port " + std::to_string(udpSocketPort) << std::endl;
        return true;
    }

    return false;
}

AuthoritativeGameUpdate::PlayerPositionUpdate GameServerConnection::readPlayerPositionUpdate(sf::Packet& signalPacket) {
    bool player1PositionChanged, player2PositionChanged;
    sf::Vector2f newPlayer1Position, newPlayer2Position;
    sf::Uint16 time;
    signalPacket >> time >> player1PositionChanged >> player2PositionChanged;

    signalPacket >> newPlayer1Position.x >> newPlayer1Position.y;

    signalPacket >> newPlayer2Position.x >> newPlayer2Position.y;

    return  AuthoritativeGameUpdate::PlayerPositionUpdate(time, player1PositionChanged, newPlayer1Position, player2PositionChanged, newPlayer2Position);
}

AuthoritativeGameUpdate::MoveCommandAckUpdate GameServerConnection::readMoveCommandAckUpdate(sf::Packet& signalPacket) {
    sf::Uint16 sequenceNumber;
    signalPacket >> sequenceNumber;
    return AuthoritativeGameUpdate::MoveCommandAckUpdate(sequenceNumber);
}

AuthoritativeGameUpdate::ProjectileUpdate GameServerConnection::readProjectileUpdate(sf::Packet& signalPacket) {
    sf::Uint16 sequenceNumber;
    sf::Uint16 unackedUpdatesInPacket;
    signalPacket >> sequenceNumber >> unackedUpdatesInPacket;

    // std::cout << "RECEIVED PROJECTILE UPDATE SEQ " << sequenceNumber << std::endl;

    int actualUnackedUpdates = sequenceNumber - lastAckedProjectileUpdateSeqNumber;
    std::vector<AuthoritativeGameUpdate::ProjectileCreatedUpdate> unackedProjectileCreatedCommands;
    std::vector<AuthoritativeGameUpdate::ProjectileHitUpdate> unackedProjectileHitCommands;
    for (int i = 0; i < actualUnackedUpdates; i++) {
        sf::Int8 updateType;
        signalPacket >> updateType;

        if (updateType == ServerSignal::PROJECTILE_CREATED) {
            sf::Vector2f position;
            bool isDirectionRight;
            bool isFiredByPlayer1;
            signalPacket >> position.x >> position.y >> isDirectionRight >> isFiredByPlayer1;
            unackedProjectileCreatedCommands.emplace_back(AuthoritativeGameUpdate::ProjectileCreatedUpdate(position,
                                                                                                           isDirectionRight ? SimulationProperties::Direction::RIGHT : SimulationProperties::Direction::LEFT,
                                                                                                           isFiredByPlayer1));
        } else if (updateType == ServerSignal::PROJECTILE_HIT) {
            bool hitPlayer1;
            signalPacket >> hitPlayer1;
            unackedProjectileHitCommands.emplace_back(AuthoritativeGameUpdate::ProjectileHitUpdate(hitPlayer1));
        } else {
            std::cout << "Received unrecognized projectile update type";
            continue;
        }
    }

    return AuthoritativeGameUpdate::ProjectileUpdate(sequenceNumber, unackedProjectileCreatedCommands, unackedProjectileHitCommands);
}

void GameServerConnection::ackProjectileUpdate(AuthoritativeGameUpdate::ProjectileUpdate& projectileUpdate) {
    // std::cout << "SENDING ACK FOR PROJECTILE UPDATE SEQ " << projectileUpdate.sequenceNumber << std::endl;

    sf::Packet ackPacket;
    ackPacket << static_cast<sf::Int8>(ClientSignal::PROJECTILE_UPDATE_ACK) << projectileUpdate.sequenceNumber;
    gameRunningUdpSocket->send(ackPacket, serverIp, gameRunningSocketPort);

    if (projectileUpdate.sequenceNumber > lastAckedProjectileUpdateSeqNumber) {
        lastAckedProjectileUpdateSeqNumber = projectileUpdate.sequenceNumber;
    }
}

void GameServerConnection::sendMoveCommand(bool left, bool right, bool jump, bool attack) {
    unackedCommands.emplace_back(MoveCommand(left, right, jump, attack));

    sf::Packet moveCommandPacket;
    moveCommandPacket << static_cast<sf::Int8>(ClientSignal::MOVE_COMMAND);

    moveCommandPacket << moveCommandSeqNumber;
    moveCommandSeqNumber++;

    moveCommandPacket << static_cast<sf::Uint16>(unackedCommands.size());

    for (int i = unackedCommands.size() - 1; i >= 0; i--) {
        moveCommandPacket << unackedCommands[i].left << unackedCommands[i].right << unackedCommands[i].jump << unackedCommands[i].attack;
    }

    gameRunningUdpSocket->send(moveCommandPacket, serverIp, gameRunningSocketPort);
}

void GameServerConnection::markMoveCommandAsAcked(int sequenceNumber) {
    if (sequenceNumber > latestAckedSeqNumber) {
        int commandsBeingAcked = sequenceNumber - latestAckedSeqNumber;
        for (int i = 0; i < commandsBeingAcked; i++) {
            unackedCommands.pop_front();
        }
        latestAckedSeqNumber = sequenceNumber;
    }
}

void GameServerConnection::setServerGameRunningSocketPort(unsigned short serverGameRunningSocketPort) {
    this->gameRunningSocketPort = serverGameRunningSocketPort;
}
