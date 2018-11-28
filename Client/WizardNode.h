#pragma once

#include "AnimatedSpriteNode.h"
#include "ResourceLoader.h"

namespace  WizardAnimation {
    const int RUN_RIGHT = 1;
    const int RUN_LEFT = 2;
    const int DEAD_RIGHT = 3;
    const int DEAD_LEFT = 4;
    const int HURT_RIGHT = 5;
    const int HURT_LEFT = 6;
    const int IDLE_RIGHT = 7;
    const int IDLE_LEFT = 8;
    const int JUMP_RIGHT = 9;
    const int JUMP_LEFT = 10;
    const int ATTACK_RIGHT = 11;
    const int ATTACK_LEFT = 12;
}

class WizardNode : public AnimatedSpriteNode {
public:
    enum class Direction {
        RIGHT,
        LEFT
    };

    enum class Color {
        PURPLE,
        ORANGE
    };

    WizardNode(Color color, ResourceLoader* resourceLoader);
    ~WizardNode();

    void run();
    void idle();
    void attack();
    
    Direction direction;
    float timeInAir = 0.0f;

private:
    Color color;

    void buildSprite(ResourceLoader* resourceLoader);
    inline Animation buildRunRightAnimation(ResourceLoader* resourceLoader);
    inline Animation buildRunLeftAnimation(ResourceLoader* resourceLoader);
    inline Animation buildDeadRightAnimation(ResourceLoader* resourceLoader);
    inline Animation buildDeadLeftAnimation(ResourceLoader* resourceLoader);
    inline Animation buildHurtRightAnimation(ResourceLoader* resourceLoader);
    inline Animation buildHurtLeftAnimation(ResourceLoader* resourceLoader);
    inline Animation buildIdleRightAnimation(ResourceLoader* resourceLoader);
    inline Animation buildIdleLeftAnimation(ResourceLoader* resourceLoader);
    inline Animation buildJumpRightAnimation(ResourceLoader* resourceLoader);
    inline Animation buildJumpLeftAnimation(ResourceLoader* resourceLoader);
    inline Animation buildAttackRightAnimation(ResourceLoader* resourceLoader);
    inline Animation buildAttackLeftAnimation(ResourceLoader* resourceLoader);
};
