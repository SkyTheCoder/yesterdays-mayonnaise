//------------------------------------------------------------------------------
//
// File Name:	PlayerMovement.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlayerMovement.h"

// Systems
#include "GameObject.h"
#include <Input.h>
#include "Space.h"
#include <Interpolation.h>

// Components
#include "Transform.h"
#include "Physics.h"
#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Map collision handler for Monkey objects.
	// Params:
	//   object = The monkey object.
	//   collision = Which sides the monkey collided on.
	void MonkeyMapCollisionHandler(GameObject& object, const MapCollision& collision)
	{
		// Get the PlayerMovement component.
		PlayerMovement* monkeyMovement = static_cast<PlayerMovement*>(object.GetComponent("PlayerMovement"));

		// If the monkey's collider is colliding on the bottom, mark the monkey as on ground.
		if (collision.bottom)
		{
			monkeyMovement->onGround = true;
		}

		// Save whether the monkey is touching a wall, used for wall jumping.

		if (collision.left)
		{
			monkeyMovement->onLeftWall = true;
		}

		if (collision.right)
		{
			monkeyMovement->onRightWall = true;
		}
	}

	// Collision handler for monkey.
	// Params:
	//   object = The monkey.
	//   other  = The object the monkey is colliding with.
	void MonkeyCollisionHandler(GameObject& object, GameObject& other)
	{
		// Destroy collectibles when touching.
		if (other.GetName() == "Collectible")
		{
			other.Destroy();
		}

		if (other.GetName() == "JumpBoost" || other.GetName() == "SpeedBoost")
		{
			Behaviors::PlayerMovement* playerMovement = static_cast<Behaviors::PlayerMovement*>(object.GetComponent("PlayerMovement"));
			if (other.GetName() == "JumpBoost")
				playerMovement->SetPowerUp(POWER_UP_JUMP);
			else if (other.GetName() == "SpeedBoost")
				playerMovement->SetPowerUp(POWER_UP_SPEED);

			playerMovement->StartPUTimer();

			other.Destroy();
		}

		// Restart the level when touching hazards or enemies.
		if (other.GetName() == "Hazard" || other.GetName() == "Enemy")
		{
			object.Destroy();
		}
	}

	// Constructor
	PlayerMovement::PlayerMovement(unsigned keyUp, unsigned keyLeft, unsigned keyRight) : Component("PlayerMovement"),
		keyUp(keyUp), keyLeft(keyLeft), keyRight(keyRight),
		monkeyWalkSpeedOG(350.0f), jumpSpeedOG(0.0f, 850.0f), slidingJumpSpeed(600.0f, 675.0f),
		monkeyWalkSpeed(monkeyWalkSpeedOG), jumpSpeed(jumpSpeedOG),
		gravity(0.0f, -1200.0f), slidingGravity(0.0f, -600.0f), terminalVelocity(700.0f), slidingTerminalVelocity(150.0f), gracePeriod(0.15f),
		transform(nullptr), physics(nullptr),
		playerID(0),
		onGround(false), onLeftWall(false), onRightWall(false),
		hasJumped(false), airTime(0.0f), leftTime(0.0f), rightTime(0.0f), movementLerpGround(0.95f), movementLerpAir(0.8f),
		powerUp(POWER_UP_NONE), PUTimer(0.0f), PUMaxTime(5.0f), jumpBoost(0.0f, 950.0f), speedBoost(425.0f)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerMovement::Clone() const
	{
		return new PlayerMovement(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerMovement::Initialize()
	{
		// Store the required components for ease of access.
		transform = static_cast<Transform*>(GetOwner()->GetComponent("Transform"));
		physics = static_cast<Physics*>(GetOwner()->GetComponent("Physics"));

		Collider* collider = static_cast<Collider*>(GetOwner()->GetComponent("Collider"));

		// Set the map collision handler.
		collider->SetMapCollisionHandler(MonkeyMapCollisionHandler);

		// Set the collision handler.
		collider->SetCollisionHandler(MonkeyCollisionHandler);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerMovement::Update(float dt)
	{
		// Check if the player has an active powerUp
		if (PUTimer -= dt <= 0.0f)
		{
			// Set the PUTimer to 0
			PUTimer = 0.0f;
			// Set the current powerUp to Nothing
			SetPowerUp();
		}

		// Handle horizontal movement.
		MoveHorizontal(dt);

		// Handle vertical movement.
		MoveVertical(dt);
	}

	// Sets the keybinds for the monkey.
	// Params:
	//   keyUp = The up keybind.
	//   keyLeft = The left keybind.
	//   keyRight = The right keybind.
	void PlayerMovement::SetKeybinds(unsigned keyUp_, unsigned keyLeft_, unsigned keyRight_)
	{
		keyUp = keyUp_;
		keyLeft = keyLeft_;
		keyRight = keyRight_;
	}

	// Sets the player's ID.
	// Params:
	//   newID = The ID to set to.
	void PlayerMovement::SetID(int newID)
	{
		playerID = newID;
	}

	// Sets the player's ID.
	// Returns:
	//   The player's ID.
	int PlayerMovement::GetID() const
	{
		return playerID;
	}

	// Returns current powerUp
	PowerUp PlayerMovement::GetPowerUp()
	{
		return powerUp;
	}

	// Sets current powerUp
	// Default is no powerUp
	void PlayerMovement::SetPowerUp(PowerUp newPowerUp)
	{
		powerUp = newPowerUp;

		if (powerUp == POWER_UP_JUMP)
			jumpSpeed = jumpBoost;
		else if (powerUp == POWER_UP_SPEED)
			monkeyWalkSpeed = speedBoost;
		else
		{
			jumpSpeed = jumpSpeedOG;
			monkeyWalkSpeed = monkeyWalkSpeedOG;
		}
	}

	// Starts PowerUp Timer
	void PlayerMovement::StartPUTimer()
	{
		PUTimer = PUMaxTime;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Moves horizontally based on input
	void PlayerMovement::MoveHorizontal(float dt) const
	{
		Input& input = Input::GetInstance();

		Vector2D velocity = physics->GetVelocity();

		// Initialize target velocity to 0.
		float targetVelocityX = 0.0f;

		// If the right arrow key is pressed, move to the right.
		if (input.CheckHeld(keyRight))
		{
			targetVelocityX += monkeyWalkSpeed;
		}

		// If the right arrow key is pressed, move to the left.
		if (input.CheckHeld(keyLeft))
		{
			targetVelocityX -= monkeyWalkSpeed;
		}

		// Smoothly interpolate the X component of the player's velocity.
		float movementMix = 1.0f - pow(1.0f - (airTime <= gracePeriod ? movementLerpGround : movementLerpAir), dt);
		velocity.x = Interpolate(velocity.x, targetVelocityX, movementMix);

		// Set the velocity.
		physics->SetVelocity(velocity);
	}

	// Moves vertically based on input
	void PlayerMovement::MoveVertical(float dt)
	{
		Input& input = Input::GetInstance();

		Vector2D velocity = physics->GetVelocity();

		// Reset time since touching walls.
		if (onLeftWall && !onRightWall)
			leftTime = 0.0f;
		else
			leftTime += dt;

		if (onRightWall && !onLeftWall)
			rightTime = 0.0f;
		else
			rightTime += dt;

		bool onlyLeftWall = leftTime <= gracePeriod;
		bool onlyRightWall = rightTime <= gracePeriod;
		bool isSliding = (onlyLeftWall || onlyRightWall) && !onGround;

		if (onGround || (onLeftWall && !onRightWall) || (onRightWall && !onLeftWall))
		{
			// Reset time spent in the air.
			airTime = 0.0f;

			// The player has not jumped since leaving the ground.
			hasJumped = false;
		}
		else
		{
			// Keep track of time spent in the air.
			airTime += dt;
		}

		bool canJump = airTime <= gracePeriod || onlyLeftWall || onlyRightWall;

		// If the monkey has not jumped since landing, was on the ground recently, and the up arrow key is pressed, jump.
		if (!hasJumped && canJump && input.CheckHeld(keyUp))
		{
			if (isSliding)
			{
				// Increase Y velocity.
				velocity.y = slidingJumpSpeed.y;

				if (onlyLeftWall)
				{
					velocity.x = slidingJumpSpeed.x;
				}
				else if (onlyRightWall)
				{
					velocity.x = -slidingJumpSpeed.x;
				}
			}
			else
			{
				// Increase Y velocity.
				velocity.y = jumpSpeed.y;
			}

			hasJumped = true;
		}
		
		// Apply gravity if in air.
		if (!onGround)
		{
			if (isSliding)
			{
				if (velocity.y > 0.0f)
					physics->AddForce(gravity);
				else
					physics->AddForce(slidingGravity);
			}
			else
			{
				physics->AddForce(gravity);
			}
		}

		// Clamp velocity.
		// Use different terminal velocity depending on whether the monkey is sliding.
		if (isSliding)
			velocity.y = max(-slidingTerminalVelocity, velocity.y);
		else
			velocity.y = max(-terminalVelocity, velocity.y);

		// Set the velocity.
		physics->SetVelocity(velocity);

		onGround = false;
		onLeftWall = false;
		onRightWall = false;
	}
}

//------------------------------------------------------------------------------