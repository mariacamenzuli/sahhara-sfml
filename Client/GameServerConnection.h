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
    bool connectToGameLobby();
    void disconnectFromGameLobby();
    bool getGameInitParameters(bool& isPlayer1Local, unsigned short& serverUdpPort);
    void setServerGameRunningSocketPort(unsigned short serverGameRunningSocketPort);
    NonBlockingNetOpStatus findGame();
    NonBlockingNetOpStatus acceptGame();
    NonBlockingNetOpStatus verifyGameLaunch(bool* gameOn);
    NonBlockingNetOpStatus getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate);

    // Run game
    void sendMoveCommand(bool left, bool right, bool jump);
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

        MoveCommand(bool left, bool right, bool jump) : left(left), right(right), jump(jump) {}
    };
    sf::Uint16 moveCommandSeqNumber = 0;
    std::deque<MoveCommand> unackedCommands;
    int lastAckedSeqNumber = -1;

    bool bindGameRunningConnection(unsigned short& udpSocketPort);
    AuthoritativeGameUpdate::PlayerPositionUpdate readPlayerPositionUpdate(sf::Packet signalPacket);
    AuthoritativeGameUpdate::MoveCommandAckUpdate readMoveCommandAckUpdate(sf::Packet signalPacket);
};
