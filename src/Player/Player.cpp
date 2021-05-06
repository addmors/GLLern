#include "Player.h"

void Player::show()
{
	if (currentShader == nullptr) return;
	if (Persone == nullptr) return;
	Persone->DrawAnim(*currentShader);
}
