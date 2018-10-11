#include "Wizard.h"

Wizard::Wizard(Color color, ResourceLoader* resourceLoader) : color(color) {
    buildSprite(resourceLoader);
    direction = Direction::RIGHT;
}

Wizard::~Wizard() = default;

void Wizard::run() {
    if (direction == Direction::RIGHT) {
        setAnimation(animationRunRightId);
    } else {
        setAnimation(animationRunLeftId);
    }
}

void Wizard::idle() {
    if (direction == Direction::RIGHT) {
        setAnimation(animationIdleRightId);
    } else {
        setAnimation(animationIdleLeftId);
    }
}

void Wizard::buildSprite(ResourceLoader* resourceLoader) {
    addAnimation(animationRunRightId, buildRunRightAnimation(resourceLoader), (sf::seconds(0.133f)));
    addAnimation(animationRunLeftId, buildRunLeftAnimation(resourceLoader), (sf::seconds(0.133f)));
    addAnimation(animationJumpRightId, buildJumpRightAnimation(resourceLoader), (sf::seconds(0.75f)));
    addAnimation(animationJumpLeftId, buildJumpLeftAnimation(resourceLoader), (sf::seconds(0.75f)));
    addAnimation(animationAttackRightId, buildAttackRightAnimation(resourceLoader), (sf::seconds(0.033f)));
    addAnimation(animationAttackLeftId, buildAttackLeftAnimation(resourceLoader), (sf::seconds(0.033f)));
    addAnimation(animationIdleRightId, buildIdleRightAnimation(resourceLoader), (sf::seconds(0.75f)));
    addAnimation(animationIdleLeftId, buildIdleLeftAnimation(resourceLoader), (sf::seconds(0.75f)));
    addAnimation(animationHurtRightId, buildHurtRightAnimation(resourceLoader), (sf::seconds(0.75f)));
    addAnimation(animationHurtLeftId, buildHurtLeftAnimation(resourceLoader), (sf::seconds(0.75f)));
    addAnimation(animationDeadRightId, buildDeadRightAnimation(resourceLoader), (sf::seconds(0.75f)));
    addAnimation(animationDeadLeftId, buildDeadLeftAnimation(resourceLoader), (sf::seconds(0.75f)));
}

Animation Wizard::buildRunRightAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation runRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        runRightAnimation.addFrame(sf::IntRect(0, 663, 166, 156));
        runRightAnimation.addFrame(sf::IntRect(166, 663, 166, 156));
        runRightAnimation.addFrame(sf::IntRect(332, 663, 166, 156));
        runRightAnimation.addFrame(sf::IntRect(498, 663, 166, 156));
        return runRightAnimation;
    } else {
        Animation runRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        runRightAnimation.addFrame(sf::IntRect(530, 504, 180, 180), sf::Vector2f(-6.0f, -28.0f));
        runRightAnimation.addFrame(sf::IntRect(350, 504, 180, 180), sf::Vector2f(-9.0f, -28.0f));
        runRightAnimation.addFrame(sf::IntRect(170, 504, 180, 180), sf::Vector2f(-12.0f, -23.0f));
        runRightAnimation.addFrame(sf::IntRect(250, 327, 150, 173), sf::Vector2f(0.0f, -19.0f));
        return runRightAnimation;
    }
}

Animation Wizard::buildRunLeftAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation runLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        runLeftAnimation.addFrame(sf::IntRect(1444, 663, 166, 156));
        runLeftAnimation.addFrame(sf::IntRect(1278, 663, 166, 156));
        runLeftAnimation.addFrame(sf::IntRect(1112, 663, 166, 156));
        runLeftAnimation.addFrame(sf::IntRect(946, 663, 166, 156));
        return runLeftAnimation;
    } else {
        Animation runLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        runLeftAnimation.addFrame(sf::IntRect(1290, 504, 180, 180), sf::Vector2f(-8.0f, -27.0f));
        runLeftAnimation.addFrame(sf::IntRect(1470, 504, 180, 180), sf::Vector2f(-8.0f, -27.0f));
        runLeftAnimation.addFrame(sf::IntRect(1650, 504, 180, 180), sf::Vector2f(-4.0f, -23.0f));
        runLeftAnimation.addFrame(sf::IntRect(1600, 327, 150, 173), sf::Vector2f(8.0f, -18.0f));
        return runLeftAnimation;
    }
}

