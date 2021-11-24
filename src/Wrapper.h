#pragma once
#include "shader\Shad.h"
class Wrapper
{
public:

	virtual void Draw() = 0;
	virtual void Draw(Shader& shader) = 0;
};

