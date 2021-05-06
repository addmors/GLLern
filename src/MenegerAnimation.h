#pragma once
#include "Animations\Animations.h"
#include "Bone\Bone.h"
#include <iostream>

class TransitAnimation {
public:
	TransitAnimation(Animation* _New) : New(_New) {};
	Animation* New;
	bool Generate() {	
		for (auto& a : New->anims) {
			a.getNowPos();
		}
		return 1;
	}

	void Play(float Factor) {
		for (auto& a : New->anims) {
			a.node->mTransformation = GLMMat4ToAi(glm::mix(a.nowPos, a.zeroPos, Factor*10));
		}
	}
};

class MenegerAnimation
{
public:
	Animation* curAnimation;
	Animation* IdleAnimation;
	TransitAnimation trans;
	float TransTime = 0.1;
	float Time = 0;
	MenegerAnimation(Animation* Idle): curAnimation(Idle), IdleAnimation(Idle), trans(Idle){};
	void resetCur(Animation* anim) {
		if (curAnimation->name == anim->name) return;
		trans = TransitAnimation(anim);
		curAnimation = anim;
		curAnimation->time = 0;
		trans.Generate();
		Time = 0;
	};

	void Play(float delta) {
		if (Time < TransTime) {
			Time += delta;
			trans.Play(Time);
			cout << "TransPlayAnimation: " << Time << endl;
		}
		else {
			cout << "PlayAnimation: " << curAnimation->name << endl;
			if (curAnimation != nullptr)  curAnimation->PlayAnimation(delta);
		}
	}

	void transitionAnimation(Animation& anim) {
		if (curAnimation->name == anim.name) return;
			
	}
};

