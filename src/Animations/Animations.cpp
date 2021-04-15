#include "Animations.h"

void Animation::PlayAnimation(float delta)
{
	time += delta;
	if (time < start_time || time > end_time) time = start_time;
	for (auto anim : anims) anim.UpdateKeyframeTransform(time);
}

