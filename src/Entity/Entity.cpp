#include "Entity.h"


glm::mat4 Transform::getLocalModelMatrix()
{
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Y * X * Z
	const glm::mat4 roationMatrix = transformY * transformX * transformZ;

	// translation * rotation * scale (also know as TRS matrix)
	return glm::translate(glm::mat4(1.0f), m_pos) * roationMatrix * glm::scale(glm::mat4(1.0f), m_scale);
}

glm::mat4 Transform::getLocalModelMatrixQ()
{
	return glm::translate(glm::mat4(1.0f), m_pos) * glm::mat4_cast(m_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
}


void Transform::computeModelMatrix()
{
	m_modelMatrix = getLocalModelMatrixQ();
}

void Transform::computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
	m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
}

void Transform::computeModelMatrixQ()
{
}

void Transform::computeModelMatrixQ(const glm::mat4& parentGlobalModelMatrix)
{
	m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrixQ();
}

void Transform::setLocalPosition(const glm::vec3& newPosition)
{
	m_pos = newPosition;
	m_isDirty = true;
}

void Transform::setLocalRotation(const glm::vec3& newRotation)
{
	m_eulerRot = newRotation;
	m_isDirty = true;
}


void Transform::setLocalRotationQ(const glm::quat& newRotation) {
	m_rotation = newRotation;
	m_isDirty = false;
}
void Transform::setLocalScale(const glm::vec3& newScale)
{
	m_scale = newScale;
	m_isDirty = true;
}



bool BoundingVolume::isOnFrustum(const Frustum& camFrustum) const
{
	return (isOnOrForwardPlan(camFrustum.leftFace) &&
		isOnOrForwardPlan(camFrustum.rightFace) &&
		isOnOrForwardPlan(camFrustum.topFace) &&
		isOnOrForwardPlan(camFrustum.bottomFace) &&
		isOnOrForwardPlan(camFrustum.nearFace) &&
		isOnOrForwardPlan(camFrustum.farFace));
};

bool Sphere::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const {
	//Get global scale thanks to our transform
	const glm::vec3 globalScale = transform.getGlobalScale();

	//Get our global center with process it with the global model matrix of our transform
	const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

	//To wrap correctly our shape, we need the maximum scale scalar.
	const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

	//Max scale is assuming for the diameter. So, we need the half to apply it to our radius
	Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

	//Check Firstly the result that have the most chance to faillure to avoid to call all functions.
	return (globalSphere.isOnOrForwardPlan(camFrustum.leftFace) &&
		globalSphere.isOnOrForwardPlan(camFrustum.rightFace) &&
		globalSphere.isOnOrForwardPlan(camFrustum.farFace) &&
		globalSphere.isOnOrForwardPlan(camFrustum.nearFace) &&
		globalSphere.isOnOrForwardPlan(camFrustum.topFace) &&
		globalSphere.isOnOrForwardPlan(camFrustum.bottomFace));
}




bool SquareAABB::isOnOrForwardPlan(const Plan& plan) const 
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	const float r = extent * (std::abs(plan.normal.x) + std::abs(plan.normal.y) + std::abs(plan.normal.z));
	return -r <= plan.getSignedDistanceToPlan(center);
}

bool SquareAABB::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const 
{
	//Get global scale thanks to our transform
	const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

	// Scaled orientation
	const glm::vec3 right = transform.getRight() * extent;
	const glm::vec3 up = transform.getUp() * extent;
	const glm::vec3 forward = transform.getForward() * extent;

	const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

	const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

	const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

	const SquareAABB globalAABB(globalCenter, std::max(std::max(newIi, newIj), newIk));

	return (globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.farFace));
};


