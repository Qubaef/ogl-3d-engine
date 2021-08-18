#pragma once

#include "Engine/Include/Common.h"

struct QuadTerrainNode
{
private:
	// True if Node is leaf in the tree
	bool leaf;

public:
	glm::vec2 startPos;
	float size;

	// Parents of the node
	QuadTerrainNode* parent = nullptr;

	// Children of the node
	QuadTerrainNode* tl = nullptr;
	QuadTerrainNode* tr = nullptr;
	QuadTerrainNode* bl = nullptr;
	QuadTerrainNode* br = nullptr;

	QuadTerrainNode(float startPosX, float startPosZ, float size, QuadTerrainNode* parent = nullptr)
		: startPos(startPosX, startPosZ), size(size), leaf(true), parent(parent)
	{
	}

	QuadTerrainNode(glm::vec2 startPos, float size, QuadTerrainNode* parent = nullptr)
		: startPos(startPos), size(size), leaf(true), parent(parent)
	{
	}

	bool contains(glm::vec3 point)
	{
		return point.x >= startPos.x && point.x <= startPos.x + size &&
			point.z >= startPos.y && point.z <= startPos.y + size;
	}

	bool intersects(glm::vec3 point)
	{
		float d = glm::abs(distance(point, vec3(startPos.x + size * 0.5, 0, startPos.y + size * 0.5)));
		float cap = sqrt(pow(size * 0.5, 2.0) * 2.0) * 5.0;

		return d < cap;
	}

	// Check if point is in front of the camera
	bool inFieldOfView(Camera* cameraPtr)
	{
		// Get camera direction
		vec2 dir = normalize(cameraPtr->getDirection().xz());

		// Calculate direction to the middle of the sector
		//  The (dir * camPos.y) component is subtracted from camera position
		//  to move position back according to camera height
		vec2 midPoint = startPos + vec2(size / 2);
		vec3 camPos = cameraPtr->getPosition();
		vec2 pointDir = midPoint - (camPos.xz() - (dir * camPos.y));

		// Angle of Fov (TODO: connect this value with camera FOV)
		float maxAngle = 45;

		// Calculate angle between vectors
		float angle = degrees(orientedAngle(dir, normalize(pointDir)));

		return angle < maxAngle && angle > -maxAngle;
	}

	void update(vec3& referencePoint, float minSize)
	{
		ZoneScoped;

		// If node is leaf that should split and if its child will be large enough
		// if (leaf && contains(referencePoint))
		if (intersects(referencePoint))
		{
			if (size / 2 >= minSize)
			{
				if (leaf)
				{
					// Split the node
					leaf = false;

					tl = new QuadTerrainNode(startPos.x, startPos.y, size / 2, this);
					tr = new QuadTerrainNode(startPos.x + size / 2, startPos.y, size / 2, this);
					bl = new QuadTerrainNode(startPos.x, startPos.y + size / 2, size / 2, this);
					br = new QuadTerrainNode(startPos.x + size / 2, startPos.y + size / 2, size / 2, this);
				}

				// Update all the children nodes
				tl->update(referencePoint, minSize);
				tr->update(referencePoint, minSize);
				bl->update(referencePoint, minSize);
				br->update(referencePoint, minSize);
			}
		}
		else if (!leaf)
		{
			// Merge the node
			leaf = true;

			delete tl;
			delete tr;
			delete bl;
			delete br;

			tl = nullptr;
			tr = nullptr;
			bl = nullptr;
			br = nullptr;
		}
	}

	void render(Shader* shaderPtr, GLuint& nodePosId, GLuint& nodeSizeId, Engine* enginePtr)
	{
		ZoneScoped;

		if (leaf)
		{
			// Check if patch is inf front of the camera
			if (inFieldOfView(enginePtr->getCamera())) {
				// Set node's uniforms
				glUniform3f(nodePosId, startPos.x, 0, startPos.y);
				glUniform1f(nodeSizeId, size);

				// Draw node
				glDrawElements(
					GL_PATCHES,				// mode
					4,						// count
					GL_UNSIGNED_INT,		// type
					(void*)0				// element array buffer offset
				);
			}
		}
		else
		{
			// Draw all children
			tl->render(shaderPtr, nodePosId, nodeSizeId, enginePtr);
			tr->render(shaderPtr, nodePosId, nodeSizeId, enginePtr);
			bl->render(shaderPtr, nodePosId, nodeSizeId, enginePtr);
			br->render(shaderPtr, nodePosId, nodeSizeId, enginePtr);
		}
	}

	~QuadTerrainNode()
	{
		if (!leaf)
		{
			delete tl;
			delete tr;
			delete bl;
			delete br;
		}
	}
};
