#pragma once
#include <SFML/Network/UdpSocket.hpp>
#include "PlayerAddress.h"
#include "NetworkCommunicationSignals.h"
#include "ClientUpdate.h"

class GameClientConnection {
public:
    GameClientConnection(sf::IpAddress player1RemoteIp, unsigned short player1RemotePort, sf::IpAddress player2RemoteIp, unsigned short player2RemotePort);
    ~GameClientConnection();

    void initialize(unsigned short& player1UdpSocketPort, unsigned short& player2UdpSocketPort);
    NonBlockingNetOpStatus getPlayer1Update(ClientUpdate& clientUpdate);
    NonBlockingNetOpStatus getPlayer2Update(ClientUpdate& clientUpdate);

private:
    sf::UdpSocket player1UdpSocket;
    sf::UdpSocket player2UdpSocket;

    PlayerAddress player1Address;
    PlayerAddress player2Address;

    static NonBlockingNetOpStatus getPlayerUpdate(sf::UdpSocket& playerSocket, ClientUpdate& clientUpdate);
};

