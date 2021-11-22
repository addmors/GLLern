#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list> //std::list
#include <array> //std::array
#include <memory> //std::unique_ptr
#include "../Camera/Camera.h"
#include "../Model/Model.h"


class Transform
{
protected:
	//Local space information
	glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f }; //In degrees
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	glm::quat m_rotation = { 1,1,1,1 };

	//Global space informaiton concatenate in matrix
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	//Dirty flag
	bool m_isDirty = true;

protected:
	glm::mat4 getLocalModelMatrix();
	glm::mat4 getLocalModelMatrixQ();
public:

	void computeModelMatrix();
	void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);
	void computeModelMatrixQ();
	void computeModelMatrixQ(const glm::mat4& parentGlobalModelMatrix);

	void setLocalPosition(const glm::vec3& newPosition);
	void setLocalRotation(const glm::vec3& newRotation);
	void setLocalRotationQ(const glm::quat& newRotation);
	void setLocalScale(const glm::vec3& newScale);

	const glm::vec3& getGlobalPosition() const{return m_modelMatrix[3];}

	const glm::vec3& getLocalPosition() const{return m_pos;}

	const glm::vec3& getLocalRotation() const{return m_eulerRot;}

	const glm::quat& getLocalRotationQ() const { return m_rotation; }

	const glm::vec3& getLocalScale() const{return m_scale;}

	const glm::mat4& getModelMatrix() const{return m_modelMatrix;}

	glm::vec3 getRight() const{return m_modelMatrix[0];}

	glm::vec3 getUp() const{return m_modelMatrix[1];}

	glm::vec3 getBackward() const{return m_modelMatrix[2];}

	glm::vec3 getForward() const{return -m_modelMatrix[2];}

	glm::vec3 getGlobalScale() const{return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };}

	bool isDirty() const{return m_isDirty;}
};

struct Plan
{
	glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
	float     distance = 0.f;        // Distance with origin

	Plan() = default;

	Plan(const glm::vec3& p1, const glm::vec3& norm)
		: normal(glm::normalize(norm)),
		distance(glm::dot(normal, p1))
	{}

	float getSignedDistanceToPlan(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
		
	}
};

struct Frustum
{
	Plan topFace;
	Plan bottomFace;

	Plan rightFace;
	Plan leftFace;

	Plan farFace;
	Plan nearFace;
};

struct BoundingVolume
{
	virtual bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const = 0;

	virtual bool isOnOrForwardPlan(const Plan& plan) const = 0;

	bool isOnFrustum(const Frustum& camFrustum) const;
};

struct Sphere : public BoundingVolume
{
	glm::vec3 center{ 0.f, 0.f, 0.f };
	float radius{ 0.f };

	Sphere(const glm::vec3& inCenter, float inRadius)
		: BoundingVolume{}, center{ inCenter }, radius{ inRadius }
	{}

	bool isOnOrForwardPlan(const Plan& plan) const final
	{
		return plan.getSignedDistanceToPlan(center) > -radius;
	}

	bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final;
};

struct SquareAABB : public BoundingVolume
{
	glm::vec3 center{ 0.f, 0.f, 0.f };
	float extent{ 0.f };

	SquareAABB(const glm::vec3& inCenter, float inExtent)
		: BoundingVolume{}, center{ inCenter }, extent{ inExtent }
	{}

	bool isOnOrForwardPlan(const Plan& plan) const final;

	bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final;
};


struct  AABB : public BoundingVolume {
	glm::vec3 center{ 0,0,0 };
	glm::vec3 extents{ 0,0,0 };
	AABB(const glm::vec3& min, glm::vec3& max) : BoundingVolume(), center{ (min + max) * 0.5f }, extents{ max - center }
	{};
	AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
		: BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
	{};
	std::array<glm::vec3, 8> getVertice() const;
	bool isOnOrForwardPlan(const Plan& plan) const final;
	bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final;
};

Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY, float zNear, float zFar);
AABB generateAABB(const Model& model);
Sphere generateSphereBV(const Model& model);
AABB generateAABB(const PrimShape& shape);
Sphere generateSphereBV(const PrimShape& shape);


template <typename T>
class Entity
{
public:
	//Scene graph
	std::list<std::unique_ptr<Entity>> children;
	Entity* parent = nullptr;
	
	bool flag_instanse = false;
	//Space information
	Transform transform;
	T* pEntity;
	std::unique_ptr<AABB> boundingVolume;

	// constructor, expects a filepath to a 3D model.
	Entity(T& model) : pEntity{ &model }
	{
		boundingVolume = std::make_unique<AABB>(generateAABB(&pEntity));
		//boundingVolume = std::make_unique<Sphere>(generateSphereBV(model));
	};

	//TODO
	//Entity(T& model, btRigitBody body): 
	AABB getGlobalAABB()
	{
		//Get global scale thanks to our transform
		const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(boundingVolume->center, 1.f) };

		// Scaled orientation
		const glm::vec3 right = transform.getRight() * boundingVolume->extents.x;
		const glm::vec3 up = transform.getUp() * boundingVolume->extents.y;
		const glm::vec3 forward = transform.getForward() * boundingVolume->extents.z;

		const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

		const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

		const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

		return AABB(globalCenter, newIi, newIj, newIk);
	}

	//Add child. Argument input is argument of any constructor that you create. By default you can use the default constructor and don't put argument input.
	template<typename... TArgs>
	void addChild(TArgs&... args)
	{
		children.emplace_back(std::make_unique<Entity>(args...));
		children.back()->parent = this;
	}

	//Update transform if it was changed
	void updateSelfAndChild()
	{
		if (!transform.isDirty())
			return;

		forceUpdateSelfAndChild();
	}

	//Force update of transform even if local space don't change
	void forceUpdateSelfAndChild()
	{
		if (parent)
			transform.computeModelMatrix(parent->transform.getModelMatrix());
		else
			transform.computeModelMatrix();

		for (auto&& child : children)
		{
			child->forceUpdateSelfAndChild();
		}
	}


	void drawSelfAndChild(const Frustum& frustum, Shader& ourShader, unsigned int& display, unsigned int& total)
	{
		if (boundingVolume->isOnFrustum(frustum, transform))
		{
			ourShader.SetMat4("model", transform.getModelMatrix());
			pEntity->Draw();
			display++;
		}
		total++;

		for (auto&& child : children)
		{
			child->drawSelfAndChild(frustum, ourShader, display, total);
		}
	}
};

