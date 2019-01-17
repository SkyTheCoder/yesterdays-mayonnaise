//------------------------------------------------------------------------------
//
// File Name:	Hazard.h
// Author(s):	A.J. Bussman
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

#include "Component.h" // base class
#include <Color.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class Hazard : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   rotation = rotation of Hazard
		Hazard(bool alwaysCollidable, bool collidable, float rotation = 0.0f);

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Returns if the hazard is collidable
		bool isCollidable();

		// Changes whether the hazard is collidable or not
		void ChangeCollidablity();

		// Collision handler for Hazard objects.
		// Params:
		//   object = The first object.
		//   other  = The other object the first object is colliding with.
		friend void HazardCollisionHandler(GameObject& object, GameObject& other);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties
		float rotation;
		bool alwaysCollidable;
		bool collidable;
	};
}