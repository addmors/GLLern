#pragma once

#include <glm/glm.hpp>
#include <string>
#include "../AniNode/AnimNode.h"
#include<vector>

using namespace std;
static float TimeToFrame(float time)
{
	float frame = 0.041666666667;
	return time / frame;
}

static glm::vec2 FramesToTime(glm::vec2 frames)
{
	float frame = 0.041666666667;
	return frames * frame;
}

class Animation
{
public:
	vector<AnimNode> anims;
	Animation() : start_time(0), end_time(0), priority(0), name("Untitled"), time(0){};
	Animation(std::string in_name, glm::vec2 times, int in_priority) : start_time(times.x), end_time(times.y), priority(in_priority), name(in_name),time(times.x) {};
	float start_time;
	float end_time;
	int priority;
	bool loop = false;
	std::string name;
	float time;
	void PlayAnimation(bool loop);
};

