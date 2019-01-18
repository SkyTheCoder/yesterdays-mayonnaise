//------------------------------------------------------------------------------
//
// File Name:	PlayerMovement.h
// Author(s):	David Cohen (david.cohen)
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

#include "Vector2D.h" // Vector2D

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Physics;
struct MapCollision;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class MonkeyAnimation;

	class PlayerMovement : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerMovement(unsigned keyUp, unsigned keyLeft, unsigned keyRight);

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Sets the keybinds for the monkey.
		// Params:
		//   keyUp = The up keybind.
		//   keyLeft = The left keybind.
		//   keyRight = The right keybind.
		void SetKeybinds(unsigned keyUp, unsigned keyLeft, unsigned keyRight);

		// Sets the player's ID.
		// Params:
		//   newID = The ID to set to.
		void SetID(int newID);

		// Sets the player's ID.
		// Returns:
		//   The player's ID.
		int GetID() const;

		// Map collision handler for Monkey objects.
		// Params:
		//   object = The monkey object.
		//   collision = Which sides the monkey collided on.
		friend void MonkeyMapCollisionHandler(GameObject& object, 
			const MapCollision& collision);

		// Collision handler for monkey.
		// Params:
		//   object = The monkey.
		//   other  = The object the monkey is colliding with.
		friend void MonkeyCollisionHandler(GameObject& object, GameObject& other);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Moves horizontally based on input
		void MoveHorizontal(float dt) const;

		// Moves vertically based on input
		void MoveVertical(float dt);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Keybinds
		unsigned keyUp;
		unsigned keyLeft;
		unsigned keyRight;

		// Movement properties
		float monkeyWalkSpeed;
		Vector2D jumpSpeed;
		Vector2D slidingJumpSpeed;
		Vector2D gravity;
		Vector2D slidingGravity;
		float terminalVelocity;
		float slidingTerminalVelocity;
		float gracePeriod;

		// Components
		Transform* transform;
		Physics* physics;

		// Misc
		int playerID;

		bool onGround;
		bool onLeftWall;
		bool onRightWall;
		bool hasJumped;
		float airTime;
		float leftTime;
		float rightTime;
		float movementLerpGround;
		float movementLerpAir;

		friend class MonkeyAnimation;
	};
}

//------------------------------------------------------------------------------
