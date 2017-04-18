#include "Bubble.h"



void Bubble::Move()
{
	
	if (currentAnimation == BUBBLEANIM_GROUND_COLLISION && animator.IsCurrentSequenceFinished());
	{
		DefineCurrentAnimation(BUBBLEANIM_IDLE);
	}
	

	if (GetPosition().y + collider.GetRadius() >= GROUND_Y)
	{
		velocity.y = -BUBBLES_SPEED[category].y;
		SetPosition({ GetPosition().x, GROUND_Y - collider.GetRadius()});
		DefineCurrentAnimation(BUBBLEANIM_GROUND_COLLISION);
	}

	if (GetPosition().x + collider.GetRadius() >= SCREEN_WIDTH)
	{
		directionX = -1;
		SetPosition({ SCREEN_WIDTH - collider.GetRadius(), GetPosition().y });
	}
	else if (GetPosition().x - collider.GetRadius() <= 0)
	{
		directionX = 1;
		SetPosition({ SCREEN_WIDTH + collider.GetRadius(), GetPosition().y });
	}

	velocity.x = BUBBLES_SPEED[category].x * directionX;
	velocity.y += GRAVITY_Y;

	animator.SetCurrentSequence(currentAnimation);
	Translate(velocity);
}

void Bubble::DefineCurrentAnimation(BubbleAnimations animation)
{
	if (currentAnimation != animation)
	{
		currentAnimation = animation;
		animator.Reset();
	}
}

Bubble::Bubble()
{
}


Bubble::~Bubble()
{
}

void Bubble::New(Vector2 position, int category, int directionX, ALLEGRO_BITMAP * spriteSheet)
{
	SetPosition(position);
	this->category = category;
	velocity = { 0, 0 };
	this->directionX = directionX;

	animator.New(spriteSheet, BUBBLES_SEQUENCES_LENGHT);
	animator.NewSequence({ BUBBLES_SEQUENCE_FRAMES, BUBBLES_RADIUS[category]*2, BUBBLES_RADIUS[category]*2, BUBBLES_SEQUENCE_DURATION, 
	{(float)-BUBBLES_RADIUS[category] / 2, (float)-BUBBLES_RADIUS[category] / 2 }, true }, 0);
	animator.NewSequence({ BUBBLES_SEQUENCE_FRAMES, BUBBLES_RADIUS[category]*2, BUBBLES_RADIUS[category]*2, BUBBLES_SEQUENCE_DURATION, 
	{(float)-BUBBLES_RADIUS[category] / 2, (float)-BUBBLES_RADIUS[category] / 2 }, false }, 1);

	collider.New(GetPosition(), BUBBLES_RADIUS[category]);
}

void Bubble::Activate()
{
	SetActive(true);
	SetDrawable(true);
}

void Bubble::Deactivate()
{
	SetActive(false);
	SetDrawable(false);
}

void Bubble::Update()
{
	if (IsActive())
	{
		Move();
		collider.UpdatePosition(GetPosition());
		animator.Update();
	}
}

void Bubble::Draw()
{
	if (IsDrawable())
	{
		animator.Draw(GetPosition(), false);
		collider.DebugDraw();
	}
}