std::array<glm::vec3, 8> AABB::getVertice() const
{
	std::array<glm::vec3, 8> vertice;
	vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
	vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
	vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
	vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
	vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
	vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
	vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
	vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
	return vertice;
}
bool AABB::isOnOrForwardPlan(const Plan& plan) const 
{
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	const float r = extents.x * std::abs(plan.normal.x) + extents.y * std::abs(plan.normal.y) +
		extents.z * std::abs(plan.normal.z);

	return -r <= plan.getSignedDistanceToPlan(center);
}
bool AABB::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const
{
	//Get global scale thanks to our transform
	const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

	// Scaled orientation
	const glm::vec3 right = transform.getRight() * extents.x;
	const glm::vec3 up = transform.getUp() * extents.y;
	const glm::vec3 forward = transform.getForward() * extents.z;

	const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

	const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

	const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

	const AABB globalAABB(globalCenter, newIi, newIj, newIk);

	return (globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
		globalAABB.isOnOrForwardPlan(camFrustum.farFace));
};




Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY, float zNear, float zFar) {
	Frustum     frustum;
	const float halfVSide = zFar * tanf(fovY * .5f);
	const float halfHSide = halfVSide * aspect;
	const glm::vec3 frontMultFar = zFar * cam.cameraFront;

	frustum.nearFace = { cam.cameraPos + zNear * cam.cameraFront, cam.cameraFront };
	frustum.farFace = { cam.cameraPos + frontMultFar, -cam.cameraFront };
	frustum.rightFace = { cam.cameraPos, glm::cross(cam.cameraUp, frontMultFar + cam.cameraRight_ * halfHSide) };
	frustum.leftFace = { cam.cameraPos, glm::cross(frontMultFar - cam.cameraRight_ * halfHSide, cam.cameraUp) };
	frustum.topFace = { cam.cameraPos, glm::cross(cam.cameraRight_, frontMultFar - cam.cameraUp * halfVSide) };
	frustum.bottomFace = { cam.cameraPos, glm::cross(frontMultFar + cam.cameraUp * halfVSide, cam.cameraRight_) };

	return frustum;
};


AABB generateAABB(const Model& model)
{
	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
	for (auto&& mesh : model.meshes)
	{
		for (auto&& vertex : mesh._vertices)
		{
			minAABB.x = std::min(minAABB.x, vertex.Position.x);
			minAABB.y = std::min(minAABB.y, vertex.Position.y);
			minAABB.z = std::min(minAABB.z, vertex.Position.z);

			maxAABB.x = std::max(maxAABB.x, vertex.Position.x);
			maxAABB.y = std::max(maxAABB.y, vertex.Position.y);
			maxAABB.z = std::max(maxAABB.z, vertex.Position.z);
		}
	}
	return AABB(minAABB, maxAABB);
}

Sphere generateSphereBV(const Model& model)
{
	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
	for (auto&& mesh : model.meshes)
	{
		for (auto&& vertex : mesh._vertices)
		{
			minAABB.x = std::min(minAABB.x, vertex.Position.x);
			minAABB.y = std::min(minAABB.y, vertex.Position.y);
			minAABB.z = std::min(minAABB.z, vertex.Position.z);

			maxAABB.x = std::max(maxAABB.x, vertex.Position.x);
			maxAABB.y = std::max(maxAABB.y, vertex.Position.y);
			maxAABB.z = std::max(maxAABB.z, vertex.Position.z);
		}
	}

	return Sphere((maxAABB + minAABB) * 0.5f, glm::length(minAABB - maxAABB));
}

AABB generateAABB(const PrimShape& shape)
{

	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
	unsigned int size = shape.getVertexSize()*3;
	const float* data = shape.getVertices();
	for (int i = 0; i<size;i+=3)
	{
			minAABB.x = std::min(minAABB.x, data[i]);
			minAABB.y = std::min(minAABB.y, data[i+1]);
			minAABB.z = std::min(minAABB.z, data[i+2]);

			maxAABB.x = std::max(maxAABB.x, data[i]);
			maxAABB.y = std::max(maxAABB.y, data[i+1]);
			maxAABB.z = std::max(maxAABB.z, data[i+2]);
	}
	return AABB(minAABB, maxAABB);
}

Sphere generateSphereBV(const PrimShape& shape)
{
	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
	unsigned int size = shape.getVertexSize() * 3;
	const float* data = shape.getVertices();
	for (int i = 0; i < size; i += 3)
	{
		minAABB.x = std::min(minAABB.x, data[i]);
		minAABB.y = std::min(minAABB.y, data[i + 1]);
		minAABB.z = std::min(minAABB.z, data[i + 2]);

		maxAABB.x = std::max(maxAABB.x, data[i]);
		maxAABB.y = std::max(maxAABB.y, data[i + 1]);
		maxAABB.z = std::max(maxAABB.z, data[i + 2]);
	}
	return Sphere((maxAABB + minAABB) * 0.5f, glm::length(minAABB - maxAABB));
}



