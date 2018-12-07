#include "GameClientConnection.h"
#include "ProjectileCreatedUpdate.h"
#include <iostream>
#include "ProjectileHitUpdate.h"

GameClientConnection::GameClientConnection(ThreadLogger logger,
                                           sf::IpAddress player1RemoteIp,
                                           unsigned short player1RemotePort,
                                           sf::IpAddress player2RemoteIp,
                                           unsigned short player2RemotePort) : logger(logger),
                                                                               player1Connection(player1RemoteIp, player1RemotePort),
                                                                               player2Connection(player2RemoteIp, player2RemotePort) {
}

GameClientConnection::~GameClientConnection() {
    player1Connection.udpSocket.unbind();
    player2Connection.udpSocket.unbind();
};

void GameClientConnection::initialize(unsigned short& player1UdpSocketPort, unsigned short& player2UdpSocketPort) {
    if (player1Connection.udpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        //todo: throw excpetion?
    }
    player1Connection.udpSocket.setBlocking(false);
    player1UdpSocketPort = player1Connection.udpSocket.getLocalPort();

    if (player2Connection.udpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        //todo: throw excpetion?
    }
    player2Connection.udpSocket.setBlocking(false);
    player2UdpSocketPort = player2Connection.udpSocket.getLocalPort();
}

NonBlockingNetOpStatus GameClientConnection::getPlayer1Update(ClientUpdate& clientUpdate) {
    return player1Connection.getNetworkUpdate(clientUpdate);
}

NonBlockingNetOpStatus GameClientConnection::getPlayer2Update(ClientUpdate& clientUpdate) {
    return player2Connection.getNetworkUpdate(clientUpdate);
}

void GameClientConnection::broadcastPlayerPositions(sf::Uint16 time, sf::Vector2<float> player1Position, sf::Vector2<float> player2Position) {
    sf::Packet movementUpdate;
    movementUpdate << static_cast<sf::Int8>(ServerSignal::PLAYER_POSITION_UPDATE) << time;

    movementUpdate << player1Position.x << player1Position.y;
    movementUpdate << player2Position.x << player2Position.y;

    player1Connection.udpSocket.send(movementUpdate, player1Connection.address.ip, player1Connection.address.port);
    player2Connection.udpSocket.send(movementUpdate, player2Connection.address.ip, player2Connection.address.port);
}

void GameClientConnection::queueProjectileCreationBroadcast(sf::Uint16 time, sf::Vector2f position, SimulationProperties::Direction direction, bool firedByPlayer1) {
    player1Connection.queueProjectileCreationBroadcast(time, position, direction, firedByPlayer1);
    player2Connection.queueProjectileCreationBroadcast(time, position, direction, firedByPlayer1);
}

void GameClientConnection::queueProjectileHitBroadcast(bool hitPlayer1) {
    player1Connection.queueProjectileHitBroadcast(hitPlayer1);
    player2Connection.queueProjectileHitBroadcast(hitPlayer1);
}

void GameClientConnection::sendUnackedProjectileUpdates() {
    player1Connection.sendUnackedProjectileUpdates();
    player2Connection.sendUnackedProjectileUpdates();
}

bool GameClientConnection::allProjectileUpdatesAcked() {
    return player1Connection.allProjectileUpdatesAcked() && player2Connection.allProjectileUpdatesAcked();
}

void GameClientConnection::PlayerConnection::sendUnackedProjectileUpdates() {
    if (unackedUpdates.empty()) {
        return;
    }
    
    sf::Packet unackedCommandsPacket;
    unackedCommandsPacket << static_cast<sf::Int8>(ServerSignal::PROJECTILE_UPDATE);
    
    unackedCommandsPacket << projectileUpdateSeqNumber;
    
    unackedCommandsPacket << static_cast<sf::Uint16>(unackedUpdates.size());
    
    for (int i = 0; i < unackedUpdates.size(); i++) {
        unackedUpdates[i]->appendToPacket(unackedCommandsPacket);
    }

    // std::cout << "SENDING UNACKED PROJECTILE UPDATE SEQ " << projectileUpdateSeqNumber << " to connection with port " << address.port << std::endl;

    udpSocket.send(unackedCommandsPacket, address.ip, address.port);
}

