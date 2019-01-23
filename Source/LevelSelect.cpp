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

// Systems
#include "Archetypes.h"
#include "Space.h"
#include <Input.h>
#include "MeshHelper.h"
#include <Texture.h>
#include <SpriteSource.h>
#include <Mesh.h>
#include <Graphics.h>
#include <Camera.h>

// Components
#include "SpriteText.h"
#include <Transform.h>
#include "Button.h"

// Levels
#include "Level1.h"

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

		// Create a new quad mesh for the sprite.
		meshButton = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// Load the player texture.
		textureButton = Texture::CreateTextureFromFile("Button.png");

		// Setup the player sprite source.
		spriteSourceButton = new SpriteSource(1, 1, textureButton);
	}

	// Initialize the memory associated with LevelSelect.
	void LevelSelect::Initialize()
	{
		std::cout << "LevelSelect::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		objectManager.AddArchetype(*Archetypes::CreateButtonArchetype(meshButton, spriteSourceButton));

		// Create and add descriptive text
		objectManager.AddArchetype(*Archetypes::CreateText());
		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Select Your Level");
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(0.0f, 250.0f));
		objectManager.AddObject(*text);
		
		GameObject* levelButton = new GameObject(*objectManager.GetArchetypeByName("Button"));
		static_cast<Transform*>(levelButton->GetComponent("Transform"))->SetTranslation(Vector2D(-175.0f, 150.0f));
		objectManager.AddObject(*levelButton);

		text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Arena 3");
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetColor(Color(0.0f, 0.0f, 0.0f));
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(-175.0f, 150.0f));
		objectManager.AddObject(*text);

		levelButton = new GameObject(*objectManager.GetArchetypeByName("Button"));
		static_cast<Transform*>(levelButton->GetComponent("Transform"))->SetTranslation(Vector2D(175.0f, 150.0f));
		static_cast<Behaviors::Button*>(levelButton->GetComponent("Button"))->SetMap(Levels::Map::MediumBoy);
		objectManager.AddObject(*levelButton);

		text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("MediumBoy");
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetColor(Color(0.0f, 0.0f, 0.0f));
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(175.0f, 150.0f));
		objectManager.AddObject(*text);

		levelButton = new GameObject(*objectManager.GetArchetypeByName("Button"));
		static_cast<Transform*>(levelButton->GetComponent("Transform"))->SetTranslation(Vector2D(-175.0f, 50.0f));
		static_cast<Behaviors::Button*>(levelButton->GetComponent("Button"))->SetMap(Levels::Map::Channels);
		objectManager.AddObject(*levelButton);

		text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Channels");
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetColor(Color(0.0f, 0.0f, 0.0f));
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(-175.0f, 50.0f));
		objectManager.AddObject(*text);

		levelButton = new GameObject(*objectManager.GetArchetypeByName("Button"));
		static_cast<Transform*>(levelButton->GetComponent("Transform"))->SetTranslation(Vector2D(175.0f, 50.0f));
		static_cast<Behaviors::Button*>(levelButton->GetComponent("Button"))->SetMap(Levels::Map::Separation);
		objectManager.AddObject(*levelButton);

		text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Separation");
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetColor(Color(0.0f, 0.0f, 0.0f));
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(175.0f, 50.0f));
		objectManager.AddObject(*text);

		Camera& camera = Graphics::GetInstance().GetCurrentCamera();
		camera.SetTranslation(Vector2D());
		camera.SetDistance(60.0f);
	}

	// Update LevelSelect.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void LevelSelect::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Unload the resources associated with LevelSelect.
	void LevelSelect::Unload()
	{
		std::cout << "LevelSelect::Unload" << std::endl;

		delete spriteSourceButton;
		delete textureButton;
		delete meshButton;
	}
}
//----------------------------------------------------------------------------
