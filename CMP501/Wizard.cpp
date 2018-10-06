#include "Wizard.h"

Wizard::Wizard(ResourceLoader* resourceLoader) {
	buildSprite(resourceLoader);
	direction = RIGHT;
}

Wizard::~Wizard() = default;

void Wizard::setDirection(Direction direction) {
	this->direction = direction;
}

void Wizard::run() {
	if (direction == RIGHT) {
		setAnimation(animationRunRightId);
		
	} else {
		setAnimation(animationRunLeftId);
	}
}

void Wizard::idle() {
	if (direction == RIGHT) {
		setAnimation(animationIdleRightId);

	} else {
		setAnimation(animationIdleLeftId);
	}
}

void Wizard::buildSprite(ResourceLoader* resourceLoader) {
	Animation deadRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	deadRightAnimation.addFrame(sf::IntRect(0, 174, 174, 165));
	deadRightAnimation.addFrame(sf::IntRect(174, 174, 174, 141), sf::Vector2f(11.0f, 27.0f));
	deadRightAnimation.addFrame(sf::IntRect(348, 174, 174, 141), sf::Vector2f(11.0f, 20.0f));
	deadRightAnimation.addFrame(sf::IntRect(522, 174, 143, 93), sf::Vector2f(15.0f, 70.0f));

	Animation deadLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	deadLeftAnimation.addFrame(sf::IntRect(1436, 174, 174, 165));
	deadLeftAnimation.addFrame(sf::IntRect(1262, 174, 174, 141), sf::Vector2f(-11.0f, 27.0f));
	deadLeftAnimation.addFrame(sf::IntRect(1088, 174, 174, 141), sf::Vector2f(-11.0f, 20.0f));
	deadLeftAnimation.addFrame(sf::IntRect(945, 174, 143, 93), sf::Vector2f(10.0f, 70.0f));

	Animation hurtRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	hurtRightAnimation.addFrame(sf::IntRect(0, 339, 174, 165));
	hurtRightAnimation.addFrame(sf::IntRect(174, 339, 174, 150), sf::Vector2f(13.0f, 18.0f));
	hurtRightAnimation.addFrame(sf::IntRect(348, 339, 174, 165));

	Animation hurtLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	hurtLeftAnimation.addFrame(sf::IntRect(1436, 339, 174, 165));
	hurtLeftAnimation.addFrame(sf::IntRect(1262, 339, 174, 150), sf::Vector2f(-13.0f, 18.0f));
	hurtLeftAnimation.addFrame(sf::IntRect(1088, 339, 174, 165));

	Animation runRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	runRightAnimation.addFrame(sf::IntRect(0, 663, 166, 156));
	runRightAnimation.addFrame(sf::IntRect(166, 663, 166, 156));
	runRightAnimation.addFrame(sf::IntRect(332, 663, 166, 156));
	runRightAnimation.addFrame(sf::IntRect(498, 663, 166, 156));

	Animation runLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	runLeftAnimation.addFrame(sf::IntRect(1444, 663, 166, 156));
	runLeftAnimation.addFrame(sf::IntRect(1278, 663, 166, 156));
	runLeftAnimation.addFrame(sf::IntRect(1112, 663, 166, 156));
	runLeftAnimation.addFrame(sf::IntRect(946, 663, 166, 156));

	Animation jumpRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	jumpRightAnimation.addFrame(sf::IntRect(286, 504, 172, 145));
	jumpRightAnimation.addFrame(sf::IntRect(458, 504, 130, 159));
	jumpRightAnimation.addFrame(sf::IntRect(588, 504, 172, 145));

	Animation jumpLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	jumpLeftAnimation.addFrame(sf::IntRect(1152, 504, 172, 145));
	jumpLeftAnimation.addFrame(sf::IntRect(1022, 504, 130, 159));
	jumpLeftAnimation.addFrame(sf::IntRect(850, 504, 172, 145));

	Animation attackRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	attackRightAnimation.addFrame(sf::IntRect(0, 0, 168, 143));
	attackRightAnimation.addFrame(sf::IntRect(168, 0, 258, 163), sf::Vector2f(9.0f, -20.0f));
	attackRightAnimation.addFrame(sf::IntRect(426, 0, 291, 174), sf::Vector2f(11.0f, -33.0f));

	Animation attackLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	attackLeftAnimation.addFrame(sf::IntRect(1442, 0, 168, 143));
	attackLeftAnimation.addFrame(sf::IntRect(1184, 0, 258, 163), sf::Vector2f(-98.0f, -20.0f));
	attackLeftAnimation.addFrame(sf::IntRect(893, 0, 291, 174), sf::Vector2f(-133.0f, -33.0f));

	Animation idleRightAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	idleRightAnimation.addFrame(sf::IntRect(522, 339, 143, 150));
	idleRightAnimation.addFrame(sf::IntRect(665, 339, 143, 148), sf::Vector2f(1.0f, 0.0f));
	idleRightAnimation.addFrame(sf::IntRect(0, 504, 143, 148), sf::Vector2f(0.0f, 1.0f));
	idleRightAnimation.addFrame(sf::IntRect(143, 504, 143, 148), sf::Vector2f(2.0f, 1.0f));

	Animation idleLeftAnimation(resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
	idleLeftAnimation.addFrame(sf::IntRect(945, 339, 143, 148));
	idleLeftAnimation.addFrame(sf::IntRect(802, 339, 143, 150), sf::Vector2f(-1.0f, 0.0f));
	idleLeftAnimation.addFrame(sf::IntRect(1467, 504, 143, 148), sf::Vector2f(0.0f, -1.0f));
	idleLeftAnimation.addFrame(sf::IntRect(1324, 504, 143, 148), sf::Vector2f(-2.0f, -1.0f));

	addAnimation(animationRunRightId, runRightAnimation, (sf::seconds(0.133f)));
	addAnimation(animationRunLeftId, runLeftAnimation, (sf::seconds(0.133f)));
	addAnimation(animationJumpRightId, jumpRightAnimation, (sf::seconds(0.75f)));
	addAnimation(animationJumpLeftId, jumpLeftAnimation, (sf::seconds(0.75f)));
	addAnimation(animationAttackRightId, attackRightAnimation, (sf::seconds(0.033f)));
	addAnimation(animationAttackLeftId, attackLeftAnimation, (sf::seconds(0.033f)));
	addAnimation(animationIdleRightId, idleRightAnimation, (sf::seconds(0.75f)));
	addAnimation(animationIdleLeftId, idleLeftAnimation, (sf::seconds(0.75f)));
	addAnimation(animationHurtRightId, hurtRightAnimation, (sf::seconds(0.75f)));
	addAnimation(animationHurtLeftId, hurtLeftAnimation, (sf::seconds(0.75f)));
	addAnimation(animationDeadRightId, deadRightAnimation, (sf::seconds(0.75f)));
	addAnimation(animationDeadLeftId, deadLeftAnimation, (sf::seconds(0.75f)));
}
