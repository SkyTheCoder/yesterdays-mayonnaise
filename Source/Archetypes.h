//------------------------------------------------------------------------------
//
// File Name:	Archetypes.h
// Author(s):	David Cohen (david.cohen), A.J. Bussman (anthony.bussman), Daniel Walther (daniel.walther)
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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class GameObject;
class SpriteSource;
class Mesh;
class Tilemap;
class SpriteText;

namespace Archetypes
{
	//------------------------------------------------------------------------------
	// Public Typedefs:
	//------------------------------------------------------------------------------

	typedef GameObject* (*ItemFunc)(Mesh*, SpriteSource*);

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------
	
	// Create the monkey game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreatePlayer(Mesh* mesh, SpriteSource* spriteSource);

	// Create a tilemap object
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	//   map = The map to use for tilemap related components.
	// Returns:
	//   A pointer to the newly constructed game object.
	GameObject* CreateTilemapObject(Mesh* mesh, SpriteSource* spriteSource, Tilemap* map);

	// Create a text game object.
	// Returns:
	//	 A pointer to the newly constructed game object.
	GameObject* CreateText();

	// Create a collectible archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateCollectibleArchetype(Mesh* mesh, SpriteSource* spriteSource);

	// Create a game controller object.
	// Returns;
	//	 A pointer to the newly constructed game object.
	GameObject* CreateGameController();

	// Create a hazard archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateHazardArchetype(std::string name, Mesh* mesh, SpriteSource* spriteSource, unsigned frame);

	// Create a hazard archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateJumpBoostArchetype(Mesh* mesh, SpriteSource* spriteSource);

	// Create a speedBoost archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateSpeedBoostArchetype(Mesh* mesh, SpriteSource* spriteSource);

	// Create a button archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateButtonArchetype(Mesh* mesh, SpriteSource* spriteSource);

	// Create a fullscreen image archetype
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	// Returns:
	//   A pointer to the newly constructed game archetype.
	GameObject* CreateFullScreenImageArchetype(Mesh* mesh, SpriteSource* spriteSource);
}

//----------------------------------------------------------------------------
