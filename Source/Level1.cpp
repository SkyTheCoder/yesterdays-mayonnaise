//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
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

#include "Level1.h"

// Systems
#include <Engine.h>
#include "Space.h"
#include "MeshHelper.h"
#include <Mesh.h>
#include "SpriteSource.h"
#include <Texture.h>
#include <Input.h>
#include "Archetypes.h"
#include <glfw3.h>
#include "Tilemap.h"
#include <Graphics.h>

// Components
#include "Sprite.h"
#include "Animation.h"
#include "Transform.h"
#include "Physics.h"
#include "SpriteText.h"
#include "ChipCollectible.h"
#include "WinLevel.h"
#include "CameraFollow.h"
#include "PlayerMovement.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 1.
	Level1::Level1() : Level("Level1"), meshMonkey(nullptr), textureMonkey(nullptr), spriteSourceMonkey(nullptr),
		columnsMonkey(3), rowsMonkey(5),
		dataStaticMap(nullptr), dataRedMap(nullptr), dataBlueMap(nullptr),
		textureStaticMap(nullptr), textureRedMap(nullptr), textureBlueMap(nullptr),
		spriteSourceStaticMap(nullptr), spriteSourceRedMap(nullptr), spriteSourceBlueMap(nullptr),
		meshMap(nullptr), columnsMap(4), rowsMap(3)
	{
	}

	// Load the resources associated with Level 1.
	void Level1::Load()
	{
		// Create a new quad mesh for the sprite.
		meshMonkey = CreateQuadMesh(Vector2D(1.0f / columnsMonkey, 1.0f / rowsMonkey), Vector2D(0.5f, 0.5f));

		// Load the player texture.
		textureMonkey = Texture::CreateTextureFromFile("Monkey.png");

		// Setup the player sprite source.
		spriteSourceMonkey = new SpriteSource(columnsMonkey, rowsMonkey, textureMonkey);

		// create collectible archetype and add to object manager archetypes
		genericQuadMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		textureCollectible = Texture::CreateTextureFromFile("collectible.png");
		spriteSourceCollectible = new SpriteSource(1, 1, textureCollectible);
		GetSpace()->GetObjectManager().AddArchetype(*Archetypes::CreateCollectibleArchetype(genericQuadMesh, spriteSourceCollectible));

		// Load the tilemap.
		dataStaticMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Arena3Static.txt");
		dataRedMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Arena3Red.txt");
		dataBlueMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Arena3Blue.txt");

		if (dataStaticMap == nullptr || dataRedMap == nullptr || dataBlueMap == nullptr)
		{
			std::cout << "Error loading map!" << std::endl;
		}
		else
		{
			// Create a new quad mesh for the sprite tilemap.
			meshMap = CreateQuadMesh(Vector2D(1.0f / columnsMap, 1.0f / rowsMap), Vector2D(0.5f, 0.5f));

			// Load the tilemap texture.
			textureStaticMap = Texture::CreateTextureFromFile("Tilemap.png");
			textureRedMap = Texture::CreateTextureFromFile("Tilemap.png");
			textureBlueMap = Texture::CreateTextureFromFile("Tilemap.png");

			// Setup the tilemap sprite source.
			spriteSourceStaticMap = new SpriteSource(columnsMap, rowsMap, textureStaticMap);
			spriteSourceRedMap = new SpriteSource(columnsMap, rowsMap, textureRedMap);
			spriteSourceBlueMap = new SpriteSource(columnsMap, rowsMap, textureBlueMap);
		}

		// Set the background color to black.
		Graphics::GetInstance().SetBackgroundColor(Colors::Black);
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		
		// Add archetypes to the object manager.
		objectManager.AddArchetype(*Archetypes::CreatePlayer(meshMonkey, spriteSourceMonkey));
		objectManager.AddArchetype(*Archetypes::CreateText());
		objectManager.AddArchetype(*Archetypes::CreateGameController());

		// Create collectible(s)
		GameObject* collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		collectible->AddComponent(new Behaviors::ChipCollectible());
		static_cast<Transform*>(collectible->GetComponent("Transform"))->SetTranslation(Vector2D(300.0f, 0.0f));
		objectManager.AddObject(*collectible);

		// Create the player and add it to the object manager.
		GameObject* player = new GameObject(*objectManager.GetArchetypeByName("Player"));
		//static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(100.0f, -2800.0f));
		static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(-150.0f, 50.0f));
		objectManager.AddObject(*player);

		GameObject* player2 = new GameObject(*objectManager.GetArchetypeByName("Player"));
		static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"))->SetKeybinds('W', 'A', 'D');
		//static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(100.0f, -2800.0f));
		static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(-150.0f, 50.0f));
		objectManager.AddObject(*player2);

		// Create winText and add to objectManager
		GameObject* winText = new GameObject(*objectManager.GetArchetypeByName("Text"));
		objectManager.AddObject(*winText);

		// Play the player's animation.
		static_cast<Animation*>(player->GetComponent("Animation"))->Play(0, 8, 0.2f, true);

		if (dataStaticMap != nullptr && dataRedMap != nullptr && dataBlueMap != nullptr)
		{
			// Create the tilemap and add it to the object manager.
			GameObject* tilemap = Archetypes::CreateTilemapObject(meshMap, spriteSourceStaticMap, dataStaticMap);
			objectManager.AddObject(*tilemap);
		}

		GameObject* gameController = new GameObject(*objectManager.GetArchetypeByName("GameController"));
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player);
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player2);
		objectManager.AddObject(*gameController);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// End game if a player dies
		if (GetSpace()->GetObjectManager().GetObjectCount("Player") < 2)
		{
			Input& input = Input::GetInstance();

			GameObject* winText = GetSpace()->GetObjectManager().GetObjectByName("Text");

			// Set text to winText
			static_cast<SpriteText*>(winText->GetComponent("SpriteText"))->SetText("Someone won! Press <SPACE> to restart");

			// Text follows camera
			static_cast<Transform*>(winText->GetComponent("Transform"))
				->SetTranslation(Graphics::GetInstance().GetCurrentCamera().GetTranslation());

			// Restart on <SPACE>
			if (input.CheckTriggered(' '))
			{
				GetSpace()->RestartLevel();
			}
		}
	}

	// Unload the resources associated with Level 1.
	void Level1::Unload()
	{
		// Free all allocated memory.
		delete spriteSourceStaticMap;
		delete spriteSourceRedMap;
		delete spriteSourceBlueMap;
		delete textureStaticMap;
		delete textureRedMap;
		delete textureBlueMap;
		delete meshMap;
		delete dataStaticMap;
		delete dataRedMap;
		delete dataBlueMap;
		delete spriteSourceCollectible;
		delete textureCollectible;
		delete spriteSourceMonkey;
		delete textureMonkey;
		delete meshMonkey;
		delete genericQuadMesh;
	}
}

//----------------------------------------------------------------------------
