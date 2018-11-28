#pragma once

namespace SimulationProperties {
    constexpr float TICKS_PER_SECOND = 30.0f;
    constexpr float RUN_VELOCITY = 500.0f;
    constexpr float JUMP_KICKOFF_VELOCITY = -650.0f;
    constexpr float JUMP_VELOCITY = -500.0f;
    constexpr float MAX_AIR_TIME = 0.35f;
    constexpr float JUMP_KICK_OFF_TIME = 0.1f;
    constexpr float GRAVITY = 600.0f;
    constexpr float MAX_X_BOUNDARY = 1780.0f;
    constexpr float MIN_X_BOUNDARY = 0.0f;
    constexpr float MAX_Y_BOUNDARY = 865.0f;
    constexpr float ATTACK_ANIMATION_DURATION = 450.0f;

    enum class Direction {
        RIGHT,
        LEFT
    };
}
