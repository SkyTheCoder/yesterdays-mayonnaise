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
#include "CameraFollow.h"
#include "PlayerMovement.h"
#include "DimensionController.h"

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
	Level1::Level1() : Level("Level1"),
		meshMonkey(nullptr), textureMonkey(nullptr), spriteSourceMonkey(nullptr),
		columnsMonkey(3), rowsMonkey(5),
		meshGenericQuad(nullptr),
		textureCollectible(nullptr), spriteSourceCollectible(nullptr),
		meshSpikes(nullptr), textureSpikes(nullptr), spriteSourceSpikes(nullptr), columnsSpikes(1), rowsSpikes(3),
		dataStaticMap(nullptr), dataRedMap(nullptr), dataBlueMap(nullptr),
		textureStaticMap(nullptr), textureRedMap(nullptr), textureBlueMap(nullptr),
		spriteSourceStaticMap(nullptr), spriteSourceRedMap(nullptr), spriteSourceBlueMap(nullptr),
		meshMap(nullptr), columnsMap(2), rowsMap(2)
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

		meshGenericQuad = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// Load collectible assets
		textureCollectible = Texture::CreateTextureFromFile("Collectible.png");
		spriteSourceCollectible = new SpriteSource(1, 1, textureCollectible);

		// Load spike assets
		meshSpikes = CreateQuadMesh(Vector2D(1.0f / columnsSpikes, 1.0f / rowsSpikes), Vector2D(0.5f, 0.5f));
		textureSpikes = Texture::CreateTextureFromFile("Spikes.png");
		spriteSourceSpikes = new SpriteSource(columnsSpikes, rowsSpikes, textureSpikes);

		// Load the tilemaps.
		std::string mapName = "MediumBoy";
		dataStaticMap = Tilemap::CreateTilemapFromFile("Assets/Levels/" + mapName + "Static.txt");
		dataRedMap = Tilemap::CreateTilemapFromFile("Assets/Levels/" + mapName + "Red.txt");
		dataBlueMap = Tilemap::CreateTilemapFromFile("Assets/Levels/" + mapName + "Blue.txt");

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
		objectManager.AddArchetype(*Archetypes::CreateCollectibleArchetype(meshGenericQuad, spriteSourceCollectible));
		objectManager.AddArchetype(*Archetypes::CreateHazardArchetype("StaticSpike", meshSpikes, spriteSourceSpikes, 0));
		objectManager.AddArchetype(*Archetypes::CreateHazardArchetype("RedSpike", meshSpikes, spriteSourceSpikes, 1));
		objectManager.AddArchetype(*Archetypes::CreateHazardArchetype("BlueSpike", meshSpikes, spriteSourceSpikes, 2));

		// Create the players and add them to the object manager.
		GameObject* player = new GameObject(*objectManager.GetArchetypeByName("Player"));
		Behaviors::PlayerMovement* playerMovement = static_cast<Behaviors::PlayerMovement*>(player->GetComponent("PlayerMovement"));
		playerMovement->SetPlayerID(1);
		static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(900.0f, -300.0f));
		//static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(600.0f, -2600.0f));
		//static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(300.0f, -200.0f));
		objectManager.AddObject(*player);

		GameObject* player2 = new GameObject(*objectManager.GetArchetypeByName("Player"));
		Behaviors::PlayerMovement* player2Movement = static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"));
		player2Movement->SetKeybinds('W', 'A', 'D', VK_LCONTROL);
		player2Movement->SetPlayerID(2);
		static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(2200.0f, -200.0f));	
		//static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(2300.0f, -2700.0f));
		//static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(2800.0f, -200.0f));
		objectManager.AddObject(*player2);

		// Create the Game Controller, which handles the camera and dimensions.
		GameObject* gameController = new GameObject(*objectManager.GetArchetypeByName("GameController"));
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player);
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player2);
		Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));
		objectManager.AddObject(*gameController);

		if (dataStaticMap != nullptr && dataRedMap != nullptr && dataBlueMap != nullptr)
		{
			// Create the tilemap and add it to the object manager.
			GameObject* tilemapStatic = Archetypes::CreateTilemapObject(meshMap, spriteSourceStaticMap, dataStaticMap);
			objectManager.AddObject(*tilemapStatic);

			GameObject* tilemapRed = Archetypes::CreateTilemapObject(meshMap, spriteSourceRedMap, dataRedMap);
			objectManager.AddObject(*tilemapRed);
			unsigned redDimension = dimensionController.AddDimension(tilemapRed);

			GameObject* tilemapBlue = Archetypes::CreateTilemapObject(meshMap, spriteSourceBlueMap, dataBlueMap);
			objectManager.AddObject(*tilemapBlue);
			unsigned blueDimension = dimensionController.AddDimension(tilemapBlue);

			// Map data (spikes, collectibles, etc.) - this will be separated into a much neater format for the final level select screen!

#if 0
			// Level: Arena3.
			// Map data automatically generated by Processing.
			// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

			float staticSpikes[168] = {
				13.0f, 0.0f, 14.0f, 0.0f, 15.0f, 0.0f, 29.0f, 0.0f, 30.0f, 0.0f, 31.0f, 0.0f, 33.0f, 0.0f, 34.0f, 0.0f, 35.0f, 0.0f, 48.0f, 1.0f, 48.0f, 2.0f, 48.0f, 3.0f, 4.0f, 4.0f, 5.0f, 4.0f, 6.0f, 4.0f, 7.0f, 4.0f, 
				8.0f, 4.0f, 0.0f, 5.0f, 0.0f, 6.0f, 0.0f, 7.0f, 28.0f, 8.0f, 0.0f, 9.0f, 28.0f, 9.0f, 48.0f, 9.0f, 0.0f, 10.0f, 28.0f, 10.0f, 48.0f, 10.0f, 0.0f, 11.0f, 28.0f, 11.0f, 48.0f, 11.0f, 16.0f, 12.0f, 16.0f, 13.0f, 
				16.0f, 14.0f, 16.0f, 15.0f, 37.0f, 16.0f, 38.0f, 16.0f, 39.0f, 16.0f, 1.0f, 24.0f, 2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 24.0f, 5.0f, 24.0f, 6.0f, 24.0f, 7.0f, 24.0f, 8.0f, 24.0f, 9.0f, 24.0f, 10.0f, 24.0f, 11.0f, 24.0f, 
				12.0f, 24.0f, 13.0f, 24.0f, 14.0f, 24.0f, 15.0f, 24.0f, 16.0f, 24.0f, 17.0f, 24.0f, 18.0f, 24.0f, 19.0f, 24.0f, 20.0f, 24.0f, 21.0f, 24.0f, 22.0f, 24.0f, 23.0f, 24.0f, 24.0f, 24.0f, 25.0f, 24.0f, 26.0f, 24.0f, 27.0f, 24.0f, 
				28.0f, 24.0f, 29.0f, 24.0f, 30.0f, 24.0f, 31.0f, 24.0f, 32.0f, 24.0f, 33.0f, 24.0f, 34.0f, 24.0f, 35.0f, 24.0f, 36.0f, 24.0f, 37.0f, 24.0f, 38.0f, 24.0f, 39.0f, 24.0f, 40.0f, 24.0f, 41.0f, 24.0f, 42.0f, 24.0f, 43.0f, 24.0f, 
				44.0f, 24.0f, 45.0f, 24.0f, 46.0f, 24.0f, 47.0f, 24.0f
			};

			for (int i = 0; i < 168; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("StaticSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(staticSpikes[i] * 100.0f, staticSpikes[i + 1] * -100.0f));
				objectManager.AddObject(*spike);
			}

			float redSpikes[94] = {
				12.0f, 1.0f, 40.0f, 1.0f, 12.0f, 2.0f, 40.0f, 2.0f, 12.0f, 3.0f, 40.0f, 3.0f, 9.0f, 4.0f, 10.0f, 4.0f, 11.0f, 4.0f, 12.0f, 4.0f, 29.0f, 4.0f, 30.0f, 4.0f, 31.0f, 4.0f, 32.0f, 4.0f, 33.0f, 4.0f, 34.0f, 4.0f, 
				35.0f, 4.0f, 1.0f, 8.0f, 2.0f, 8.0f, 3.0f, 8.0f, 8.0f, 8.0f, 45.0f, 8.0f, 46.0f, 8.0f, 47.0f, 8.0f, 8.0f, 9.0f, 8.0f, 10.0f, 8.0f, 11.0f, 17.0f, 12.0f, 18.0f, 12.0f, 19.0f, 12.0f, 4.0f, 13.0f, 40.0f, 13.0f, 
				4.0f, 14.0f, 40.0f, 14.0f, 4.0f, 15.0f, 40.0f, 15.0f, 40.0f, 16.0f, 20.0f, 17.0f, 20.0f, 18.0f, 20.0f, 19.0f, 1.0f, 20.0f, 2.0f, 20.0f, 3.0f, 20.0f, 20.0f, 20.0f, 21.0f, 20.0f, 22.0f, 20.0f, 23.0f, 20.0f
			};

			for (int i = 0; i < 94; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("RedSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(redSpikes[i] * 100.0f, redSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(redDimension, spike);
				objectManager.AddObject(*spike);
			}

			float blueSpikes[72] = {
				24.0f, 1.0f, 24.0f, 2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 20.0f, 5.0f, 40.0f, 5.0f, 20.0f, 6.0f, 40.0f, 6.0f, 20.0f, 7.0f, 40.0f, 7.0f, 4.0f, 8.0f, 20.0f, 8.0f, 4.0f, 9.0f, 4.0f, 10.0f, 4.0f, 11.0f, 4.0f, 12.0f, 
				36.0f, 12.0f, 36.0f, 13.0f, 36.0f, 14.0f, 36.0f, 15.0f, 45.0f, 16.0f, 46.0f, 16.0f, 47.0f, 16.0f, 28.0f, 17.0f, 32.0f, 17.0f, 28.0f, 18.0f, 32.0f, 18.0f, 28.0f, 19.0f, 32.0f, 19.0f, 8.0f, 20.0f, 9.0f, 20.0f, 10.0f, 20.0f, 
				11.0f, 20.0f, 12.0f, 20.0f, 28.0f, 20.0f, 32.0f, 20.0f
			};

			for (int i = 0; i < 72; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("BlueSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(blueSpikes[i] * 100.0f, blueSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(blueDimension, spike);
				objectManager.AddObject(*spike);
			}
#endif

#if 1
			// Level: MediumBoy.
			// Map data automatically generated by Processing.
			// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

			float staticSpikes[60] = {
				1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f, 13.0f, 31.0f, 14.0f, 31.0f, 15.0f, 31.0f, 16.0f, 31.0f,
				17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f, 29.0f, 31.0f, 30.0f, 31.0f
			};

			for (int i = 0; i < 60; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("StaticSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(staticSpikes[i] * 100.0f, staticSpikes[i + 1] * -100.0f));
				objectManager.AddObject(*spike);
			}

			float redSpikes[66] = {
				26.0f, 8.0f, 27.0f, 8.0f, 28.0f, 8.0f, 29.0f, 8.0f, 30.0f, 8.0f, 1.0f, 13.0f, 2.0f, 13.0f, 3.0f, 13.0f, 4.0f, 13.0f, 5.0f, 13.0f, 30.0f, 14.0f, 30.0f, 15.0f, 30.0f, 16.0f, 30.0f, 17.0f, 14.0f, 18.0f, 15.0f, 18.0f,
				30.0f, 18.0f, 30.0f, 19.0f, 30.0f, 20.0f, 28.0f, 21.0f, 29.0f, 21.0f, 30.0f, 21.0f, 30.0f, 22.0f, 16.0f, 23.0f, 17.0f, 23.0f, 30.0f, 23.0f, 30.0f, 24.0f, 30.0f, 25.0f, 30.0f, 26.0f, 30.0f, 27.0f, 30.0f, 28.0f, 30.0f, 29.0f,
				30.0f, 30.0f
			};

			for (int i = 0; i < 66; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("RedSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(redSpikes[i] * 100.0f, redSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(redDimension, spike);
				objectManager.AddObject(*spike);
			}

			float blueSpikes[66] = {
				1.0f, 8.0f, 2.0f, 8.0f, 3.0f, 8.0f, 4.0f, 8.0f, 5.0f, 8.0f, 26.0f, 13.0f, 27.0f, 13.0f, 28.0f, 13.0f, 29.0f, 13.0f, 30.0f, 13.0f, 1.0f, 14.0f, 1.0f, 15.0f, 1.0f, 16.0f, 1.0f, 17.0f, 1.0f, 18.0f, 16.0f, 18.0f,
				17.0f, 18.0f, 1.0f, 19.0f, 1.0f, 20.0f, 1.0f, 21.0f, 2.0f, 21.0f, 3.0f, 21.0f, 1.0f, 22.0f, 1.0f, 23.0f, 14.0f, 23.0f, 15.0f, 23.0f, 1.0f, 24.0f, 1.0f, 25.0f, 1.0f, 26.0f, 1.0f, 27.0f, 1.0f, 28.0f, 1.0f, 29.0f,
				1.0f, 30.0f
			};

			for (int i = 0; i < 66; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("BlueSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(blueSpikes[i] * 100.0f, blueSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(blueDimension, spike);
				objectManager.AddObject(*spike);
			}

			float chipsSpawns[16] = {
				4.0f, 6.0f, 27.0f, 6.0f, 2.0f, 16.0f, 29.0f, 16.0f, 10.0f, 21.0f, 21.0f, 21.0f, 2.0f, 28.0f, 29.0f, 28.0f
			};

			for (int i = 0; i < 16; i += 2)
			{
				GameObject* chips = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
				static_cast<Transform*>(chips->GetComponent("Transform"))->SetTranslation(Vector2D(chipsSpawns[i] * 100.0f, chipsSpawns[i + 1] * -100.0f));
				objectManager.AddObject(*chips);
			}
#endif

#if 0
			// Level: Channels.
			// Map data automatically generated by Processing.
			// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

			float staticSpikes[68] = {
				10.0f, 9.0f, 21.0f, 9.0f, 5.0f, 17.0f, 26.0f, 17.0f, 1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f,
				13.0f, 31.0f, 14.0f, 31.0f, 15.0f, 31.0f, 16.0f, 31.0f, 17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f,
				29.0f, 31.0f, 30.0f, 31.0f
			};

			for (int i = 0; i < 68; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("StaticSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(staticSpikes[i] * 100.0f, staticSpikes[i + 1] * -100.0f));
				objectManager.AddObject(*spike);
			}

			float redSpikes[42] = {
				22.0f, 8.0f, 22.0f, 9.0f, 22.0f, 10.0f, 22.0f, 11.0f, 11.0f, 12.0f, 12.0f, 12.0f, 22.0f, 12.0f, 11.0f, 13.0f, 12.0f, 13.0f, 22.0f, 13.0f, 22.0f, 14.0f, 22.0f, 15.0f, 22.0f, 16.0f, 22.0f, 17.0f, 22.0f, 18.0f, 22.0f, 19.0f,
				22.0f, 20.0f, 22.0f, 21.0f, 22.0f, 22.0f, 22.0f, 23.0f, 22.0f, 24.0f
			};

			for (int i = 0; i < 42; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("RedSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(redSpikes[i] * 100.0f, redSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(redDimension, spike);
				objectManager.AddObject(*spike);
			}

			float blueSpikes[42] = {
				9.0f, 8.0f, 9.0f, 9.0f, 9.0f, 10.0f, 9.0f, 11.0f, 9.0f, 12.0f, 19.0f, 12.0f, 20.0f, 12.0f, 9.0f, 13.0f, 19.0f, 13.0f, 20.0f, 13.0f, 9.0f, 14.0f, 9.0f, 15.0f, 9.0f, 16.0f, 9.0f, 17.0f, 9.0f, 18.0f, 9.0f, 19.0f,
				9.0f, 20.0f, 9.0f, 21.0f, 9.0f, 22.0f, 9.0f, 23.0f, 9.0f, 24.0f
			};

			for (int i = 0; i < 42; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("BlueSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(blueSpikes[i] * 100.0f, blueSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(blueDimension, spike);
				objectManager.AddObject(*spike);
			}

			float chipsSpawns[8] = {
				6.0f, 6.0f, 25.0f, 6.0f, 4.0f, 21.0f, 27.0f, 21.0f
			};

			for (int i = 0; i < 8; i += 2)
			{
				GameObject* chips = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
				static_cast<Transform*>(chips->GetComponent("Transform"))->SetTranslation(Vector2D(chipsSpawns[i] * 100.0f, chipsSpawns[i + 1] * -100.0f));
				objectManager.AddObject(*chips);
			}


#endif

#if 0
			// Level: Separation.
			// Map data automatically generated by Processing.
			// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

			float staticSpikes[60] = {
				1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f, 13.0f, 31.0f, 14.0f, 31.0f, 15.0f, 31.0f, 16.0f, 31.0f,
				17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f, 29.0f, 31.0f, 30.0f, 31.0f
			};

			for (int i = 0; i < 60; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("StaticSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(staticSpikes[i] * 100.0f, staticSpikes[i + 1] * -100.0f));
				objectManager.AddObject(*spike);
			}

			float redSpikes[16] = {
				10.0f, 15.0f, 11.0f, 15.0f, 20.0f, 15.0f, 21.0f, 15.0f, 10.0f, 16.0f, 11.0f, 16.0f, 20.0f, 16.0f, 21.0f, 16.0f
			};

			for (int i = 0; i < 16; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("RedSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(redSpikes[i] * 100.0f, redSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(redDimension, spike);
				objectManager.AddObject(*spike);
			}

			float blueSpikes[32] = {
				3.0f, 15.0f, 28.0f, 15.0f, 3.0f, 16.0f, 28.0f, 16.0f, 3.0f, 17.0f, 28.0f, 17.0f, 3.0f, 18.0f, 28.0f, 18.0f, 3.0f, 21.0f, 28.0f, 21.0f, 3.0f, 22.0f, 28.0f, 22.0f, 3.0f, 23.0f, 28.0f, 23.0f, 3.0f, 24.0f, 28.0f, 24.0f
			};

			for (int i = 0; i < 32; i += 2)
			{
				GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("BlueSpike"));
				static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(blueSpikes[i] * 100.0f, blueSpikes[i + 1] * -100.0f));
				dimensionController.AddSpikeToDimension(blueDimension, spike);
				objectManager.AddObject(*spike);
			}

			float chipsSpawns[8] = {
				11.0f, 11.0f, 20.0f, 11.0f, 5.0f, 20.0f, 26.0f, 20.0f
			};

			for (int i = 0; i < 8; i += 2)
			{
				GameObject* chips = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
				static_cast<Transform*>(chips->GetComponent("Transform"))->SetTranslation(Vector2D(chipsSpawns[i] * 100.0f, chipsSpawns[i + 1] * -100.0f));
				objectManager.AddObject(*chips);
			}
#endif

			dimensionController.SetActiveDimension(redDimension);
		}

		// Create winText and add to objectManager
		GameObject* winText = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(winText->GetComponent("SpriteText"))->SetColor(Colors::White);
		objectManager.AddObject(*winText);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		//GameObject* gameController = GetSpace()->GetObjectManager().GetObjectByName("GameController");
		//Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// End game if a player dies
		unsigned playerCount = objectManager.GetObjectCount("Player");
		if (playerCount == 1)
		{
			GameObject* winText = GetSpace()->GetObjectManager().GetObjectByName("Text");

			GameObject* lastPlayer = objectManager.GetObjectByName("Player");
			Behaviors::PlayerMovement* lastPlayerMovement = static_cast<Behaviors::PlayerMovement*>(lastPlayer->GetComponent("PlayerMovement"));

			// Set text to winText
			static_cast<SpriteText*>(winText->GetComponent("SpriteText"))->SetText(std::string("Player ") + std::to_string(lastPlayerMovement->GetPlayerID()) + std::string(" won! Press <SPACE> to restart"));

			// Text follows camera
			static_cast<Transform*>(winText->GetComponent("Transform"))
				->SetTranslation(Graphics::GetInstance().GetCurrentCamera().GetTranslation());

			// Restart on <SPACE>
			if (input.CheckTriggered(' '))
			{
				GetSpace()->RestartLevel();
			}
		}
		else if (playerCount <= 0)
		{
			GetSpace()->RestartLevel();
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
		delete spriteSourceSpikes;
		delete textureSpikes;
		delete meshSpikes;
		delete spriteSourceCollectible;
		delete textureCollectible;
		delete spriteSourceMonkey;
		delete textureMonkey;
		delete meshMonkey;
		delete meshGenericQuad;
	}
}

//----------------------------------------------------------------------------