Animation Wizard::buildDeadRightAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation deadRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        deadRightAnimation.addFrame(sf::IntRect(0, 174, 174, 165), sf::Vector2f(0.0f, -15.0f));
        deadRightAnimation.addFrame(sf::IntRect(174, 174, 174, 141), sf::Vector2f(11.0f, 12.0f));
        deadRightAnimation.addFrame(sf::IntRect(348, 174, 174, 141), sf::Vector2f(11.0f, 10.0f));
        deadRightAnimation.addFrame(sf::IntRect(522, 174, 143, 93), sf::Vector2f(15.0f, 70.0f));
        return deadRightAnimation;
    } else {
        Animation deadRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        deadRightAnimation.addFrame(sf::IntRect(824, 0, 176, 158), sf::Vector2f(0.0f, -15.0f));
        deadRightAnimation.addFrame(sf::IntRect(296, 0, 176, 154), sf::Vector2f(11.0f, -10.0f));
        deadRightAnimation.addFrame(sf::IntRect(146, 0, 150, 154), sf::Vector2f(11.0f, 5.0f));
        deadRightAnimation.addFrame(sf::IntRect(0, 0, 146, 128), sf::Vector2f(15.0f, 45.0f));
        return deadRightAnimation;
    }
}

Animation Wizard::buildDeadLeftAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation deadLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        deadLeftAnimation.addFrame(sf::IntRect(1436, 174, 174, 165));
        deadLeftAnimation.addFrame(sf::IntRect(1262, 174, 174, 141), sf::Vector2f(-11.0f, 27.0f));
        deadLeftAnimation.addFrame(sf::IntRect(1088, 174, 174, 141), sf::Vector2f(-11.0f, 20.0f));
        deadLeftAnimation.addFrame(sf::IntRect(945, 174, 143, 93), sf::Vector2f(10.0f, 70.0f));
        return deadLeftAnimation;
    } else {
        Animation deadLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        deadLeftAnimation.addFrame(sf::IntRect(1000, 0, 176, 158), sf::Vector2f(-20.0f, -10.0f));
        deadLeftAnimation.addFrame(sf::IntRect(1528, 0, 176, 154), sf::Vector2f(-30.0f, -8.0f));
        deadLeftAnimation.addFrame(sf::IntRect(1704, 0, 150, 154));
        deadLeftAnimation.addFrame(sf::IntRect(1854, 0, 146, 128), sf::Vector2f(0.0f, 50.0f));
        return deadLeftAnimation;
    }
}

Animation Wizard::buildHurtRightAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation hurtRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        hurtRightAnimation.addFrame(sf::IntRect(0, 339, 174, 165), sf::Vector2f(0.0f, -10.0f));
        hurtRightAnimation.addFrame(sf::IntRect(174, 339, 174, 150), sf::Vector2f(13.0f, 8.0f));
        hurtRightAnimation.addFrame(sf::IntRect(348, 339, 174, 165), sf::Vector2f(0.0f, -10.0f));
        return hurtRightAnimation;
    } else {
        Animation hurtRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        hurtRightAnimation.addFrame(sf::IntRect(472, 0, 176, 158), sf::Vector2f(0.0f, -10.0f));
        hurtRightAnimation.addFrame(sf::IntRect(0, 158, 176, 158), sf::Vector2f(13.0f, -10.0f));
        hurtRightAnimation.addFrame(sf::IntRect(648, 0, 176, 158), sf::Vector2f(0.0f, -10.0f));
        return hurtRightAnimation;
    }
}

