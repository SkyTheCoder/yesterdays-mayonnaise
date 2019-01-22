//------------------------------------------------------------------------------
//
// File Name:	LevelSelect.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "LevelSelect.h"

#include "Level1.h"

#include "Archetypes.h"
#include "Space.h"
#include "SpriteText.h"
#include <Input.h>
#include <Transform.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of LevelSelect.
	LevelSelect::LevelSelect()
		: Level("LevelSelect")
	{
	}

	// Load the resources associated with LevelSelect.
	void LevelSelect::Load()
	{
		std::cout << "LevelSelect::Load" << std::endl;
	}

	// Initialize the memory associated with LevelSelect.
	void LevelSelect::Initialize()
	{
		std::cout << "LevelSelect::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Create and add descriptive text
		objectManager.AddArchetype(*Archetypes::CreateText());
		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Select Your Level");
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(0.0f, 250.0f));
		objectManager.AddObject(*text);
	}

	// Update LevelSelect.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void LevelSelect::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Switch levels
		//Input& input = Input::GetInstance();
	}

	// Unload the resources associated with LevelSelect.
	void LevelSelect::Unload()
	{
		std::cout << "LevelSelect::Unload" << std::endl;
	}
}
//----------------------------------------------------------------------------
