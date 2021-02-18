#include "Bone.h"
Bone::Bone(Mesh* in_mesh, unsigned int in_id, std::string in_name, aiMatrix4x4 in_o_mat)
{
	id = in_id;
	name = in_name;
	offset_matrix = AiToGLMMat4(in_o_mat);
	mesh = in_mesh;
	parent_bone = nullptr;
	
}

Bone::Bone(Mesh* in_mesh, unsigned int in_id, std::string in_name, glm::mat4 in_o_mat)
{
	id = in_id;
	name = in_name;
	offset_matrix = in_o_mat;
	mesh = in_mesh;
	parent_bone = nullptr;


}
glm::mat4 Bone::GetParentTransforms()
{
	aiNode* b = this->node;    //In order to recursively concatenate the transforms,
					   //we first start with this bone's parent.
	std::vector<glm::mat4> mats;    //Where we'll store the concatenated transforms.

	while (b != nullptr)    //As long as 'b' has a parent (see the end of the loop
	{                      //to avoid confusion).
		glm::mat4 tmp_mat = AiToGLMMat4(b->mTransformation); //This bone's transformation.
		mats.push_back(glm::transpose(tmp_mat));

		/*if (b->parent_bone == nullptr) {
			mats.push_back(glm::transpose(AiToGLMMat4(b->node->mParent->mTransformation)));
		}*/
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
unsigned int Bone::FindPosition(float time)
{
	for (unsigned int i = 0; i < animNode->mNumPositionKeys - 1; i++)
	{
		//If the time passed in is less than the time of the next
		//keyframe, then this is the keyframe we want!
		if (time < (float)animNode->mPositionKeys[i + 1].mTime)
			return i;
	}
	return 0;    //If we don't find anything, just return 0.
}
unsigned int Bone::FindScaling(float time)
{
	for (unsigned int i = 0; i < animNode->mNumScalingKeys - 1; i++)
	{
		//If the time passed in is less than the time of the next
		//keyframe, then this is the keyframe we want!
		if (time < (float)animNode->mScalingKeys[i + 1].mTime)
			return i;
	}
	return 0;    //If we don't find anything, just return 0.
}
unsigned int Bone::FindRotation(float time)
{
	for (unsigned int i = 0; i < animNode->mNumRotationKeys - 1; i++)
	{
		//Same as with the position.
		if (time < (float)animNode->mRotationKeys[i + 1].mTime)
			return i;
	}
	return 0;
}

glm::vec3 Bone::CalcInterpolatedPosition(float time) {
	if(animNode->mNumPositionKeys == 1)
	{
		aiVector3D assimp_val = animNode->mPositionKeys[0].mValue;
		glm::vec3 val(assimp_val.x, assimp_val.y, assimp_val.z);
		return val;
	}
	unsigned int PositionIndex = FindPosition(time);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	float DeltaTime = animNode->mPositionKeys[NextPositionIndex].mTime - animNode->mPositionKeys[PositionIndex].mTime;
	float Factor = (time - (float)animNode->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	const aiVector3D StartPosition = animNode->mPositionKeys[PositionIndex].mValue;
	const aiVector3D EndPosition = animNode->mPositionKeys[NextPositionIndex].mValue;
	glm::vec3 p1(StartPosition.x, StartPosition.y, StartPosition.z);
	glm::vec3 p2(EndPosition.x, EndPosition.y, EndPosition.z);
	glm::vec3 val = glm::mix(p1, p2, Factor);

	return val;
}

glm::vec3 Bone::CalcInterpolatedSkaling(float time) {
	if (animNode->mNumPositionKeys == 1)
	{
		aiVector3D assimp_val = animNode->mScalingKeys[0].mValue;
		glm::vec3 val(assimp_val.x, assimp_val.y, assimp_val.z);
		return val;
	}
	unsigned int SkalingIndex = FindScaling(time);
	unsigned int NextSkalingIndex = (SkalingIndex + 1);
	float DeltaTime = animNode->mScalingKeys[NextSkalingIndex].mTime - animNode->mScalingKeys[SkalingIndex].mTime;
	float Factor = (time - (float)animNode->mScalingKeys[SkalingIndex].mTime) / DeltaTime;
	const aiVector3D StartPosition = animNode->mScalingKeys[SkalingIndex].mValue;
	const aiVector3D EndPosition = animNode->mScalingKeys[NextSkalingIndex].mValue;
	glm::vec3 p1(StartPosition.x, StartPosition.y, StartPosition.z);
	glm::vec3 p2(EndPosition.x, EndPosition.y, EndPosition.z);
	glm::vec3 val = glm::mix(p1, p2, Factor);

	return val;
}
glm::quat Bone::CalcInterpolatedRotation(float time){
	if (animNode->mNumRotationKeys == 1)
	{
		aiQuaternion assimp_val = animNode->mRotationKeys[0].mValue;
		glm::quat val(assimp_val.w, assimp_val.x, assimp_val.y, assimp_val.z);
		return val;
	}

	unsigned int RotationIndex = FindRotation(time);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	float DeltaTime = animNode->mRotationKeys[NextRotationIndex].mTime - animNode->mRotationKeys[RotationIndex].mTime;
	float Factor = (time - (float)animNode->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	const aiQuaternion& StartRotationQ = animNode->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = animNode->mRotationKeys[NextRotationIndex].mValue;
	glm::quat r1(StartRotationQ.w, StartRotationQ.x, StartRotationQ.y, StartRotationQ.z);
	glm::quat r2(EndRotationQ.w, EndRotationQ.x, EndRotationQ.y, EndRotationQ.z);

	glm::quat val = glm::slerp(r1, r2, Factor);
	return val;

}
void Bone::UpdateKeyframeTransform(float time) {
	if (animNode == nullptr) {
		//node->mTransformation.Transpose();
		return;
	}
	pos = CalcInterpolatedPosition(time);
	rot = CalcInterpolatedRotation(time);
	scale = CalcInterpolatedSkaling(time);
	glm::mat4 mat;
	mat = glm::translate(mat,pos);
	mat *= glm::mat4_cast(rot);
	mat = glm::scale(mat,scale);

	node->mTransformation = GLMMat4ToAi(glm::transpose(mat));
}