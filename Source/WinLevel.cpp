//------------------------------------------------------------------------------
//
// File Name:	WinLevel.cpp
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
#include "WinLevel.h"

#include "Level1.h"

#include "Archetypes.h"
#include "Space.h"
#include "SpriteText.h"
#include <Input.h>

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of WinLevel.
	WinLevel::WinLevel()
		: Level("WinLevel")
	{
	}

	// Load the resources associated with WinLevel.
	void WinLevel::Load()
	{
		std::cout << "WinLevel::Load" << std::endl;
	}

	// Initialize the memory associated with WinLevel.
	void WinLevel::Initialize()
	{
		std::cout << "WinLevel::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Create and add win-text
		objectManager.AddArchetype(*Archetypes::CreateText());
		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("u win pres space 2 rstrt");
		objectManager.AddObject(*text);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void WinLevel::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Switch levels
		Input& input = Input::GetInstance();

		if (input.CheckTriggered(' '))
		{
			GetSpace()->SetLevel(new Level1());
		}
	}

	// Unload the resources associated with WinLevel.
	void WinLevel::Unload()
	{
		std::cout << "WinLevel::Unload" << std::endl;
	}
}
//----------------------------------------------------------------------------
