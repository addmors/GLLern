#include "Skeleton.h"

Skeleton::Skeleton(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform) {
	Init(in_bones, in_globalInverseTransform);
}
void Skeleton::Init(std::vector<Bone> in_bones, glm::mat4 in_globalInverseTransform) {
	for (unsigned int i = 0; i < in_bones.size(); i++) {
		bones.push_back(in_bones[i]);
	}

}
Bone* Skeleton::FindBone(std::string name) {
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i].name == name)
			return &bones[i];
	}
	return nullptr;
}
void Skeleton::UpdateBoneMatsVector() {
	if (bones.size() == 0)
		return;

	boneMats.clear();

	for (int i = 0; i < 100; i++)
	{
		if (i > bones.size() - 1)
			boneMats.push_back(glm::mat4(1.0));
		else
		{
			boneMats.push_back(bones[i].GetParentTransforms() * bones[i].offset_matrix);
		}
	}
	int a;
}
void Skeleton::Update() {
	UpdateBoneMatsVector();
}