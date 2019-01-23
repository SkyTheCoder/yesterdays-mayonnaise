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

// Systems
#include "Archetypes.h"
#include "Space.h"
#include <Input.h>
#include <Graphics.h>
#include <Camera.h>

// Components
#include "SpriteText.h"
#include <Transform.h>

// Levels
#include "LevelSelect.h"

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

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Create and add descriptive text
		objectManager.AddArchetype(*Archetypes::CreateText());
		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Yesterday's Mayonnaise");
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(0.0f, 50.0f));
		objectManager.AddObject(*text);

		text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Press space to go level select");
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(0.0f, -50.0f));
		static_cast<Transform*>(text->GetComponent("Transform"))->SetScale(Vector2D(32.0f, 32.0f));
		objectManager.AddObject(*text);

		Camera& camera = Graphics::GetInstance().GetCurrentCamera();
		camera.SetTranslation(Vector2D());
		camera.SetDistance(60.0f);
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
			GetSpace()->SetLevel(new LevelSelect());
		}
	}

	// Unload the resources associated with MainMenu.
	void MainMenu::Unload()
	{
		std::cout << "MainMenu::Unload" << std::endl;
	}
}
//----------------------------------------------------------------------------
