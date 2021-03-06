#include "Player.h"
#include <stdio.h>

void Player::DefineCurrentAnimation(PlayerAnimations animation)
{
	if (currentAnimation != animation)
	{
		currentAnimation = animation;
		animator.Reset();
	}
}

void Player::SetCurrentAnimation()
{
	animator.SetCurrentSequence(currentAnimation);
}

Player::Player()
{
}

Player::Player(int index)
{
	SetPosition({ (float)PLAYER_SOURCEPOSITION_X[index], GROUND_Y });
	SetRotation(0);
	SetScale(1);

	direction = { 0, 0 };
	velocity = { 0, 0 };
	isGrounded = false;
	isJumping = false;
	isAlive = true;

	animator.New(al_load_bitmap(PLAYER_SPRITE[index]), 3);
	animator.NewSequence({ 4, PLAYER_WIDTH, PLAYER_HEIGHT, 10,{ -PLAYER_WIDTH / 2, -PLAYER_HEIGHT }, true }, RUNNING);
	animator.NewSequence({ 3, PLAYER_WIDTH, PLAYER_HEIGHT, 10,{ -PLAYER_WIDTH / 2, -PLAYER_HEIGHT }, false }, JUMPING);
	animator.NewSequence({ 3, PLAYER_WIDTH, PLAYER_HEIGHT, 10,{ -PLAYER_WIDTH / 2, -PLAYER_HEIGHT }, true }, IDLE);

	arrow.New(al_load_bitmap(ARROW_SPRITE));

	DefineCurrentAnimation(IDLE);

	collider.New(GetPosition(), { 0, -PLAYER_HEIGHT / 2 + PLAYER_HEIGHT / 2 - PLAYER_COLLIDER_HEIGHT / 2 }, { PLAYER_COLLIDER_WIDTH, PLAYER_COLLIDER_HEIGHT });
}

Player::~Player()
{
	collider.~AABB();
	animator.~MultiAnimation2D();
	arrow.~Arrow();
}

void Player::New(int index)
{
	SetPosition({ (float)PLAYER_SOURCEPOSITION_X[index], GROUND_Y });
	SetRotation(0);
	SetScale(1);

	direction = { 0, 0 };
	velocity = { 0, 0 };
	isGrounded = false;
	isJumping = false;
	isAlive = true;

	animator.New(al_load_bitmap(PLAYER_SPRITE[index]), 3);
	animator.NewSequence({ 4, PLAYER_WIDTH, PLAYER_HEIGHT, 10, { -PLAYER_WIDTH / 2, -PLAYER_HEIGHT }, true }, RUNNING);
	animator.NewSequence({ 3, PLAYER_WIDTH, PLAYER_HEIGHT, 10, { -PLAYER_WIDTH / 2, -PLAYER_HEIGHT }, false }, JUMPING);
	animator.NewSequence({ 3, PLAYER_WIDTH, PLAYER_HEIGHT, 10, { -PLAYER_WIDTH / 2, -PLAYER_HEIGHT }, true}, IDLE);

	arrow.New(al_load_bitmap(ARROW_SPRITE));

	DefineCurrentAnimation(IDLE);

	collider.New(GetPosition(), { 0, -PLAYER_HEIGHT/2 + PLAYER_HEIGHT/2 - PLAYER_COLLIDER_HEIGHT/2 }, { PLAYER_COLLIDER_WIDTH, PLAYER_COLLIDER_HEIGHT });
}

