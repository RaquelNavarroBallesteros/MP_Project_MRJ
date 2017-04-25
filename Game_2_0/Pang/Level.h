#pragma once

#include "BubbleManager.h"
#include "Background.h"

class Level
{
private:
	int level;
	Background background;
	BubbleManager * bubbleManagers;

public:
	Level();
	~Level();

	void New(int level);
};