NonBlockingNetOpStatus GameClientConnection::PlayerConnection::getNetworkUpdate(ClientUpdate& clientUpdate) {
    sf::Packet signalPacket;
    sf::IpAddress senderIpAddress;
    unsigned short senderPort;
    const auto serverDataReceiveStatus = udpSocket.receive(signalPacket, senderIpAddress, senderPort);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signalType;
    signalPacket >> signalType;
    clientUpdate.type = ClientUpdate::determineUpdateType(signalType);
    switch (clientUpdate.type) {
    case ClientUpdate::Type::MOVE:
        clientUpdate.move = readMoveUpdate(signalPacket);
        ackMoves(clientUpdate.move);
        break;
    case ClientUpdate::Type::PROJECTILE_UPDATE_ACK:
        clientUpdate.projectileAck = readProjectileUpdateAck(signalPacket);
        markProjectileUpdateAsAcked(clientUpdate.projectileAck.sequenceNumber);
        break;
    case ClientUpdate::Type::UNKNOWN:
    default:
        return NonBlockingNetOpStatus::ERROR;
    }

    return NonBlockingNetOpStatus::COMPLETE;
}

ClientUpdate::MoveUpdate GameClientConnection::PlayerConnection::readMoveUpdate(sf::Packet& signalPacket) {
    sf::Uint16 sequenceNumber;
    sf::Uint16 unackedMovesInPacket;
    signalPacket >> sequenceNumber >> unackedMovesInPacket;

    int actualUnackedMoves = sequenceNumber - lastAckedMoveCmdSeqNumber;
    std::vector<ClientUpdate::MoveCommand> unackedCommands;
    for (int i = 0; i < actualUnackedMoves; i++) {
        bool left;
        bool right;
        bool jump;
        bool attack;
        signalPacket >> left >> right >> jump >> attack;
        unackedCommands.emplace_back(ClientUpdate::MoveCommand(left, right, jump, attack));
    }

    return ClientUpdate::MoveUpdate(sequenceNumber, unackedCommands);
}

ClientUpdate::ProjectileUpdateAck GameClientConnection::PlayerConnection::readProjectileUpdateAck(sf::Packet& signalPacket) {
    sf::Uint16 sequenceNumber;
    signalPacket >> sequenceNumber;

    // std::cout << "RECEIVED ACK FOR PROJECTILE UPDATE SEQ " << sequenceNumber << " from connection with port " << address.port << std::endl;

    return ClientUpdate::ProjectileUpdateAck(sequenceNumber);
}

void GameClientConnection::PlayerConnection::markProjectileUpdateAsAcked(int sequenceNumber) {
    if (sequenceNumber > lastAckedProjectileUpdateSeqNumber) {
        int commandsBeingAcked = sequenceNumber - lastAckedProjectileUpdateSeqNumber;
        for (int i = 0; i < commandsBeingAcked; i++) {
            unackedUpdates.pop_front();
        }
        lastAckedProjectileUpdateSeqNumber = sequenceNumber;
    }
}

void GameClientConnection::PlayerConnection::ackMoves(ClientUpdate::MoveUpdate& moveUpdate) {
    sf::Packet ackPacket;
    ackPacket << static_cast<sf::Int8>(ServerSignal::MOVE_COMMAND_ACK) << static_cast<sf::Uint16>(moveUpdate.sequenceNumber);
    udpSocket.send(ackPacket, address.ip, address.port);

    if (moveUpdate.sequenceNumber > lastAckedMoveCmdSeqNumber) {
        lastAckedMoveCmdSeqNumber = moveUpdate.sequenceNumber;
        
    }
}

void GameClientConnection::PlayerConnection::queueProjectileCreationBroadcast(sf::Uint16 time, sf::Vector2f position, SimulationProperties::Direction direction, bool firedByPlayer1) {
    unackedUpdates.emplace_back(std::make_unique<ProjectileCreatedUpdate>(time, position, direction, firedByPlayer1));
    projectileUpdateSeqNumber++;
}

void GameClientConnection::PlayerConnection::queueProjectileHitBroadcast(bool hitPlayer1) {
    unackedUpdates.emplace_back(std::make_unique<ProjectileHitUpdate>(hitPlayer1));
    projectileUpdateSeqNumber++;
}

bool GameClientConnection::PlayerConnection::allProjectileUpdatesAcked() {
    return unackedUpdates.empty();
}
