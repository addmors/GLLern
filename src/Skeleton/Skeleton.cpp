#include "Skeleton.h"

Skeleton::Skeleton(std::vector<Bone*> in_bones, glm::mat4 in_globalInverseTransform) {
	Init(in_bones, in_globalInverseTransform);
}

void Skeleton::Init(std::vector<Bone*>& in_bones, glm::mat4 in_globalInverseTransform) {
	std::copy(in_bones.begin(), in_bones.end(),
		std::back_inserter(this->bones));
}
void Skeleton::Init(std::vector<Bone>& in_bones, glm::mat4 in_globalInverseTransform) {
	for (auto& a : in_bones)
		bones.push_back(&a);
}


Bone* Skeleton::FindBone(std::string name) {
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i]->name == name)
			return bones[i];
	}
	return nullptr;
}
void Skeleton::UpdateBoneMatsVector() {
	if (bones.size() == 0)
		return;

	boneMats.clear();

	for (int i = 0; i < 100; i++)
	{
		if (i > bones.size() - 1) boneMats.push_back(glm::mat4(1.0));
		else boneMats.push_back(bones[i]->nowTransformation);
	}
}
void Skeleton::Update() {
	for (auto a : bones) {
		a->getTransform();
	}
}