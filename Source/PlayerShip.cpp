//------------------------------------------------------------------------------
//
// File Name:	PlayerShip.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlayerShip.h"

// Systems
#include "GameObject.h"
#include "Space.h"
#include "GameObjectManager.h"
#include <Input.h>
#include <Parser.h>
#include <Engine.h>
#include "SoundManager.h"
#include <fmod_studio.h>

// Components
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Non-default constructor
	// Params:
	//   forwardThrust = Amount of force added to the ship when thrusters are activated.
	//   maximumSpeed  = Maximum attainable speed of the ship.
	//   rotationSpeed = Speed at which the ship rotates.
	//   bulletSpeed   = Speed at which bullets move when fired by ship.
	PlayerShip::PlayerShip(float forwardThrust, float maximumSpeed, float rotationSpeed,
		float bulletSpeed) : Component("PlayerShip"), forwardThrust(forwardThrust), maximumSpeed(maximumSpeed), rotationSpeed(rotationSpeed), bulletSpeed(bulletSpeed), bulletArchetype(nullptr), transform(nullptr), physics(nullptr)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerShip::Clone() const
	{
		return new PlayerShip(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerShip::Initialize()
	{
		// Store the required components for ease of access.
		transform = static_cast<Transform*>(GetOwner()->GetComponent("Transform"));
		physics = static_cast<Physics*>(GetOwner()->GetComponent("Physics"));

		GameObject* owner = GetOwner();
		Space* space = owner->GetSpace();
		GameObjectManager& objectManager = space->GetObjectManager();

		// Store the required archetypes for ease of access.
		bulletArchetype = objectManager.GetArchetypeByName("Bullet");

		// Create the thrust sound effect.
		soundEvent = Engine::GetInstance().GetModule<SoundManager>()->PlayEvent("Test Tones");

		// Configure the thrust sound effect.
		soundEvent->setPaused(true);
		soundEvent->setVolume(0.2f);
		soundEvent->setParameterValue("Wave Type", 1.0f);
		soundEvent->setParameterValue("LowMidHigh", 0.0f);

		// Start the event instance, and attempt to release it immediately afterwards.
		// The event will be automatically released when it is done playing.
		soundEvent->start();
		soundEvent->release();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerShip::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		
		// Handle movement (forward thrust).
		Move();

		// Handle rotation.
		Rotate();

		// Handle shooting.
		Shoot();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Move forward when up arrow is pressed
	void PlayerShip::Move() const
	{
		Input& input = Input::GetInstance();

		// Check if the up arrow key is pressed.
		if (input.CheckHeld(VK_UP))
		{
			// Add force in the direction the ship is facing.
			physics->AddForce(Vector2D::FromAngleRadians(transform->GetRotation()) * forwardThrust);

			// Clamp velocity to the maximum speed.
			if (physics->GetVelocity().Magnitude() > maximumSpeed)
			{
				physics->SetVelocity(physics->GetVelocity().Normalized() * maximumSpeed);
			}

			// Play the thrust sound effect.
			soundEvent->setPaused(false);
		}
		else
		{
			// Resume the thrust sound effect.
			soundEvent->setPaused(true);
		}
	}

	// Rotate when left or right arrow key is pressed
	void PlayerShip::Rotate() const
	{
		Input& input = Input::GetInstance();

		float angularVelocity = 0.0f;

		// If the right arrow key is pressed, turn right.
		if (input.CheckHeld(VK_RIGHT))
		{
			angularVelocity -= rotationSpeed;
		}

		// If the left arrow key is pressed, turn left.
		if (input.CheckHeld(VK_LEFT))
		{
			angularVelocity += rotationSpeed;
		}
		
		// Set the angular velocity.
		physics->SetAngularVelocity(angularVelocity);
	}

	// Shoot projectiles when space is pressed
	void PlayerShip::Shoot() const
	{
		Input& input = Input::GetInstance();

		// Check if the space key is pressed.
		if (input.CheckTriggered(VK_SPACE))
		{
			// Create a new instance of the bullet archetype.
			GameObject* bullet = new GameObject(*bulletArchetype);

			// Calculate the direction the ship the ship is facing.
			Vector2D direction = Vector2D::FromAngleRadians(transform->GetRotation());
			
			Transform* bulletTransform = static_cast<Transform*>(bullet->GetComponent("Transform"));

			// Place the bullet in front of the ship.
			bulletTransform->SetTranslation(transform->GetTranslation() + direction * 32.0f);
			bulletTransform->SetRotation(transform->GetRotation());

			// Set the bullet's velocity.
			static_cast<Physics*>(bullet->GetComponent("Physics"))->SetVelocity(direction * bulletSpeed);

			// Add the bullet to the object manager.
			GetOwner()->GetSpace()->GetObjectManager().AddObject(*bullet);
		}
	}
}

//------------------------------------------------------------------------------
