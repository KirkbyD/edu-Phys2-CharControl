#pragma once

#include "GLCommon.hpp"
#include "cTransformComponent.hpp"
#include "vector"

class cInstancedTransformComponent {
private:
	std::vector<cTransformComponent*> transformComponents;
	glm::mat4* worldMatrixBufferOnCPU;
	GLuint worldMatrixBufferOnGPU;

	void initializeOnCPU();
	void updateOnCPU();

public:
	cInstancedTransformComponent();

	void addTransformInstance(cTransformComponent* instance);
	int getInstancesCount();

	void bindBuffer();
	void unbindBuffer();

	void initializeOnGPU();
	void updateOnGPU();
};