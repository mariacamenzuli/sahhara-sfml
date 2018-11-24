#pragma once
#include <SFML/Network/IpAddress.hpp>

class PlayerAddress {
public:
    PlayerAddress(sf::IpAddress ip, unsigned short port);
    ~PlayerAddress();

    const sf::IpAddress ip;
    const unsigned short port;
};

