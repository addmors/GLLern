#pragma once
#include "shader\Shad.h"


class Query
{
private: 
	unsigned int uiOcclusionQuery;
public:	
	Query(){
		glGenQueries(1, &uiOcclusionQuery);
	};
	void start() {
		glBeginQuery(GL_SAMPLES_PASSED, uiOcclusionQuery);
	};
	void end() {
		glEndQuery(GL_SAMPLES_PASSED);
	};
	~Query() {
		glDeleteQueries(1,&uiOcclusionQuery);
	}
	int Test() {
		int iSamplesPassed = 0;
		glGetQueryObjectiv(uiOcclusionQuery, GL_QUERY_RESULT, &iSamplesPassed);
		return iSamplesPassed;
	}
};

