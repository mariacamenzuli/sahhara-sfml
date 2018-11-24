#pragma once

#include "PlayerAddress.h"
#include "NetworkCommunicationSignals.h"
#include "ClientUpdate.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Network/UdpSocket.hpp>

class GameClientConnection {
public:
    GameClientConnection(sf::IpAddress player1RemoteIp, unsigned short player1RemotePort, sf::IpAddress player2RemoteIp, unsigned short player2RemotePort);
    ~GameClientConnection();

    void initialize(unsigned short& player1UdpSocketPort, unsigned short& player2UdpSocketPort);

    NonBlockingNetOpStatus getPlayer1Update(ClientUpdate& clientUpdate);
    NonBlockingNetOpStatus getPlayer2Update(ClientUpdate& clientUpdate);
    void broadcastPlayer1Position(sf::Vector2<float> position);
    void broadcastPlayer2Position(sf::Vector2<float> position);

private:
    sf::UdpSocket player1UdpSocket;
    sf::UdpSocket player2UdpSocket;

    PlayerAddress player1Address;
    PlayerAddress player2Address;

    static NonBlockingNetOpStatus getPlayerUpdate(sf::UdpSocket& playerSocket, ClientUpdate& clientUpdate);
};

