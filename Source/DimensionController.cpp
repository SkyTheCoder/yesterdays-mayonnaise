//------------------------------------------------------------------------------
//
// File Name:	DimensionController.cpp
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

#include "DimensionController.h"

// Systems
#include <GameObject.h>
#include <Input.h>
#include <Space.h>
#include <Interpolation.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <ColliderTilemap.h>

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
	DimensionController::DimensionController() : Component("DimensionController"),
		dimensions(std::vector<Dimension>())
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* DimensionController::Clone() const
	{
		return new DimensionController(*this);
	}

	// Initialize this component (happens at object creation).
	void DimensionController::Initialize()
	{
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void DimensionController::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Sets the active dimension and deactivates all others.
	// Params:
	//   dimension = The dimension to make active.
	void DimensionController::SetActiveDimension(unsigned dimension)
	{
		if (dimension >= dimensions.size())
			return;

		activeDimension = dimension;

		for (unsigned i = 0; i < dimensions.size(); i++)
		{
			if (i == activeDimension)
				continue;

			static_cast<ColliderTilemap*>(dimensions[i].tilemap->GetComponent("Collider"))->SetActive(false);
			for (GameObject* spike : dimensions[i].spikes)
			{
				UNREFERENCED_PARAMETER(spike);
				//static_cast<Hazard*>(spike->GetComponent("Hazard"))->SetCollidable(false);
			}
		}

		static_cast<ColliderTilemap*>(dimensions[activeDimension].tilemap->GetComponent("Collider"))->SetActive(true);
		for (GameObject* spike : dimensions[activeDimension].spikes)
		{
			UNREFERENCED_PARAMETER(spike);
			//static_cast<Hazard*>(spike->GetComponent("Hazard"))->SetCollidable(true);
		}
	}

	// Adds a new dimension.
	// Params:
	//   tilemap = The game object with the tilemap for the dimension.
	// Returns:
	//   The ID of the new dimension.
	unsigned DimensionController::AddDimension(GameObject* tilemap)
	{
		Dimension dimension(tilemap);
		dimensions.push_back(tilemap);
		return static_cast<unsigned>(dimensions.size() - 1);
	}

	// Adds a spike to an existing dimension.
	// Params:
	//   dimension = The ID of the dimension to add the spike to.
	//   spike = The game object for the spike.
	void DimensionController::AddSpikeToDimension(unsigned dimension, GameObject* spike)
	{
		if (dimension >= dimensions.size())
			return;

		dimensions[dimension].spikes.push_back(spike);
	}

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   tilemap = The game oject with the tilemap for the dimension.
	DimensionController::Dimension::Dimension(GameObject* tilemap) : tilemap(tilemap), spikes(std::vector<GameObject*>())
	{
	}
}

//------------------------------------------------------------------------------
