#include "MainMenuScene.h"
#include "TextNode.h"
#include "SpriteNode.h"
#include "CameraNode.h"
#include "EmptySceneNode.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "FpsDisplay.h"
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <iostream>

MainMenuScene::MainMenuScene(GameSceneDirector* sceneDirector, ResourceLoader* resourceLoader, GameMetricsTracker* gameMetricsTracker) : sceneDirector(sceneDirector), resourceLoader(resourceLoader), gameMetricsTracker(gameMetricsTracker), rootSceneNode(new EmptySceneNode()) {
	buildScene();
	presentMenu();
}

MainMenuScene::~MainMenuScene() = default;

void MainMenuScene::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::Escape && isPressed) {
		sceneDirector->concludeCurrentScene();
		return;
	}
	
	switch (state) {
	case State::PRESENTING_MENU:
		if (key == sf::Keyboard::Enter && isPressed) {
			connectToServer();
			// sceneDirector->initiateScene(GameSceneDirector::SceneId::BATTLE);
		}
		break;
	case State::CONNECTING_TO_SERVER: break;
	case State::WAITING_FOR_CHALLENGER: break;
	default:
		throw std::runtime_error("Unknown main menu scene state");
	}
}

void MainMenuScene::update(sf::Time deltaTime) {
	switch (state) {
	case State::PRESENTING_MENU:
		;
	}
}

SceneNode* MainMenuScene::getRootSceneNode() {
	return rootSceneNode.get();
}

void MainMenuScene::buildScene() {
	std::unique_ptr<CameraNode> camera(new CameraNode(sf::Vector2f(1920.0f, 1080.0f), 960.0f, 540.0f));
	rootSceneNode->attachChild(std::move((camera)));

	sf::Sprite backgroundSprite = sf::Sprite(resourceLoader->getTexture(ResourceLoader::TextureId::BACKGROUND));

	std::unique_ptr<SpriteNode> background(new SpriteNode(backgroundSprite));
	background->setPosition(0.0f, 0.0f);
	rootSceneNode->attachChild(std::move(background));

	sf::Text gameTitleText;
	gameTitleText.setFont(resourceLoader->getFont(ResourceLoader::FontId::GAME_TITLE));
	gameTitleText.setString("Sahhara");
	gameTitleText.setCharacterSize(250);
	gameTitleText.setFillColor(sf::Color::Black);

	std::unique_ptr<TextNode> gameTitle(new TextNode(gameTitleText));
	gameTitle->setPosition(425.0f, 150.0f);
	rootSceneNode->attachChild(std::move(gameTitle));

	std::unique_ptr<FpsDisplay> fpsDisplay(new FpsDisplay(gameMetricsTracker));
	fpsDisplay->getText()->setFont(resourceLoader->getFont(ResourceLoader::FontId::FPS_DISPLAY));
	rootSceneNode->attachChild(std::move((fpsDisplay)));
}

void MainMenuScene::presentMenu() {
	state = State::PRESENTING_MENU;

	sf::Text findBattleText;
	findBattleText.setFont(resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
	findBattleText.setString("Find a Challenger");
	findBattleText.setStyle(sf::Text::Bold);
	findBattleText.setCharacterSize(75);
	findBattleText.setFillColor(sf::Color::Black);
	findBattleText.setOutlineColor(sf::Color::Yellow);
	findBattleText.setOutlineThickness(3);

	std::unique_ptr<TextNode> findBattleButton(new TextNode(findBattleText));
	findBattleButton->setPosition(725.0f, 515.0f);
	rootSceneNode->attachChild(std::move(findBattleButton));

	sf::Text exitText;
	exitText.setFont(resourceLoader->getFont(ResourceLoader::FontId::GAME_TEXT));
	exitText.setString("Exit");
	exitText.setStyle(sf::Text::Bold);
	exitText.setCharacterSize(75);
	exitText.setFillColor(sf::Color::Black);

	std::unique_ptr<TextNode> exitButton(new TextNode(exitText));
	exitButton->setPosition(875.0f, 615.0f);
	rootSceneNode->attachChild(std::move(exitButton));
}

void MainMenuScene::connectToServer() {
	state = State::CONNECTING_TO_SERVER;

	std::cout << "Connecting to the server...";

	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(sf::IpAddress("127.0.0.1"), 53000);
	if (status != sf::Socket::Done) {
		std::cout << "Failed to connect to the server";
	}

	std::cout << "Connected to the server";
}
