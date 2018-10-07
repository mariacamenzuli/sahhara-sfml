#pragma once

class GameSceneDirector {
public:
	enum class SceneId {
		MAIN_MENU, BATTLE
	};

	virtual ~GameSceneDirector() = default;

	virtual void initiateScene(const SceneId sceneId) = 0;
	virtual void concludeCurrentScene() = 0;
};