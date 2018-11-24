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
    gameRunningUdpSocket->setBlocking(false);
}

GameServerConnection::~GameServerConnection() {
    serverTcpSocket->disconnect();
};

bool GameServerConnection::connectToGameLobby() {
    serverTcpSocket->setBlocking(true);
    const sf::Socket::Status status = serverTcpSocket->connect(serverIp, serverLobbyPort, sf::microseconds(1));
    if (status == sf::Socket::Done) {
        std::cout << "Connected to game lobby." << std::endl;
        serverTcpSocket->setBlocking(false);
        return true;
    } else {
        if (failedLobbyConnectAttempts % 120 == 0) {
            std::cout << "Attempting to connect to game lobby..." << std::endl;
        }
        failedLobbyConnectAttempts++;
        return false;
    }
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

NonBlockingNetOpStatus GameServerConnection::getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate) {
    sf::Packet signalPacket;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(signalPacket);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signalType;
    signalPacket >> signalType;
    gameUpdate.type = AuthoritativeGameUpdate::determineUpdateType(signalType);
    switch (gameUpdate.type) {
    case AuthoritativeGameUpdate::Type::INIT:
        bool isPlayer1;
        signalPacket >> isPlayer1;
        unsigned short udpPort;
        signalPacket >> udpPort;
        gameUpdate.init = AuthoritativeGameUpdate::InitUpdate(isPlayer1, udpPort);
        break;
    case AuthoritativeGameUpdate::Type::UNKNOWN:
    default:
        std::cout << "Received an unexpected signal from the server. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
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

void GameServerConnection::sendMoveCommand(Command command) {
    sf::Packet moveCommandPacket;
    moveCommandPacket << static_cast<sf::Int8>(ClientSignal::MOVE_COMMAND);

    if (command == Command::MOVE_LEFT) {
        moveCommandPacket << true << false << false;
    } else if ((command == Command::MOVE_RIGHT)) {
        moveCommandPacket << false << true << false;
    } else if ((command == Command::JUMP)) {
        moveCommandPacket << false << false << true;
    }
    gameRunningUdpSocket->send(moveCommandPacket, serverIp, gameRunningSocketPort); //todo: add timestamp to command
}

void GameServerConnection::setServerGameRunningSocketPort(unsigned short serverGameRunningSocketPort) {
    this->gameRunningSocketPort = serverGameRunningSocketPort;
}

unsigned short GameServerConnection::getGameRunningSocketPort() {
    return gameRunningSocketPort;
}
