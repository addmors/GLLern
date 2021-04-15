#include "Bone.h"
glm::mat4 Bone::GetParentTransforms()
{
	aiNode* b = this->node;    //In order to recursively concatenate the transforms,
					   //we first start with this bone's parent.
	std::vector<glm::mat4> mats;    //Where we'll store the concatenated transforms.

	while (b != nullptr)    //As long as 'b' has a parent (see the end of the loop
	{                      //to avoid confusion).
		mats.push_back(glm::transpose(AiToGLMMat4(b->mTransformation)));
		b = b->mParent; //We set b to its own parent so the loop can continue.
	}
	glm::mat4 concatenated_transforms;
	//IMPORTANT!!!! This next loop must be done in reverse, 
	//as multiplication with matrices is not commutative.
	for (int i = mats.size() - 1; i >= 0; i--)
		concatenated_transforms *= mats[i];
	//concatenated_transforms = glm::translate(concatenated_transforms, glm::vec3(0, 0, 3));
	return concatenated_transforms;    //Finally, we return the concatenated transforms.
}