Animation Wizard::buildHurtLeftAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation hurtLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        hurtLeftAnimation.addFrame(sf::IntRect(1436, 339, 174, 165), sf::Vector2f(0.0f, -10.0f));
        hurtLeftAnimation.addFrame(sf::IntRect(1262, 339, 174, 150), sf::Vector2f(-13.0f, 8.0f));
        hurtLeftAnimation.addFrame(sf::IntRect(1088, 339, 174, 165), sf::Vector2f(0.0f, -10.0f));
        return hurtLeftAnimation;
    } else {
        Animation hurtLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        hurtLeftAnimation.addFrame(sf::IntRect(1352, 0, 176, 158), sf::Vector2f(0.0f, -10.0f));
        hurtLeftAnimation.addFrame(sf::IntRect(1824, 158, 176, 158), sf::Vector2f(-13.0f, -10.0f));
        hurtLeftAnimation.addFrame(sf::IntRect(1176, 0, 176, 158), sf::Vector2f(0.0f, -10.0f));
        return hurtLeftAnimation;
    }
}

Animation Wizard::buildIdleRightAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation idleRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        idleRightAnimation.addFrame(sf::IntRect(522, 339, 143, 150));
        idleRightAnimation.addFrame(sf::IntRect(665, 339, 143, 148), sf::Vector2f(1.0f, 0.0f));
        idleRightAnimation.addFrame(sf::IntRect(0, 504, 143, 148), sf::Vector2f(0.0f, 1.0f));
        idleRightAnimation.addFrame(sf::IntRect(143, 504, 143, 148), sf::Vector2f(2.0f, 1.0f));
        return idleRightAnimation;
    } else {
        Animation idleRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        idleRightAnimation.addFrame(sf::IntRect(400, 327, 170, 177), sf::Vector2f(-20.0f, -30.0f));
        idleRightAnimation.addFrame(sf::IntRect(740, 327, 170, 177), sf::Vector2f(-12.0f, -27.0f));
        idleRightAnimation.addFrame(sf::IntRect(570, 327, 170, 177), sf::Vector2f(-10.0f, -27.0f));
        idleRightAnimation.addFrame(sf::IntRect(0, 504, 170, 177), sf::Vector2f(-7.0f, -24.0f));
        return idleRightAnimation;
    }
}

Animation Wizard::buildIdleLeftAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation idleLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        idleLeftAnimation.addFrame(sf::IntRect(945, 339, 143, 148));
        idleLeftAnimation.addFrame(sf::IntRect(802, 339, 143, 150), sf::Vector2f(-1.0f, 0.0f));
        idleLeftAnimation.addFrame(sf::IntRect(1467, 504, 143, 148), sf::Vector2f(0.0f, -1.0f));
        idleLeftAnimation.addFrame(sf::IntRect(1324, 504, 143, 148), sf::Vector2f(-2.0f, -1.0f));
        return idleLeftAnimation;
    } else {
        Animation idleLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        idleLeftAnimation.addFrame(sf::IntRect(1430, 327, 170, 177), sf::Vector2f(-5.0f, -30.0f));
        idleLeftAnimation.addFrame(sf::IntRect(1090, 327, 170, 177), sf::Vector2f(-14.0f, -27.0f));
        idleLeftAnimation.addFrame(sf::IntRect(1260, 327, 170, 177), sf::Vector2f(-16.0f, -27.0f));
        idleLeftAnimation.addFrame(sf::IntRect(1830, 504, 170, 177), sf::Vector2f(-19.0f, -24.0f));
        return idleLeftAnimation;
    }
}

