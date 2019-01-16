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

	// Constructor
	ChipCollectible::ChipCollectible()
		: Component("ChipCollectible")
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
		transform = static_cast<Transform*>(GetOwner()->GetComponent("Transform"));
		sprite = static_cast<Sprite*>(GetOwner()->GetComponent("Sprite"));
		
		// Set the collision handler
		Collider* ChipCollider = static_cast<Collider*>(GetOwner()->GetComponent("Collider"));
		ChipCollider->SetCollisionHandler(&ChipCollectibleCollisionHandler);
	}

	// Update
	void ChipCollectible::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Deals with chip collision
	void ChipCollectibleCollisionHandler(GameObject& collectible, GameObject& player)
	{
		UNREFERENCED_PARAMETER(player);
		UNREFERENCED_PARAMETER(collectible);

		std::cout << "Chip Collected" << std::endl;
	}
}

//------------------------------------------------------------------------------