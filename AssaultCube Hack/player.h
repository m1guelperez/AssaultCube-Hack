#pragma once
#include "aimbot.h"

namespace player {

	// Everything is public such that we can access them instantly without an additional getter function call 
	// Player class to toggle individual functionalities and keep track of them.
	class Player
	{
	public:
		Player();
		bool aimbotToggle;
		bool noRecoilToggle;
		bool unlimitedAmmo;
		bool unlimitedGrenades;
	};
}