Animation Wizard::buildJumpRightAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation jumpRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        jumpRightAnimation.addFrame(sf::IntRect(286, 504, 172, 145));
        jumpRightAnimation.addFrame(sf::IntRect(458, 504, 130, 159));
        jumpRightAnimation.addFrame(sf::IntRect(588, 504, 172, 145));
        return jumpRightAnimation;
    } else {
        Animation jumpRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        jumpRightAnimation.addFrame(sf::IntRect(796, 158, 180, 169), sf::Vector2f(0.0f, -30.0f));
        jumpRightAnimation.addFrame(sf::IntRect(710, 504, 146, 192), sf::Vector2f(-10.0f, -30.0f));
        jumpRightAnimation.addFrame(sf::IntRect(436, 158, 180, 169), sf::Vector2f(0.0f, -30.0f));
        return jumpRightAnimation;
    }
}

Animation Wizard::buildJumpLeftAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation jumpLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        jumpLeftAnimation.addFrame(sf::IntRect(1152, 504, 172, 145), sf::Vector2f(-10.0f, 0.0f));
        jumpLeftAnimation.addFrame(sf::IntRect(1022, 504, 130, 159));
        jumpLeftAnimation.addFrame(sf::IntRect(850, 504, 172, 145), sf::Vector2f(-10.0f, 0.0f));
        return jumpLeftAnimation;
    } else {
        Animation jumpLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        jumpLeftAnimation.addFrame(sf::IntRect(1024, 158, 180, 169), sf::Vector2f(-10.0f, -30.0f));
        jumpLeftAnimation.addFrame(sf::IntRect(1144, 504, 146, 192), sf::Vector2f(-10.0f, -30.0f));
        jumpLeftAnimation.addFrame(sf::IntRect(1384, 158, 180, 169), sf::Vector2f(-10.0f, -30.0f));
        return jumpLeftAnimation;
    }
}

Animation Wizard::buildAttackRightAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation attackRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        attackRightAnimation.addFrame(sf::IntRect(0, 0, 168, 143));
        attackRightAnimation.addFrame(sf::IntRect(168, 0, 258, 163), sf::Vector2f(9.0f, -20.0f));
        attackRightAnimation.addFrame(sf::IntRect(426, 0, 291, 174), sf::Vector2f(11.0f, -33.0f));
        return attackRightAnimation;
    } else {
        Animation attackRightAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        attackRightAnimation.addFrame(sf::IntRect(616, 158, 180, 169), sf::Vector2f(0.0f, -28.0f));
        attackRightAnimation.addFrame(sf::IntRect(0, 327, 250, 170), sf::Vector2f(3.0f, -28.0f));
        attackRightAnimation.addFrame(sf::IntRect(176, 158, 260, 168), sf::Vector2f(6.0f, -28.0f));
        return attackRightAnimation;
    }
}

Animation Wizard::buildAttackLeftAnimation(ResourceLoader* resourceLoader) {
    if (color == Color::PURPLE) {
        Animation attackLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_PURPLE));
        attackLeftAnimation.addFrame(sf::IntRect(1442, 0, 168, 143));
        attackLeftAnimation.addFrame(sf::IntRect(1184, 0, 258, 163), sf::Vector2f(-98.0f, -20.0f));
        attackLeftAnimation.addFrame(sf::IntRect(893, 0, 291, 174), sf::Vector2f(-133.0f, -33.0f));
        return attackLeftAnimation;
    } else {
        Animation attackLeftAnimation(*resourceLoader->getTexture(ResourceLoader::TextureId::WIZARD_ORANGE));
        attackLeftAnimation.addFrame(sf::IntRect(1204, 158, 180, 169), sf::Vector2f(-13.0f, -27.0f));
        attackLeftAnimation.addFrame(sf::IntRect(1750, 327, 250, 170), sf::Vector2f(-86.0f, -27.0f));
        attackLeftAnimation.addFrame(sf::IntRect(1564, 158, 260, 168), sf::Vector2f(-97.0f, -27.0f));
        return attackLeftAnimation;
    }
}
