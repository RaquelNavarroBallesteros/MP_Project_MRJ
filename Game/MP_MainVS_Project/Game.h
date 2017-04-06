#pragma once
#include "Player.h"
#include "GameUtils.h"
#include "Scene.h"

const Vector2 SCREEN_LENGHT = { 640, 480 };
const Vector2 SCREEN_START = { 0, 0 };
const int BLOCKS_WIDTH = 64;

class Game
{
	private:
		static const int levelsLenght = 3;
		const int groundY = SCREEN_LENGHT.y - BLOCKS_WIDTH * 1;
		const Vector2 spawnPosition = { (float)BLOCKS_WIDTH * 2, (float)BLOCKS_WIDTH * 3.17f };
		const Vector2 friction = { 0.0005f, 0.0005f };

		int currentLevel;
		Player player;
		//Scene scenes[levelsLenght];

	public:
		Game();
		~Game();
		void Initialize(int level);
		void Update();
		void Draw();
};

