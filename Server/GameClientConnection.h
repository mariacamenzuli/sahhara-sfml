#pragma once

#include "PlayerAddress.h"
#include "NetworkCommunicationSignals.h"
#include "ClientUpdate.h"
#include "ThreadLogger.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include "ProjectileUpdate.h"

class GameClientConnection {
public:
    GameClientConnection(ThreadLogger logger, sf::IpAddress player1RemoteIp, unsigned short player1RemotePort, sf::IpAddress player2RemoteIp, unsigned short player2RemotePort);
    ~GameClientConnection();

    void initialize(unsigned short& player1UdpSocketPort, unsigned short& player2UdpSocketPort);

    NonBlockingNetOpStatus getPlayer1Update(ClientUpdate& clientUpdate);
    NonBlockingNetOpStatus getPlayer2Update(ClientUpdate& clientUpdate);
    void broadcastPlayerPositions(sf::Uint16 time, bool player1PositionChanged, sf::Vector2<float> player1Position, bool player2PositionChanged, sf::Vector2<float> player2Position);
    void queueProjectileCreationBroadcast(sf::Vector2f position, SimulationProperties::Direction direction, bool firedByPlayer1);
    void sendUnackedProjectileUpdates();

private:
    ThreadLogger logger;

    struct PlayerConnection {
        PlayerAddress address;
        sf::UdpSocket udpSocket;
        int lastAckedMoveCmdSeqNumber = -1;

        // Projectile related server update ack tracking
        int lastAckedProjectileUpdateSeqNumber = -1;
        sf::Uint16 projectileUpdateSeqNumber = -1;
        std::deque<std::unique_ptr<ProjectileUpdate>> unackedUpdates;

        PlayerConnection(sf::IpAddress ip, unsigned short port) : address(ip, port) {
        }

        NonBlockingNetOpStatus getNetworkUpdate(ClientUpdate& clientUpdate);
        ClientUpdate::MoveUpdate readMoveUpdate(sf::Packet signalPacket);
        void ackMoves(ClientUpdate::MoveUpdate& moveUpdate);

        void sendUnackedProjectileUpdates();
        void queueProjectileCreationBroadcast(sf::Vector2f position, SimulationProperties::Direction direction, bool firedByPlayer1);
    };

    PlayerConnection player1Connection;
    PlayerConnection player2Connection;
};

