#include "GameServerConnection.h"
#include "NetworkCommunicationSignals.h"

#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

GameServerConnection::GameServerConnection() : failedLobbyConnectAttempts(0),
                                               networkOperationAttempts(0),
                                               serverTcpSocket(new sf::TcpSocket()) {
}

GameServerConnection::~GameServerConnection() {
    serverTcpSocket->disconnect();
};

bool GameServerConnection::connectToGameLobby() {
    serverTcpSocket->setBlocking(true);
    const sf::Socket::Status status = serverTcpSocket->connect(sf::IpAddress("127.0.0.1"), 53000, sf::microseconds(1));
    if (status == sf::Socket::Done) {
        std::cout << "Connected to game lobby." << std::endl;
        serverTcpSocket->setBlocking(false);
        return true;
    } else {
        if (failedLobbyConnectAttempts % 120 == 0) {
            std::cout << "Attempting to connect to game lobby..." << std::endl;
        }
        failedLobbyConnectAttempts++;
        return false;
    }
}

GameServerConnection::NonBlockingNetOpStatus GameServerConnection::findGame() {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Waiting for a game match..." << std::endl;
    }

    sf::Packet matchFoundPacket;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(matchFoundPacket);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        networkOperationAttempts++;
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signal;
    matchFoundPacket >> signal;

    if (signal != ServerSignal::FOUND_GAME_MATCH) {
        networkOperationAttempts++;
        std::cout << "Received an unexpected signal from the server. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    }

    std::cout << "Game match found." << std::endl;
    networkOperationAttempts = 0;

    return NonBlockingNetOpStatus::COMPLETE;
}

GameServerConnection::NonBlockingNetOpStatus GameServerConnection::acceptGame() {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Signaling acceptance of game match..." << std::endl;
    }

    sf::Packet acceptMatchPacket;
    acceptMatchPacket << static_cast<sf::Int8>(ClientSignal::READY_FOR_MATCH);
    const auto clientDataSentStatus = serverTcpSocket->send(acceptMatchPacket);

    if (clientDataSentStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (clientDataSentStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (clientDataSentStatus != sf::Socket::Done) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    std::cout << "Acceptance signaled." << std::endl;
    networkOperationAttempts = 0;

    return NonBlockingNetOpStatus::COMPLETE;
}

GameServerConnection::NonBlockingNetOpStatus GameServerConnection::verifyGameLaunch(bool* gameOn) {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Waiting for a game match to be verified..." << std::endl;
    }

    *gameOn = false;

    sf::Packet gameOnOrOffPacket;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(gameOnOrOffPacket);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        networkOperationAttempts++;
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signal;
    gameOnOrOffPacket >> signal;

    if (signal == ServerSignal::GAME_OFF) {
        std::cout << "Game has been called off. Opponent may have disconnected." << std::endl;
        return NonBlockingNetOpStatus::COMPLETE;
    } else if (signal != ServerSignal::GAME_ON) {
        std::cout << "Received an unexpected signal from the server." << std::endl;
        return NonBlockingNetOpStatus::ERROR;
    }

    std::cout << "Game match verified. Launching game." << std::endl;
    *gameOn = true;
    networkOperationAttempts = 0;

    return NonBlockingNetOpStatus::COMPLETE;
}

GameServerConnection::NonBlockingNetOpStatus GameServerConnection::getAuthoritativeGameUpdate(AuthoritativeGameUpdate& gameUpdate) {
    // todo: handle 1 receive not getting an entire command or getting more than one command
    // todo: also listen on a udp socket
    sf::Packet signalPacket;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(signalPacket);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        return NonBlockingNetOpStatus::NOT_READY;
    }

    sf::Int8 signalType;
    signalPacket >> signalType;
    gameUpdate.type = AuthoritativeGameUpdate::determineUpdateType(signalType);
    switch (gameUpdate.type) {
    case AuthoritativeGameUpdate::Type::INIT:
        bool isPlayer1;
        signalPacket >> isPlayer1;
        gameUpdate.init = AuthoritativeGameUpdate::InitUpdate(isPlayer1);
        break;
    case AuthoritativeGameUpdate::Type::UNKNOWN:
    default:
        std::cout << "Received an unexpected signal from the server. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetOpStatus::ERROR;
    }

    return NonBlockingNetOpStatus::COMPLETE;

}
