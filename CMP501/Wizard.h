#pragma once

#include "AnimatedSpriteNode.h"
#include "ResourceLoader.h"

class Wizard : public AnimatedSpriteNode {
public:
	Wizard(ResourceLoader* resourceLoader);
	~Wizard();

	const float runVelocity = 500;

	enum Direction {
		RIGHT, LEFT
	};

	void setDirection(Direction direction);
	void run();
	void idle();

private:
	const int animationRunRightId = 1;
	const int animationRunLeftId = 2;
	const int animationDeadRightId = 3;
	const int animationDeadLeftId = 4;
	const int animationHurtRightId = 5;
	const int animationHurtLeftId = 6;
	const int animationIdleRightId = 7;
	const int animationIdleLeftId = 8;
	const int animationJumpRightId = 9;
	const int animationJumpLeftId = 10;
	const int animationAttackRightId = 11;
	const int animationAttackLeftId = 12;

	Direction direction;

	void buildSprite(ResourceLoader* resourceLoader);
};
