#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Bone/Bone.h"
#include "../Animations/Animations.h"
using namespace std;
class Skeleton
{
public:
	std::vector<Bone> bones;
	std::vector<glm::mat4> boneMats;
	float time;

	float start_time;
	float end_time;

	Animation* active_animation;
	Animation* idle_animation;

	bool anim_play;
	bool anim_loop;

	Skeleton()
	{
		time = start_time = end_time = 0;
		active_animation = nullptr;

		anim_loop = false;
	};
	Skeleton(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform);
	void Init(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform);
	//This next function is pretty self-explanatory...
	Bone* FindBone(std::string name);
	//This one isn't really...
	void UpdateBoneMatsVector();
	void Update();

};

