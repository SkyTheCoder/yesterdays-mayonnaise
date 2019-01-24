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
#include <Random.h>

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
#include "MonkeyAnimation.h"

// Levels
#include "LevelSelect.h"

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
	Level1::Level1(Map map_) : Level("Level1"),
		map(map_),
		meshMonkey(nullptr), textureMonkey(nullptr), spriteSourceMonkey(nullptr),
		columnsMonkey(3), rowsMonkey(6),
		meshCat(nullptr), textureCat(nullptr), spriteSourceCat(nullptr),
		columnsCat(4), rowsCat(3),
		meshGenericQuad(nullptr),
		textureCollectible(nullptr), spriteSourceCollectible(nullptr),
		meshSpikes(nullptr), textureSpikes(nullptr), spriteSourceSpikes(nullptr), columnsSpikes(1), rowsSpikes(3),
		dataStaticMap(nullptr), dataRedMap(nullptr), dataBlueMap(nullptr),
		textureStaticMap(nullptr), textureRedMap(nullptr), textureBlueMap(nullptr),
		spriteSourceStaticMap(nullptr), spriteSourceRedMap(nullptr), spriteSourceBlueMap(nullptr),
		meshMap(nullptr), columnsMap(2), rowsMap(2),
		firstFrame(true)
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

		// Create a new quad mesh for the sprite.
		meshCat = CreateQuadMesh(Vector2D(1.0f / columnsCat, 1.0f / rowsCat), Vector2D(0.5f, 0.5f));

		// Load the player texture.
		textureCat = Texture::CreateTextureFromFile("Cat.png");

		// Setup the player sprite source.
		spriteSourceCat = new SpriteSource(columnsCat, rowsCat, textureCat);

		meshGenericQuad = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// Load collectible assets
		textureCollectible = Texture::CreateTextureFromFile("Collectible.png");
		spriteSourceCollectible = new SpriteSource(1, 1, textureCollectible);

		// Load powerup assets
		textureJumpBoost = Texture::CreateTextureFromFile("Spring.png");
		textureSpeedBoost = Texture::CreateTextureFromFile("SpeedBoost.png");
		spriteSourceJumpBoost = new SpriteSource(1, 1, textureJumpBoost);
		spriteSourceSpeedBoost = new SpriteSource(1, 1, textureSpeedBoost);

		// Load spike assets
		meshSpikes = CreateQuadMesh(Vector2D(1.0f / columnsSpikes, 1.0f / rowsSpikes), Vector2D(0.5f, 0.5f));
		textureSpikes = Texture::CreateTextureFromFile("Spikes.png");
		spriteSourceSpikes = new SpriteSource(columnsSpikes, rowsSpikes, textureSpikes);

		// Load the tilemaps.
		std::string mapName;
		switch (map)
		{
		case Map::Tutorial:
			mapName = "Tutorial";
			break;
		case Map::Arena3:
			mapName = "Arena3";
			break;
		case Map::MediumBoy:
			mapName = "MediumBoy";
			break;
		case Map::Channels:
			mapName = "Channels";
			break;
		case Map::Separation:
			mapName = "Separation";
			break;
		case Map::Descent:
			mapName = "Descent";
			break;
		}
		
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
		objectManager.AddArchetype(*Archetypes::CreateJumpBoostArchetype(meshGenericQuad, spriteSourceJumpBoost));
		objectManager.AddArchetype(*Archetypes::CreateSpeedBoostArchetype(meshGenericQuad, spriteSourceSpeedBoost));

		// Create the players and add them to the object manager.
		GameObject* player = new GameObject(*objectManager.GetArchetypeByName("Player"));
		Behaviors::PlayerMovement* playerMovement = static_cast<Behaviors::PlayerMovement*>(player->GetComponent("PlayerMovement"));
		playerMovement->SetPlayerID(1);
		objectManager.AddObject(*player);

		GameObject* player2 = new GameObject(*objectManager.GetArchetypeByName("Player"));
		Sprite* player2Sprite = static_cast<Sprite*>(player2->GetComponent("Sprite"));
		player2Sprite->SetMesh(meshCat);
		player2Sprite->SetSpriteSource(spriteSourceCat);
		static_cast<Behaviors::MonkeyAnimation*>(player2->GetComponent("MonkeyAnimation"))->SetFrames(0, 4, 8, 4, 4, 4);
		Behaviors::PlayerMovement* player2Movement = static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"));
		player2Movement->SetKeybinds('W', 'A', 'D', VK_LCONTROL);
		player2Movement->SetPlayerID(2);
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

			// Map data (spikes, collectibles, etc.)
			// Map data automatically generated by Processing.
			// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

			switch (map)
			{
			case Map::Tutorial:
			{
				float staticSpikes[84] = {
					1.0f, 12.0f, 1.0f, 13.0f, 1.0f, 14.0f, 1.0f, 15.0f, 1.0f, 16.0f, 1.0f, 17.0f, 20.0f, 17.0f, 1.0f, 18.0f, 20.0f, 18.0f, 1.0f, 19.0f, 20.0f, 19.0f, 1.0f, 20.0f, 1.0f, 21.0f, 1.0f, 22.0f, 1.0f, 23.0f, 1.0f, 24.0f,
					2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 24.0f, 5.0f, 24.0f, 6.0f, 24.0f, 7.0f, 24.0f, 8.0f, 24.0f, 9.0f, 24.0f, 10.0f, 24.0f, 11.0f, 24.0f, 12.0f, 24.0f, 13.0f, 24.0f, 14.0f, 24.0f, 15.0f, 24.0f, 16.0f, 24.0f, 17.0f, 24.0f,
					18.0f, 24.0f, 28.0f, 30.0f, 29.0f, 30.0f, 30.0f, 30.0f, 34.0f, 30.0f, 35.0f, 30.0f, 36.0f, 30.0f, 40.0f, 30.0f, 41.0f, 30.0f, 42.0f, 30.0f
				};

				float redSpikes[8] = {
					23.0f, 20.0f, 33.0f, 20.0f, 23.0f, 21.0f, 33.0f, 21.0f
				};

				float blueSpikes[8] = {
					28.0f, 20.0f, 38.0f, 20.0f, 28.0f, 21.0f, 38.0f, 21.0f
				};

				float chipsSpawns[12] = {
					22.0f, 16.0f, 44.0f, 19.0f, 20.0f, 21.0f, 5.0f, 28.0f, 24.0f, 28.0f, 44.0f, 28.0f
				};

				AddStaticSpikes(staticSpikes, 42);
				AddRedSpikes(redSpikes, 4, redDimension);
				AddBlueSpikes(blueSpikes, 4, blueDimension);
				AddChips(chipsSpawns, 6);

				static_cast<Sprite*>(player2->GetComponent("Sprite"))->SetAlpha(0.0f);
				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(100.0f, -2800.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(100.0f, -2800.0f));
				static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"))->SetKeybinds(VK_UP, VK_LEFT, VK_RIGHT, VK_RCONTROL);

				break;
			}
			case Map::Arena3:
			{
				float staticSpikes[168] = {
					13.0f, 0.0f, 14.0f, 0.0f, 15.0f, 0.0f, 29.0f, 0.0f, 30.0f, 0.0f, 31.0f, 0.0f, 33.0f, 0.0f, 34.0f, 0.0f, 35.0f, 0.0f, 48.0f, 1.0f, 48.0f, 2.0f, 48.0f, 3.0f, 4.0f, 4.0f, 5.0f, 4.0f, 6.0f, 4.0f, 7.0f, 4.0f,
					8.0f, 4.0f, 0.0f, 5.0f, 0.0f, 6.0f, 0.0f, 7.0f, 28.0f, 8.0f, 0.0f, 9.0f, 28.0f, 9.0f, 48.0f, 9.0f, 0.0f, 10.0f, 28.0f, 10.0f, 48.0f, 10.0f, 0.0f, 11.0f, 28.0f, 11.0f, 48.0f, 11.0f, 16.0f, 12.0f, 16.0f, 13.0f,
					16.0f, 14.0f, 16.0f, 15.0f, 37.0f, 16.0f, 38.0f, 16.0f, 39.0f, 16.0f, 1.0f, 24.0f, 2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 24.0f, 5.0f, 24.0f, 6.0f, 24.0f, 7.0f, 24.0f, 8.0f, 24.0f, 9.0f, 24.0f, 10.0f, 24.0f, 11.0f, 24.0f,
					12.0f, 24.0f, 13.0f, 24.0f, 14.0f, 24.0f, 15.0f, 24.0f, 16.0f, 24.0f, 17.0f, 24.0f, 18.0f, 24.0f, 19.0f, 24.0f, 20.0f, 24.0f, 21.0f, 24.0f, 22.0f, 24.0f, 23.0f, 24.0f, 24.0f, 24.0f, 25.0f, 24.0f, 26.0f, 24.0f, 27.0f, 24.0f,
					28.0f, 24.0f, 29.0f, 24.0f, 30.0f, 24.0f, 31.0f, 24.0f, 32.0f, 24.0f, 33.0f, 24.0f, 34.0f, 24.0f, 35.0f, 24.0f, 36.0f, 24.0f, 37.0f, 24.0f, 38.0f, 24.0f, 39.0f, 24.0f, 40.0f, 24.0f, 41.0f, 24.0f, 42.0f, 24.0f, 43.0f, 24.0f,
					44.0f, 24.0f, 45.0f, 24.0f, 46.0f, 24.0f, 47.0f, 24.0f
				};

				float redSpikes[94] = {
					12.0f, 1.0f, 40.0f, 1.0f, 12.0f, 2.0f, 40.0f, 2.0f, 12.0f, 3.0f, 40.0f, 3.0f, 9.0f, 4.0f, 10.0f, 4.0f, 11.0f, 4.0f, 12.0f, 4.0f, 29.0f, 4.0f, 30.0f, 4.0f, 31.0f, 4.0f, 32.0f, 4.0f, 33.0f, 4.0f, 34.0f, 4.0f,
					35.0f, 4.0f, 1.0f, 8.0f, 2.0f, 8.0f, 3.0f, 8.0f, 8.0f, 8.0f, 45.0f, 8.0f, 46.0f, 8.0f, 47.0f, 8.0f, 8.0f, 9.0f, 8.0f, 10.0f, 8.0f, 11.0f, 17.0f, 12.0f, 18.0f, 12.0f, 19.0f, 12.0f, 4.0f, 13.0f, 40.0f, 13.0f,
					4.0f, 14.0f, 40.0f, 14.0f, 4.0f, 15.0f, 40.0f, 15.0f, 40.0f, 16.0f, 20.0f, 17.0f, 20.0f, 18.0f, 20.0f, 19.0f, 1.0f, 20.0f, 2.0f, 20.0f, 3.0f, 20.0f, 20.0f, 20.0f, 21.0f, 20.0f, 22.0f, 20.0f, 23.0f, 20.0f
				};

				float blueSpikes[72] = {
					24.0f, 1.0f, 24.0f, 2.0f, 24.0f, 3.0f, 24.0f, 4.0f, 20.0f, 5.0f, 40.0f, 5.0f, 20.0f, 6.0f, 40.0f, 6.0f, 20.0f, 7.0f, 40.0f, 7.0f, 4.0f, 8.0f, 20.0f, 8.0f, 4.0f, 9.0f, 4.0f, 10.0f, 4.0f, 11.0f, 4.0f, 12.0f,
					36.0f, 12.0f, 36.0f, 13.0f, 36.0f, 14.0f, 36.0f, 15.0f, 45.0f, 16.0f, 46.0f, 16.0f, 47.0f, 16.0f, 28.0f, 17.0f, 32.0f, 17.0f, 28.0f, 18.0f, 32.0f, 18.0f, 28.0f, 19.0f, 32.0f, 19.0f, 8.0f, 20.0f, 9.0f, 20.0f, 10.0f, 20.0f,
					11.0f, 20.0f, 12.0f, 20.0f, 28.0f, 20.0f, 32.0f, 20.0f
				};

				float chipsSpawns[16] = {
					10.0f, 2.0f, 30.0f, 2.0f, 2.0f, 6.0f, 42.0f, 6.0f, 18.0f, 10.0f, 26.0f, 10.0f, 6.0f, 18.0f, 38.0f, 18.0f
				};

				float powerups[16] = {
					6.0f, 2.0f, 22.0f, 2.0f, 46.0f, 2.0f, 10.0f, 10.0f, 2.0f, 14.0f, 30.0f, 14.0f, 18.0f, 18.0f, 42.0f, 18.0f
				};

				AddStaticSpikes(staticSpikes, 84);
				AddRedSpikes(redSpikes, 47, redDimension);
				AddBlueSpikes(blueSpikes, 36, blueDimension);
				AddChips(chipsSpawns, 8);
				AddPowerups(powerups, 8);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(600.0f, -1500.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(4200.0f, -1500.0f));

				break;
			}
			case Map::MediumBoy:
			{
				float staticSpikes[60] = {
					1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f, 13.0f, 31.0f, 14.0f, 31.0f, 15.0f, 31.0f, 16.0f, 31.0f,
					17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f, 29.0f, 31.0f, 30.0f, 31.0f
				};

				float redSpikes[66] = {
					26.0f, 8.0f, 27.0f, 8.0f, 28.0f, 8.0f, 29.0f, 8.0f, 30.0f, 8.0f, 1.0f, 13.0f, 2.0f, 13.0f, 3.0f, 13.0f, 4.0f, 13.0f, 5.0f, 13.0f, 30.0f, 14.0f, 30.0f, 15.0f, 30.0f, 16.0f, 30.0f, 17.0f, 14.0f, 18.0f, 15.0f, 18.0f,
					30.0f, 18.0f, 30.0f, 19.0f, 30.0f, 20.0f, 28.0f, 21.0f, 29.0f, 21.0f, 30.0f, 21.0f, 30.0f, 22.0f, 16.0f, 23.0f, 17.0f, 23.0f, 30.0f, 23.0f, 30.0f, 24.0f, 30.0f, 25.0f, 30.0f, 26.0f, 30.0f, 27.0f, 30.0f, 28.0f, 30.0f, 29.0f,
					30.0f, 30.0f
				};

				float blueSpikes[66] = {
					1.0f, 8.0f, 2.0f, 8.0f, 3.0f, 8.0f, 4.0f, 8.0f, 5.0f, 8.0f, 26.0f, 13.0f, 27.0f, 13.0f, 28.0f, 13.0f, 29.0f, 13.0f, 30.0f, 13.0f, 1.0f, 14.0f, 1.0f, 15.0f, 1.0f, 16.0f, 1.0f, 17.0f, 1.0f, 18.0f, 16.0f, 18.0f,
					17.0f, 18.0f, 1.0f, 19.0f, 1.0f, 20.0f, 1.0f, 21.0f, 2.0f, 21.0f, 3.0f, 21.0f, 1.0f, 22.0f, 1.0f, 23.0f, 14.0f, 23.0f, 15.0f, 23.0f, 1.0f, 24.0f, 1.0f, 25.0f, 1.0f, 26.0f, 1.0f, 27.0f, 1.0f, 28.0f, 1.0f, 29.0f,
					1.0f, 30.0f
				};

				float chipsSpawns[16] = {
					4.0f, 6.0f, 27.0f, 6.0f, 2.0f, 16.0f, 29.0f, 16.0f, 10.0f, 21.0f, 21.0f, 21.0f, 2.0f, 28.0f, 29.0f, 28.0f
				};

				float powerups[10] = {
					9.0f, 12.0f, 22.0f, 12.0f, 15.5f, 16.0f, 11.0f, 29.0f, 20.0f, 29.0f
				};

				AddStaticSpikes(staticSpikes, 30);
				AddRedSpikes(redSpikes, 33, redDimension);
				AddBlueSpikes(blueSpikes, 33, blueDimension);
				AddChips(chipsSpawns, 8);
				AddPowerups(powerups, 5);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(900.0f, -300.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(2200.0f, -200.0f));

				break;
			}
			case Map::Channels:
			{
				float staticSpikes[68] = {
					10.0f, 9.0f, 21.0f, 9.0f, 5.0f, 17.0f, 26.0f, 17.0f, 1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f,
					13.0f, 31.0f, 14.0f, 31.0f, 15.0f, 31.0f, 16.0f, 31.0f, 17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f,
					29.0f, 31.0f, 30.0f, 31.0f
				};

				float redSpikes[42] = {
					22.0f, 8.0f, 22.0f, 9.0f, 22.0f, 10.0f, 22.0f, 11.0f, 11.0f, 12.0f, 12.0f, 12.0f, 22.0f, 12.0f, 11.0f, 13.0f, 12.0f, 13.0f, 22.0f, 13.0f, 22.0f, 14.0f, 22.0f, 15.0f, 22.0f, 16.0f, 22.0f, 17.0f, 22.0f, 18.0f, 22.0f, 19.0f,
					22.0f, 20.0f, 22.0f, 21.0f, 22.0f, 22.0f, 22.0f, 23.0f, 22.0f, 24.0f
				};

				float blueSpikes[42] = {
					9.0f, 8.0f, 9.0f, 9.0f, 9.0f, 10.0f, 9.0f, 11.0f, 9.0f, 12.0f, 19.0f, 12.0f, 20.0f, 12.0f, 9.0f, 13.0f, 19.0f, 13.0f, 20.0f, 13.0f, 9.0f, 14.0f, 9.0f, 15.0f, 9.0f, 16.0f, 9.0f, 17.0f, 9.0f, 18.0f, 9.0f, 19.0f,
					9.0f, 20.0f, 9.0f, 21.0f, 9.0f, 22.0f, 9.0f, 23.0f, 9.0f, 24.0f
				};

				float chipsSpawns[8] = {
					6.0f, 6.0f, 25.0f, 6.0f, 4.0f, 21.0f, 27.0f, 21.0f
				};

				float powerups[12] = {
					1.0f, 16.0f, 13.0f, 16.0f, 18.0f, 16.0f, 30.0f, 16.0f, 3.0f, 28.0f, 28.0f, 28.0f
				};

				AddStaticSpikes(staticSpikes, 34);
				AddRedSpikes(redSpikes, 21, redDimension);
				AddBlueSpikes(blueSpikes, 21, blueDimension);
				AddChips(chipsSpawns, 4);
				AddPowerups(powerups, 6);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(600.0f, -2600.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(2300.0f, -2700.0f));

				break;
			}
			case Map::Separation:
			{
				float staticSpikes[64] = {
					15.0f, 1.0f, 16.0f, 1.0f, 1.0f, 31.0f, 2.0f, 31.0f, 3.0f, 31.0f, 4.0f, 31.0f, 5.0f, 31.0f, 6.0f, 31.0f, 7.0f, 31.0f, 8.0f, 31.0f, 9.0f, 31.0f, 10.0f, 31.0f, 11.0f, 31.0f, 12.0f, 31.0f, 13.0f, 31.0f, 14.0f, 31.0f,
					15.0f, 31.0f, 16.0f, 31.0f, 17.0f, 31.0f, 18.0f, 31.0f, 19.0f, 31.0f, 20.0f, 31.0f, 21.0f, 31.0f, 22.0f, 31.0f, 23.0f, 31.0f, 24.0f, 31.0f, 25.0f, 31.0f, 26.0f, 31.0f, 27.0f, 31.0f, 28.0f, 31.0f, 29.0f, 31.0f, 30.0f, 31.0f
				};

				float redSpikes[16] = {
					10.0f, 15.0f, 11.0f, 15.0f, 20.0f, 15.0f, 21.0f, 15.0f, 10.0f, 16.0f, 11.0f, 16.0f, 20.0f, 16.0f, 21.0f, 16.0f
				};

				float blueSpikes[32] = {
					3.0f, 15.0f, 28.0f, 15.0f, 3.0f, 16.0f, 28.0f, 16.0f, 3.0f, 17.0f, 28.0f, 17.0f, 3.0f, 18.0f, 28.0f, 18.0f, 3.0f, 21.0f, 28.0f, 21.0f, 3.0f, 22.0f, 28.0f, 22.0f, 3.0f, 23.0f, 28.0f, 23.0f, 3.0f, 24.0f, 28.0f, 24.0f
				};

				float chipsSpawns[8] = {
					11.0f, 11.0f, 20.0f, 11.0f, 5.0f, 20.0f, 26.0f, 20.0f
				};

				float powerups[12] = {
					3.0f, 8.0f, 28.0f, 8.0f, 14.0f, 16.0f, 17.0f, 16.0f, 1.0f, 29.0f, 30.0f, 29.0f
				};

				AddStaticSpikes(staticSpikes, 32);
				AddRedSpikes(redSpikes, 8, redDimension);
				AddBlueSpikes(blueSpikes, 16, blueDimension);
				AddChips(chipsSpawns, 4);
				AddPowerups(powerups, 6);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(300.0f, -200.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(2800.0f, -200.0f));

				break;
			}
			case Map::Descent:
			{
				float staticSpikes[44] = {
					1.0f, 47.0f, 2.0f, 47.0f, 3.0f, 47.0f, 4.0f, 47.0f, 5.0f, 47.0f, 6.0f, 47.0f, 7.0f, 47.0f, 8.0f, 47.0f, 9.0f, 47.0f, 10.0f, 47.0f, 11.0f, 47.0f, 12.0f, 47.0f, 13.0f, 47.0f, 14.0f, 47.0f, 15.0f, 47.0f, 16.0f, 47.0f,
					17.0f, 47.0f, 18.0f, 47.0f, 19.0f, 47.0f, 20.0f, 47.0f, 21.0f, 47.0f, 22.0f, 47.0f
				};

				float chipsSpawns[18] = {
					17.0f, 7.0f, 21.0f, 10.0f, 6.0f, 12.0f, 5.0f, 21.0f, 18.0f, 21.0f, 13.0f, 27.0f, 14.0f, 32.0f, 9.0f, 43.0f, 20.0f, 43.0f
				};

				float powerups[12] = {
					19.0f, 14.0f, 3.0f, 16.0f, 20.0f, 30.0f, 2.0f, 32.0f, 18.0f, 38.0f, 5.0f, 42.0f
				};

				AddStaticSpikes(staticSpikes, 22);
				AddChips(chipsSpawns, 8);
				AddPowerups(powerups, 6);

				static_cast<Transform*>(player->GetComponent("Transform"))->SetTranslation(Vector2D(500.0f, -400.0f));
				static_cast<Transform*>(player2->GetComponent("Transform"))->SetTranslation(Vector2D(1500.0f, -400.0f));

				break;
			}
			}

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

		GameObject* gameController = GetSpace()->GetObjectManager().GetObjectByName("GameController");
		//Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		if (firstFrame)
		{
			static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->SnapToTarget();
			firstFrame = false;
		}

		// End game if a player dies
		unsigned playerCount = objectManager.GetObjectCount("Player");
		if (playerCount == 1)
		{
			GameObject* winText = GetSpace()->GetObjectManager().GetObjectByName("Text");

			GameObject* lastPlayer = objectManager.GetObjectByName("Player");
			Behaviors::PlayerMovement* lastPlayerMovement = static_cast<Behaviors::PlayerMovement*>(lastPlayer->GetComponent("PlayerMovement"));

			// Set text to winText
			SpriteText* spriteText = static_cast<SpriteText*>(winText->GetComponent("SpriteText"));
			switch (lastPlayerMovement->GetPlayerID())
			{
			case 1:
				spriteText->SetText("Ninja Monkey won! Press <SPACE> to return to level select");
				break;
			case 2:
				spriteText->SetText("Cat Fighter won! Press <SPACE> to return to level select");
				break;
			}

			// Text follows camera
			static_cast<Transform*>(winText->GetComponent("Transform"))
				->SetTranslation(Graphics::GetInstance().GetCurrentCamera().GetTranslation());

			// Restart on <SPACE>
			if (input.CheckTriggered(' '))
			{
				GetSpace()->SetLevel(new Levels::LevelSelect());
			}
		}
		else if (playerCount <= 0)
		{
			GetSpace()->SetLevel(new Levels::LevelSelect());
		}
		else if (map == Map::Tutorial)
		{
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
		delete spriteSourceSpikes;
		delete textureSpikes;
		delete meshSpikes;
		delete spriteSourceCollectible;
		delete textureCollectible;
		delete textureJumpBoost;
		delete textureSpeedBoost;
		delete spriteSourceJumpBoost;
		delete spriteSourceSpeedBoost;
		delete spriteSourceCat;
		delete textureCat;
		delete meshCat;
		delete spriteSourceMonkey;
		delete textureMonkey;
		delete meshMonkey;
		delete meshGenericQuad;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Adds static spikes to the level.
	// Params:
	//   spikes = An array of floats, each pair being a coordinate.
	//   numSpikes = How many spikes are in the array.
	void Level1::AddStaticSpikes(const float* spikes, int numSpikes)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		for (int i = 0; i < numSpikes * 2; i += 2)
		{
			GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("StaticSpike"));
			static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(spikes[i] * 100.0f, spikes[i + 1] * -100.0f));
			objectManager.AddObject(*spike);
		}
	}

	// Adds red spikes to the level.
	// Params:
	//   spikes = An array of floats, each pair being a coordinate.
	//   numSpikes = How many spikes are in the array.
	//   redDimension = The ID of the red dimension.
	void Level1::AddRedSpikes(const float* spikes, int numSpikes, unsigned redDimension)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		GameObject* gameController = objectManager.GetObjectByName("GameController");
		Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));

		for (int i = 0; i < numSpikes * 2; i += 2)
		{
			GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("RedSpike"));
			static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(spikes[i] * 100.0f, spikes[i + 1] * -100.0f));
			dimensionController.AddSpikeToDimension(redDimension, spike);
			objectManager.AddObject(*spike);
		}
	}

	// Adds blue spikes to the level.
	// Params:
	//   spikes = An array of floats, each pair being a coordinate.
	//   numSpikes = How many spikes are in the array.
	//   blueDimension = The ID of the blue dimension.
	void Level1::AddBlueSpikes(const float* spikes, int numSpikes, unsigned blueDimension)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		GameObject* gameController = objectManager.GetObjectByName("GameController");
		Behaviors::DimensionController& dimensionController = *static_cast<Behaviors::DimensionController*>(gameController->GetComponent("DimensionController"));

		for (int i = 0; i < numSpikes * 2; i += 2)
		{
			GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("BlueSpike"));
			static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(spikes[i] * 100.0f, spikes[i + 1] * -100.0f));
			dimensionController.AddSpikeToDimension(blueDimension, spike);
			objectManager.AddObject(*spike);
		}
	}

	// Adds chips to the level.
	// Params:
	//   chipsSpawns = An array of floats, each pair being a coordinate.
	//   numChis = How many chips are in the array.
	void Level1::AddChips(const float* chipsSpawns, int numChips)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		for (int i = 0; i < numChips * 2; i += 2)
		{
			GameObject* chips = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
			static_cast<Transform*>(chips->GetComponent("Transform"))->SetTranslation(Vector2D(chipsSpawns[i] * 100.0f, chipsSpawns[i + 1] * -100.0f));
			objectManager.AddObject(*chips);
		}
	}

	// Adds powerups to the level.
	// Params:
	//   powerups = An array of floats, each pair being a coordinate.
	//   numPowerups = How many powerups are in the array.
	void Level1::AddPowerups(const float* powerups, int numPowerups)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		for (int i = 0; i < numPowerups * 2; i += 2)
		{
			GameObject* powerup = new GameObject(*objectManager.GetArchetypeByName(RandomRange(0, 1) ? "JumpBoost" : "SpeedBoost"));
			static_cast<Transform*>(powerup->GetComponent("Transform"))->SetTranslation(Vector2D(powerups[i] * 100.0f, powerups[i + 1] * -100.0f));
			objectManager.AddObject(*powerup);
		}
	}
}

//----------------------------------------------------------------------------
