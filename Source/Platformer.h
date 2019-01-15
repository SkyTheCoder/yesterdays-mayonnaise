//------------------------------------------------------------------------------
//
// File Name:	Platformer.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"
#include "Level.h"
#include "Archetypes.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class Mesh;
class SpriteSource;
class GameObject;
class Tilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class Platformer : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Platformer.
		Platformer();

		// Load the resources associated with Platformer.
		void Load() override;

		// Initialize the memory associated with Platformer.
		void Initialize() override;

		// Update Platformer.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Platformer.
		void Unload() override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Loads an item's texture & adds its archetype.
		// Params:
		//   textureName = The name of the texture to load.
		//   texture = A reference to the texture pointer to save.
		//   spriteSource = A reference to the sprite source pointer to save.
		//   archetypeFunc = The archetype function to use when creating the object.
		void LoadItem(const std::string& textureName, Texture*& texture, SpriteSource*& spriteSource, Archetypes::ItemFunc itemFunc);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		Mesh* meshGeneric;

		// TO DO: Add necessary variables for platformer level here.
		// Monkey
		Mesh* meshMonkey;
		Texture* textureMonkey;
		SpriteSource* spriteSourceMonkey;

		unsigned columnsMonkey;
		unsigned rowsMonkey;

		// Collectible
		Texture* textureCollectible;
		SpriteSource* spriteSourceCollectible;

		// Hazard
		Texture* textureHazard;
		SpriteSource* spriteSourceHazard;

		// Enemy
		Texture* textureEnemy;
		SpriteSource* spriteSourceEnemy;

		// Tilemap
		Tilemap* dataMap;
		Texture* textureMap;
		SpriteSource* spriteSourceMap;
		Mesh* meshMap;

		unsigned columnsMap;
		unsigned rowsMap;
	};
}

//----------------------------------------------------------------------------
