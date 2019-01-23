//------------------------------------------------------------------------------
//
// File Name:	MonkeyAnimation.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MonkeyAnimation.h"

// Systems
#include "GameObject.h"

// Components
#include "Animation.h"
#include "Physics.h"
#include "Transform.h"
#include "PlayerMovement.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//	 walkStart  = The starting frame for the walk animation.
	//   walkLength = The number of frames of the walk animation.
	//   jumpStart  = The starting frame for the jump animation.
	//   jumpLength = The number of frames of the jump animation.
	//   idleStart  = The starting frame for the idle animation.
	//   idleLength = The number of frames of the idle animation.
	MonkeyAnimation::MonkeyAnimation(unsigned walkStart, unsigned walkLength,
		unsigned jumpStart, unsigned jumpLength, unsigned idleStart, unsigned idleLength) : Component("MonkeyAnimation"), walkStart(walkStart), walkLength(walkLength), jumpStart(jumpStart), jumpLength(jumpLength), idleStart(idleStart), idleLength(idleLength), currentState(StateIdle), nextState(StateIdle)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* MonkeyAnimation::Clone() const
	{
		return new MonkeyAnimation(*this);
	}

	// Initialize this component (happens at object creation).
	void MonkeyAnimation::Initialize()
	{
		// Store the required components for ease of access.
		animation = static_cast<Animation*>(GetOwner()->GetComponent("Animation"));
		physics = static_cast<Physics*>(GetOwner()->GetComponent("Physics"));
		transform = static_cast<Transform*>(GetOwner()->GetComponent("Transform"));
		monkeyMovement = static_cast<PlayerMovement*>(GetOwner()->GetComponent("PlayerMovement"));

		// Play the idle animation.
		animation->Play(idleStart, 1, 0.0f, true);

		// Backup the original scale.
		originalScale = transform->GetScale();
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void MonkeyAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Choose the next state.
		ChooseNextState();

		// Update the current state if necessary.
		ChangeCurrentState();

		// Flip the sprite if necessary.
		FlipSprite();
	}

	// Sets the frames for the animation.
	// Params:
	//	 walkStart  = The starting frame for the walk animation.
	//   walkLength = The number of frames of the walk animation.
	//   jumpStart  = The starting frame for the jump animation.
	//   jumpLength = The number of frames of the jump animation.
	//   idleStart  = The starting frame for the idle animation.
	void MonkeyAnimation::SetFrames(unsigned walkStart_, unsigned walkLength_,
		unsigned jumpStart_, unsigned jumpLength_, unsigned idleStart_, unsigned idleLength_)
	{

		walkStart = walkStart_;
		walkLength = walkLength_;
		jumpStart = jumpStart_;
		jumpLength = jumpLength_;
		idleStart = idleStart_;
		idleLength = idleLength_;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Choose the correct state based on velocity.
	void MonkeyAnimation::ChooseNextState()
	{
		Vector2D velocity = physics->GetVelocity();
		// If we are jumping/falling, use the jumping state.
		if (monkeyMovement->airTime > 0.1f)
		{
			nextState = State::StateJump;
		}
		// If we are moving to the side, use the walking state.
		else if (abs(velocity.x) > 75.0f)
		{
			nextState = State::StateWalk;
		}
		// If we are standing still, use the idle state.
		else
		{
			nextState = State::StateIdle;
		}
	}

	// Change states and start the appropriate animation.
	void MonkeyAnimation::ChangeCurrentState()
	{
		// Check if the state should change.
		if (currentState != nextState)
		{
			currentState = nextState;
			switch (currentState)
			{
			// If the state is changed to the idle state, begin playing the idle animation.
			case State::StateIdle:
				animation->Play(idleStart, idleLength, 0.2f, true);
				break;
			// If the state is changed to the walking state, begin playing the walking animation.
			case State::StateWalk:
				animation->Play(walkStart, walkLength, 0.1f, true);
				break;
			// If the state is changed to the jumping state, begin playing the jumping animation.
			case State::StateJump:
				animation->Play(jumpStart, jumpLength, 0.2f, true);
				break;
			}
		}
	}

	// Flip the sprite based on velocity and current state.
	void MonkeyAnimation::FlipSprite() const
	{
		// Check if we are walking or jumping.
		if (currentState != State::StateIdle)
		{
			Vector2D velocity = physics->GetVelocity();
			// If we are moving left, flip the sprite on the X axis.
			if (velocity.x < 0.0f)
			{
				transform->SetScale(Vector2D(-originalScale.x, originalScale.y));
			}
			// If we are moving right, restore the original scale.
			else if (velocity.x > 0.0f)
			{
				transform->SetScale(originalScale);
			}
		}
	}
}

//------------------------------------------------------------------------------