void Player::Update()
{
	if (IsActive())
	{
		isGrounded = false;
		direction.x = 0;

		if (GetPosition().y >= GROUND_Y)
		{
			isGrounded = true;
			isJumping = false;
			velocity.y = 0;
			SetPosition({ GetPosition().x, GROUND_Y });
		}

		// JUMP
		if (isGrounded && !isJumping && input.IsKeyPressed(PLAYER_JUMP[index]))
		{
			isJumping = true;
			velocity.y = -PLAYER_JUMPSPEED;
			DefineCurrentAnimation(JUMPING);
		}
		else if (!isGrounded)
		{
			// FALLING
			velocity.y += GRAVITY_Y;
			DefineCurrentAnimation(JUMPING);
		}

		// MOVEMENT
		if (input.IsKeyDown(PLAYER_RIGHT[index]))
		{
			direction.x = 1;
			orientation = RIGHT;
		}
		else if (input.IsKeyDown(PLAYER_LEFT[index]))
		{
			direction.x = -1;
			orientation = LEFT;
		}

		if (input.IsKeyPressed(PLAYER_SHOT[index]) && !arrow.IsActive())
		{
			arrow.Shot(GetPosition());
		}

		arrow.Update();

		
		/*
		// Direction.y in case of stairs or some (not jump) vertical movement
		if (input.IsKeyDown(PLAYER_1_UP))
		{
			direction.y = -1;
		}
		else if (input.IsKeyDown(PLAYER_1_DOWN))
		{
			direction.y = 1;
		}
		*/
		

		/*
		if (!isJumping && !isGrounded)
		{
			// falling
			velocity.x = direction.x * speed.x * fallingMovementFactor;
		}
		else if (isJumping)
		{
			// grounded or jumping
			velocity.x = direction.x * speed.x * jumpingMovementFactor;
		}
		else
		{
			velocity.x = direction.x * speed.x;
		}
		*/

		if (isGrounded)
		{
			if (direction.x == 0)
			{
				if (velocity.x > 0)
				{
					velocity.x -= GROUND_FRICTION_X;
					if (velocity.x < 0) velocity.x = 0;
				}
				else if (velocity.x < 0)
				{
					velocity.x += GROUND_FRICTION_X;
					if (velocity.x > 0) velocity.x = 0;
				}

				DefineCurrentAnimation(IDLE);
			}
			else
			{
				// Speed should have an easing dependant value!
				velocity.x += PLAYER_SPEED.x * direction.x;
				if (velocity.x > PLAYER_TOPSPEED_X[index]) velocity.x = PLAYER_TOPSPEED_X[index];
				else if (velocity.x < -PLAYER_TOPSPEED_X[index]) velocity.x = -PLAYER_TOPSPEED_X[index];

				DefineCurrentAnimation(RUNNING);
			}
		}

		// SCREEN BOUNDS COLLISION
		if (GetPosition().x - collider.GetLenght().x / 2 <= 0)
		{
			if (velocity.x < 0) velocity.x = 0;
			SetPosition({ 0 + collider.GetLenght().x / 2, GetPosition().y });
		}
		else if (GetPosition().x + collider.GetLenght().x / 2 >= SCREEN_WIDTH)
		{
			if (velocity.x > 0) velocity.x = 0;
			SetPosition({ SCREEN_WIDTH - collider.GetLenght().x / 2, GetPosition().y });
		}

		Translate(velocity);
		collider.UpdatePosition(GetPosition());
		SetCurrentAnimation();
		animator.Update();

		//printf("P_v: %f\n", velocity.x);
		//printf("Anim: %i\n", (int)currentAnimation);
	}
}

void Player::Draw()
{
	if (IsDrawable())
	{
		arrow.Draw();
		animator.Draw(GetPosition(), (bool)orientation);

		#ifdef DEBUG	
		collider.DebugDraw();
		#endif
	}
}

void Player::CheckBubblesCollision(Bubble * b, int lenght)
{
	int i = 0;
	while (isAlive && i < lenght)
	{
		if (b[i].IsActive() && collider.CheckAABBCircleCollision(b[i].GetCollider()))
		{
			isAlive = false;
			arrow.Restart();
			// TEMP
			SetActive(false);
			SetDrawable(false);
			// Set death anim.
		}
		i++;
	}

	arrow.CheckBubblesCollision(b, lenght);
}

bool Player::IsAlive()
{
	return isAlive;
}

void Player::SetAsAlive()
{
	isAlive = true;
	SetPosition({ (float)PLAYER_SOURCEPOSITION_X[0], (float)GROUND_Y });
	collider.UpdatePosition(GetPosition());
	DefineCurrentAnimation(IDLE);
	SetCurrentAnimation();
	animator.Reset();
	Activate();
}
