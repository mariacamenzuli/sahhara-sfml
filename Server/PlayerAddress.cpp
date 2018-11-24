#include "PlayerAddress.h"

PlayerAddress::PlayerAddress(sf::IpAddress ip, unsigned short port) : ip(ip), port(port) {
}

PlayerAddress::~PlayerAddress() = default;
