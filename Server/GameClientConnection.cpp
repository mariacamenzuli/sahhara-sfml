#include "GameClientConnection.h"

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

void GameClientConnection::broadcastPlayer1Position(sf::Vector2<float> position) {
    sf::Packet player1PositionPacket; //todo add seq number or timestamp
    player1PositionPacket << static_cast<sf::Int8>(ServerSignal::PLAYER_POSITION_UPDATE) << ServerSignal::IS_PLAYER_1 << position.x << position.y;
    player1Connection.udpSocket.send(player1PositionPacket, player1Connection.address.ip, player1Connection.address.port);
    player2Connection.udpSocket.send(player1PositionPacket, player2Connection.address.ip, player2Connection.address.port);
}

void GameClientConnection::broadcastPlayer2Position(sf::Vector2<float> position) {
    sf::Packet player2PositionPacket; //todo add seq number or timestamp
    player2PositionPacket << static_cast<sf::Int8>(ServerSignal::PLAYER_POSITION_UPDATE) << ServerSignal::IS_NOT_PLAYER_1 << position.x << position.y;
    player1Connection.udpSocket.send(player2PositionPacket, player1Connection.address.ip, player1Connection.address.port);
    player2Connection.udpSocket.send(player2PositionPacket, player2Connection.address.ip, player2Connection.address.port);
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
        ackMoves(clientUpdate.move.sequenceNumber);
        break;
    case ClientUpdate::Type::UNKNOWN:
    default:
        return NonBlockingNetOpStatus::ERROR;
    }

    return NonBlockingNetOpStatus::COMPLETE;
}

ClientUpdate::MoveUpdate GameClientConnection::PlayerConnection::readMoveUpdate(sf::Packet signalPacket) {
    sf::Uint16 sequenceNumber;
    sf::Uint16 unackedMovesInPacket;
    signalPacket >> sequenceNumber >> unackedMovesInPacket;

    int actualUnackedMoves = sequenceNumber - lastAckedMoveCmdSeqNumber;
    std::vector<ClientUpdate::MoveCommand> unackedCommands;
    for (int i = 0; i < actualUnackedMoves; i++) {
        bool left;
        bool right;
        bool jump;
        signalPacket >> left >> right >> jump;
        unackedCommands.emplace_back(ClientUpdate::MoveCommand(left, right, jump));
    }

    return ClientUpdate::MoveUpdate(sequenceNumber, unackedCommands);
}

void GameClientConnection::PlayerConnection::ackMoves(int moveCmdSeqNumber) {
    sf::Packet ackPacket;
    ackPacket << static_cast<sf::Int8>(ServerSignal::MOVE_COMMAND_ACK) << static_cast<sf::Uint16>(moveCmdSeqNumber);
    udpSocket.send(ackPacket, address.ip, address.port);
    lastAckedMoveCmdSeqNumber = moveCmdSeqNumber;
}
