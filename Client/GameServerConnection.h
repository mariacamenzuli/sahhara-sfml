#pragma once

#include "AuthoritativeGameUpdate.h"

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <memory>
#include <queue>


class GameServerConnection {
public:
    GameServerConnection();
    ~GameServerConnection();

    // Find and start game
    NonBlockingNetOpStatus connectToGameLobby();
    NonBlockingNetOpStatus pingCheck();
    void disconnectFromGameLobby();
    bool getGameInitParameters(bool& isPlayer1Local, unsigned short& serverUdpPort);
    void setServerGameRunningSocketPort(unsigned short serverGameRunningSocketPort);
    NonBlockingNetOpStatus findGame();
    NonBlockingNetOpStatus acceptGame();
    NonBlockingNetOpStatus verifyGameLaunch(bool* gameOn);
    NonBlockingNetOpStatus getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate);

    // Run game
    void sendMoveCommand(bool left, bool right, bool jump, bool attack);
    void markMoveCommandAsAcked(int sequenceNumber);

private:
    // Log related counters
    int failedLobbyConnectAttempts;
    int networkOperationAttempts;

    // Sockets & ports
    const sf::IpAddress serverIp = sf::IpAddress("127.0.0.1");
    const unsigned short serverLobbyPort = 53000;
    std::unique_ptr<sf::TcpSocket> serverTcpSocket;
    std::unique_ptr<sf::UdpSocket> gameRunningUdpSocket;
    unsigned short gameRunningSocketPort = 0;

    // Move command ack tracking
    struct MoveCommand {
        bool left;
        bool right;
        bool jump;
        bool attack;

        MoveCommand(bool left, bool right, bool jump, bool attack) : left(left), right(right), jump(jump), attack(attack) {}
    };
    sf::Uint16 moveCommandSeqNumber = 0;
    std::deque<MoveCommand> unackedCommands;
    int latestAckedSeqNumber = -1;

    // Projectile update tracking
    int lastAckedProjectileUpdateSeqNumber = -1;

    // Other
    bool connectingToLobby = false;

    bool bindGameRunningConnection(unsigned short& udpSocketPort);
    AuthoritativeGameUpdate::PlayerPositionUpdate readPlayerPositionUpdate(sf::Packet& signalPacket);
    AuthoritativeGameUpdate::MoveCommandAckUpdate readMoveCommandAckUpdate(sf::Packet& signalPacket);
    AuthoritativeGameUpdate::ProjectileUpdate readProjectileUpdate(sf::Packet& signalPacket);
    void ackProjectileUpdate(AuthoritativeGameUpdate::ProjectileUpdate& projectileUpdate);
};
