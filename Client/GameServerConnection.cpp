#include "GameServerConnection.h"
#include "NetworkCommunicationSignals.h"

#include <iostream>
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

GameServerConnection::NonBlockingNetworkOperationStatus GameServerConnection::findGame() {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Waiting for a game match..." << std::endl;
    }

    char dataReceived[1];
    std::size_t receiveCount;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(dataReceived, 1, receiveCount);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetworkOperationStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetworkOperationStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        networkOperationAttempts++;
        return NonBlockingNetworkOperationStatus::NOT_READY;
    }

    if (dataReceived[0] != ServerSignal::FOUND_GAME_MATCH) {
        networkOperationAttempts++;
        std::cout << "Received an unexpected signal from the server. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetworkOperationStatus::ERROR;
    }

    std::cout << "Game match found." << std::endl;
    networkOperationAttempts = 0;

    return NonBlockingNetworkOperationStatus::COMPLETE;
}

GameServerConnection::NonBlockingNetworkOperationStatus GameServerConnection::acceptGame() {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Signaling acceptance of game match..." << std::endl;
    }

    std::size_t sendCount = 0;
    const auto clientDataSentStatus = serverTcpSocket->send(&ClientSignal::READY_FOR_MATCH, 1, sendCount);

    if (clientDataSentStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetworkOperationStatus::ERROR;
    } else if (clientDataSentStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetworkOperationStatus::ERROR;
    } else if (clientDataSentStatus != sf::Socket::Done || sendCount == 0) {
        return NonBlockingNetworkOperationStatus::NOT_READY;
    }

    std::cout << "Acceptance signaled." << std::endl;
    networkOperationAttempts = 0;

    return NonBlockingNetworkOperationStatus::COMPLETE;
}

GameServerConnection::NonBlockingNetworkOperationStatus GameServerConnection::verifyGameLaunch(bool* gameOn) {
    if (networkOperationAttempts % 120 == 0) {
        std::cout << "Waiting for a game match to be verified..." << std::endl;
    }

    *gameOn = false;

    char dataReceived[1];
    std::size_t receiveCount;
    const auto serverDataReceiveStatus = serverTcpSocket->receive(dataReceived, 1, receiveCount);

    if (serverDataReceiveStatus == sf::Socket::Error) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Resetting connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetworkOperationStatus::ERROR;
    } else if (serverDataReceiveStatus == sf::Socket::Disconnected) {
        networkOperationAttempts++;
        std::cout << "An unexpected error has occurred. Lost connection to the server." << std::endl;
        serverTcpSocket->disconnect();
        return NonBlockingNetworkOperationStatus::ERROR;
    } else if (serverDataReceiveStatus != sf::Socket::Done) {
        networkOperationAttempts++;
        return NonBlockingNetworkOperationStatus::NOT_READY;
    }

    if (dataReceived[0] == '-') {
        std::cout << "Game has been called off. Opponent may have disconnected." << std::endl;
        return NonBlockingNetworkOperationStatus::COMPLETE;
    }

    std::cout << "Game match verified. Launching game." << std::endl;
    *gameOn = true;
    networkOperationAttempts = 0;

    return NonBlockingNetworkOperationStatus::COMPLETE;
}

bool GameServerConnection::getIsPlayer1() {
    char dataReceived[2];
    std::size_t receiveCount;
    serverTcpSocket->setBlocking(true); //todo fix
    const auto serverDataReceiveStatus = serverTcpSocket->receive(dataReceived, 2, receiveCount);
    serverTcpSocket->setBlocking(false);
    bool isPlayer1 = dataReceived[1];
    return isPlayer1;
}
