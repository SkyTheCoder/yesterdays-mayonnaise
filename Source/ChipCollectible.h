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
		//   type: The Chip's type
		ChipCollectible();
		
		// Clones the componnet
		Component* Clone() const override;
		// Initializes the component
		void Initialize() override;

		// Updates the component
		void Update(float dt) override;

		// A friend function that handles the collisions of this Chip collectible
		friend void ChipCollectibleCollisionHandler(GameObject& object, GameObject& otherObject);
		
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
		
	private:
		Transform* transform;
		Sprite* sprite;
	};
}

//------------------------------------------------------------------------------