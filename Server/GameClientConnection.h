#pragma once

#include "PlayerAddress.h"
#include "NetworkCommunicationSignals.h"
#include "ClientUpdate.h"
#include "ThreadLogger.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Network/UdpSocket.hpp>

class GameClientConnection {
public:
    GameClientConnection(ThreadLogger logger, sf::IpAddress player1RemoteIp, unsigned short player1RemotePort, sf::IpAddress player2RemoteIp, unsigned short player2RemotePort);
    ~GameClientConnection();

    void initialize(unsigned short& player1UdpSocketPort, unsigned short& player2UdpSocketPort);

    NonBlockingNetOpStatus getPlayer1Update(ClientUpdate& clientUpdate);
    NonBlockingNetOpStatus getPlayer2Update(ClientUpdate& clientUpdate);
    void broadcastPlayer1Position(sf::Vector2<float> position);
    void broadcastPlayer2Position(sf::Vector2<float> position);

private:
    ThreadLogger logger;

    struct PlayerConnection {
        PlayerAddress address;
        sf::UdpSocket udpSocket;
        int lastAckedMoveCmdSeqNumber = -1;

        PlayerConnection(sf::IpAddress ip, unsigned short port) : address(ip, port) {
        }

        NonBlockingNetOpStatus getNetworkUpdate(ClientUpdate& clientUpdate);
        ClientUpdate::MoveUpdate readMoveUpdate(sf::Packet signalPacket);
        void ackMoves(int moveCmdSeqNumber);
    };

    PlayerConnection player1Connection;
    PlayerConnection player2Connection;
};

