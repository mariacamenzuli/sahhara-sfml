#include "GameClientConnection.h"

GameClientConnection::GameClientConnection(sf::IpAddress player1RemoteIp,
                                           unsigned short player1RemotePort,
                                           sf::IpAddress player2RemoteIp,
                                           unsigned short player2RemotePort) : player1Address(player1RemoteIp, player1RemotePort), player2Address(player2RemoteIp, player2RemotePort) {
}

GameClientConnection::~GameClientConnection() {
    player1UdpSocket.unbind();
    player2UdpSocket.unbind();
};

void GameClientConnection::initialize(unsigned short& player1UdpSocketPort, unsigned short& player2UdpSocketPort) {
    if (player1UdpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        //todo: throw excpetion?
    }
    player1UdpSocket.setBlocking(false);
    player1UdpSocketPort = player1UdpSocket.getLocalPort();

    if (player2UdpSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
        //todo: throw excpetion?
    }
    player2UdpSocket.setBlocking(false);
    player2UdpSocketPort = player2UdpSocket.getLocalPort();
}

NonBlockingNetOpStatus GameClientConnection::getPlayer1Update(ClientUpdate& clientUpdate) {
    return getPlayerUpdate(player1UdpSocket, clientUpdate);
}

NonBlockingNetOpStatus GameClientConnection::getPlayer2Update(ClientUpdate& clientUpdate) {
    return getPlayerUpdate(player2UdpSocket, clientUpdate);
}

void GameClientConnection::broadcastPlayer1Position(sf::Vector2<float> position) {
    sf::Packet player1PositionPacket; //todo add seq number or timestamp
    player1PositionPacket << static_cast<sf::Int8>(ServerSignal::PLAYER_POSITION_UPDATE) << ServerSignal::IS_PLAYER_1 << position.x << position.y;
    player1UdpSocket.send(player1PositionPacket, player1Address.ip, player1Address.port);
    player2UdpSocket.send(player1PositionPacket, player2Address.ip, player2Address.port);
}

void GameClientConnection::broadcastPlayer2Position(sf::Vector2<float> position) {
    sf::Packet player2PositionPacket; //todo add seq number or timestamp
    player2PositionPacket << static_cast<sf::Int8>(ServerSignal::PLAYER_POSITION_UPDATE) << ServerSignal::IS_NOT_PLAYER_1 << position.x << position.y;
    player1UdpSocket.send(player2PositionPacket, player1Address.ip, player1Address.port);
    player2UdpSocket.send(player2PositionPacket, player2Address.ip, player2Address.port);
}

NonBlockingNetOpStatus GameClientConnection::getPlayerUpdate(sf::UdpSocket& playerSocket, ClientUpdate& clientUpdate) {
    sf::Packet signalPacket;
    sf::IpAddress senderIpAddress;
    unsigned short senderPort;
    const auto serverDataReceiveStatus = playerSocket.receive(signalPacket, senderIpAddress, senderPort);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        //todo log?
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        //todo log?
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signalType;
    signalPacket >> signalType;
    clientUpdate.type = ClientUpdate::determineUpdateType(signalType);
    switch (clientUpdate.type) {
    case ClientUpdate::Type::MOVE:
        bool left;
        bool right;
        bool jump;
        signalPacket >> left >> right >> jump;
        clientUpdate.move = ClientUpdate::MoveUpdate(left, right, jump);
        break;
    case ClientUpdate::Type::UNKNOWN:
    default:
        //todo log?
        return NonBlockingNetOpStatus::ERROR;
    }

    return NonBlockingNetOpStatus::COMPLETE;
}
