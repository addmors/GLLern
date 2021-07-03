#include "AnimNode.h"

static aiMatrix4x4 GLMMat4ToAi(glm::mat4 mat)
{
	return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}


static glm::mat4 AiToGLMMat4(aiMatrix4x4& in_mat)
{
	glm::mat4 tmp;
	tmp[0][0] = in_mat.a1;
	tmp[1][0] = in_mat.b1;
	tmp[2][0] = in_mat.c1;
	tmp[3][0] = in_mat.d1;

	tmp[0][1] = in_mat.a2;
	tmp[1][1] = in_mat.b2;
	tmp[2][1] = in_mat.c2;
	tmp[3][1] = in_mat.d2;

	tmp[0][2] = in_mat.a3;
	tmp[1][2] = in_mat.b3;
	tmp[2][2] = in_mat.c3;
	tmp[3][2] = in_mat.d3;

	tmp[0][3] = in_mat.a4;
	tmp[1][3] = in_mat.b4;
	tmp[2][3] = in_mat.c4;
	tmp[3][3] = in_mat.d4;
	return tmp;
};

unsigned int AnimNode::FindPosition(float time)
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
unsigned int AnimNode::FindScaling(float time)
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
unsigned int AnimNode::FindRotation(float time)
{
	for (unsigned int i = 0; i < animNode->mNumRotationKeys - 1; i++)
	{
		//Same as with the position.
		if (time < (float)animNode->mRotationKeys[i + 1].mTime)
			return i;
	}
	return 0;
}

aiVector3D AnimNode::CalcInterpolatedPosition(float time) {
	if (animNode->mNumPositionKeys == 1)
	{
		return  animNode->mPositionKeys[0].mValue;
	}
	unsigned int PositionIndex = FindPosition(time);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	float DeltaTime = animNode->mPositionKeys[NextPositionIndex].mTime - animNode->mPositionKeys[PositionIndex].mTime;
	float Factor = (time - (float)animNode->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	const aiVector3D StartPosition = animNode->mPositionKeys[PositionIndex].mValue;
	const aiVector3D EndPosition = animNode->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = EndPosition - StartPosition;
	return StartPosition + Factor * Delta;
}

aiVector3D AnimNode::CalcInterpolatedSkaling(float time) {
	if (animNode->mNumPositionKeys == 1)
	{
		return animNode->mScalingKeys[0].mValue;
	}
	unsigned int SkalingIndex = FindScaling(time);
	unsigned int NextSkalingIndex = (SkalingIndex + 1);
	float DeltaTime = animNode->mScalingKeys[NextSkalingIndex].mTime - animNode->mScalingKeys[SkalingIndex].mTime;
	float Factor = (time - (float)animNode->mScalingKeys[SkalingIndex].mTime) / DeltaTime;
	const aiVector3D StartPosition = animNode->mScalingKeys[SkalingIndex].mValue;
	const aiVector3D EndPosition = animNode->mScalingKeys[NextSkalingIndex].mValue;
	aiVector3D Delta = EndPosition - StartPosition;
	return StartPosition + Factor * Delta;
}
aiQuaternion AnimNode::CalcInterpolatedRotation(float time) {
	if (animNode->mNumRotationKeys == 1)
		return animNode->mRotationKeys[0].mValue;
	

	unsigned int RotationIndex = FindRotation(time);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	float DeltaTime = animNode->mRotationKeys[NextRotationIndex].mTime - animNode->mRotationKeys[RotationIndex].mTime;
	float Factor = (time - (float)animNode->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	const aiQuaternion& StartRotationQ = animNode->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = animNode->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion val;
	aiQuaternion::Interpolate(val, StartRotationQ, EndRotationQ, Factor);
	val = val.Normalize();
	return val;

}


void AnimNode::UpdateKeyframeTransform(float time) {
	if (animNode == nullptr) {
		//node->mTransformation.Transpose();
		return;
	}
	aiMatrix4x4 val;
	//Pos
	aiMatrix4x4::Translation(CalcInterpolatedPosition(time), val);
	//Rot
	val *= aiMatrix4x4(CalcInterpolatedRotation(time).GetMatrix());
	//Scale
	aiMatrix4x4 scalemat;
	val  *= aiMatrix4x4::Scaling(CalcInterpolatedSkaling(time), scalemat);

	node->mTransformation =val;
}

aiMatrix4x4 AnimNode::GetKeyframeTransform(float time) {
	if (animNode == nullptr) {
		//node->mTransformation.Transpose();
		return aiMatrix4x4();
	}
	aiMatrix4x4 val;
	//Pos
	aiMatrix4x4::Translation(CalcInterpolatedPosition(time), val);
	//Rot
	val *= aiMatrix4x4(CalcInterpolatedRotation(time).GetMatrix());
	//Scale
	aiMatrix4x4 scalemat;
	val *= aiMatrix4x4::Scaling(CalcInterpolatedSkaling(time), scalemat);
	return val;
	

}


aiMatrix4x4 AnimNode::getZeroPos()
{
	return zeroPos;
}

aiMatrix4x4 AnimNode::getNowPos()
{
	nowPos = node->mTransformation;
	return node->mTransformation;
}
