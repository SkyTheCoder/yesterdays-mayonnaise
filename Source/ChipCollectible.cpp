//------------------------------------------------------------------------------
//
// File Name:	ChipCollectible.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChipCollectible.h"

#include <Transform.h>						// Transform
#include <Sprite.h>							// Sprite
#include <Collider.h>						// Collider
#include <GameObject.h>						// Game Object

//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Deals with chip collision
	void ChipCollectibleCollisionHandler(GameObject& collectible, GameObject& other)
	{
		UNREFERENCED_PARAMETER(collectible);
		UNREFERENCED_PARAMETER(other);
	}

	// Constructor
	ChipCollectible::ChipCollectible(float cooldown)
		: Component("ChipCollectible"), sprite(nullptr), cooldown(cooldown), timer(0.0f), active(true)
	{
	}

	// Clones the component
	Component* ChipCollectible::Clone() const
	{
		return new ChipCollectible(*this);
	}

	// Initialize
	void ChipCollectible::Initialize()
	{
		sprite = static_cast<Sprite*>(GetOwner()->GetComponent("Sprite"));
		
		// Set the collision handler
		Collider* ChipCollider = static_cast<Collider*>(GetOwner()->GetComponent("Collider"));
		ChipCollider->SetCollisionHandler(&ChipCollectibleCollisionHandler);
	}

	// Update
	void ChipCollectible::Update(float dt)
	{
		timer += dt;
		
		// set active after cooldown
		if (timer >= cooldown)
		{
			timer = 0;
			active = true;
		}

		// Make transparent if inactive
		if (active)
			sprite->SetAlpha(1.0f);
		else
			sprite->SetAlpha(0.3f);
	}

	// Returns whether the collectible is active
	bool ChipCollectible::IsActive() const
	{
		return active;
	}

	// Set active
	void ChipCollectible::SetActive(bool active_)
	{
		active = active_;
	}
}

//------------------------------------------------------------------------------