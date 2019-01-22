//------------------------------------------------------------------------------
//
// File Name:	ChipCollectible.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include <Component.h>

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Sprite;

//------------------------------------------------------------------------------

namespace Behaviors
{
	class ChipCollectible : public Component
	{
		
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
	
	public:
		// Constructor
		// Params:
		//   cooldown = The cooldown after collected
		ChipCollectible(float cooldown = 30.0f);
		
		// Clones the component
		Component* Clone() const override;
		// Initializes the component
		void Initialize() override;

		// Updates the component
		void Update(float dt) override;

		// Returns whether the collectible is active
		bool IsActive() const;

		// Set active
		void SetActive(bool active_);

		// A friend function that handles the collisions of this Chip collectible
		friend void ChipCollectibleCollisionHandler(GameObject& object, GameObject& otherObject);
		
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
		
	private:
		Sprite* sprite;

		float cooldown;
		float timer;

		bool active;
	};
}

//------------------------------------------------------------------------------