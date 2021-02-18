#include "Animations.h"

void Animation::PlayAnimation(bool loop)
{
	time += 0.01;
	if (time < start_time)
		time = start_time;

	if (time > end_time)
	{
		time = start_time;			
	};
	for (auto anim : anims) {
		anim.UpdateKeyframeTransform(time);
	}
}

