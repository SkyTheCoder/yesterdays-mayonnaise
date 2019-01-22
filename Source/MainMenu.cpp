//------------------------------------------------------------------------------
//
// File Name:	MainMenu.cpp
// Author(s):	A.J. Bussman
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
#include "MainMenu.h"

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
	// Creates an instance of MainMenu.
	MainMenu::MainMenu()
		: Level("MainMenu")
	{
	}

	// Load the resources associated with MainMenu.
	void MainMenu::Load()
	{
		std::cout << "MainMenu::Load" << std::endl;
	}

	// Initialize the memory associated with MainMenu.
	void MainMenu::Initialize()
	{
		std::cout << "MainMenu::Initialize" << std::endl;

		// GameObjectManager& objectManager = GetSpace()->GetObjectManager();
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void MainMenu::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Switch levels
		Input& input = Input::GetInstance();

		if (input.CheckTriggered(' '))
		{
			GetSpace()->SetLevel(new Level1(Levels::Level1::Map::Arena3));
		}
	}

	// Unload the resources associated with MainMenu.
	void MainMenu::Unload()
	{
		std::cout << "MainMenu::Unload" << std::endl;
	}
}
//----------------------------------------------------------------------------
