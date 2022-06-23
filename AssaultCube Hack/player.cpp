#include "player.h"
#include "aimbot.h"

namespace player {
	Player::Player() {
		this->aimbotToggle = false;
		this->noRecoilToggle = false;
		this->unlimitedAmmo = false;
		this->unlimitedGrenades = false;
	}
}