//------------------------------------------------------------------------------
//
// File Name:	WinLevel.h
// Author(s):	Daniel Walther (daniel.walther)
// Project:		Yesterday's Mayonnaise
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

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class WinLevel : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of WinLevel.
		WinLevel();

		// Load the resources associated with WinLevel.
		void Load() override;

		// Initialize the memory associated with WinLevel.
		void Initialize() override;

		// Update WinLevel.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with WinLevel.
		void Unload() override;
	};
}

//----------------------------------------------------------------------------
