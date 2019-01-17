//------------------------------------------------------------------------------
//
// File Name:	Hazard.cpp
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

#include "Hazard.h"

// Systems
#include "Engine.h"
#include "GameObject.h"

// Components
#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	void HazardCollisionHandler(GameObject& object, GameObject& other);

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   rotation = rotation of Hazard
	Hazard::Hazard(bool alwaysCollidable, bool collidable, float rotation) : Component("Hazard"), 
		alwaysCollidable(alwaysCollidable), collidable(collidable), rotation(rotation)
	{
	}

	// Return a new copy of the component.
	Component* Hazard::Clone() const
	{
		return new Hazard(*this);
	}

	// Initialize data for this object.
	void Hazard::Initialize()
	{
		// Set the object's collision handler to ours.
		Collider* collider = static_cast<Collider*>(GetOwner()->GetComponent("Collider"));
		collider->SetCollisionHandler(&Behaviors::HazardCollisionHandler);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Hazard::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Returns if the hazard is collidable
	bool Hazard::isCollidable()
	{
		return collidable;
	}

	// Changes whether the hazard is collidable or not
	void Hazard::ChangeCollidablity()
	{
		if (alwaysCollidable)
			return;
		collidable = !collidable;
	}

	// Collision handler for Hazard objects.
	// Params:
	//   object = The first object.
	//   other  = The other object the first object is colliding with.
	void HazardCollisionHandler(GameObject& object, GameObject& other)
	{
		if (other.GetName() == "Player")
		{
			if (static_cast<Hazard*>(object.GetComponent("Hazard"))->collidable)
				Engine::GetInstance().Stop();
		}
	}
}