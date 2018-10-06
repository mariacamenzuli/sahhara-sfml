#pragma once

#include "AnimatedSpriteNode.h"
#include "ResourceLoader.h"

class Wizard : public AnimatedSpriteNode {
public:
	Wizard(ResourceLoader* resourceLoader);
	~Wizard();

	const float runVelocity = 500;
	const float jumpKickOffVelocity = -650.0f;
	const float jumpVelocity = -500.0f;
	const float maxAirTime = 0.35f;
	const float jumpKickOffTime = 0.1f;

	enum Direction {
		RIGHT, LEFT
	};

	void run();
	void idle();

	Direction direction;
	float timeInAir = 0.0f;

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

	void buildSprite(ResourceLoader* resourceLoader);
};
