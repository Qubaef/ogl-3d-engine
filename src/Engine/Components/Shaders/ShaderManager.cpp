#include "ShaderManager.h"
#include "Engine/Engine.h"

#include "App/Renderables/GuiEntityManager/EntityProperties/Vec3PropertyContinuousModifier.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterEntityMessage.h"
#include "App/Renderables/GuiEntityManager/Messages/RegisterPropertyMessage.h"

using namespace glm;

ShaderManager::ShaderManager(Engine& engine) :
	IMessanger(&engine.getMessageBus(), "ShaderManager"),
	engine(engine)
{
	initializeLights();

	sendMessage(new RegisterEntityMessage(""), "EntityManager");

	sendMessage(new RegisterPropertyMessage("ShaderManager",
		new Vec3PropertyContinuousModifier("directionalLight", -1, 1, directionalLight->getDirectionRef())),
		"EntityManager");

	// Init unchangeable uniforms
	for (const auto& shaderPair : shaderCollection)
	{
		shaderPair.second->use();
		shaderPair.second->setInt(pointLightsCountName, actualPointLightsNumber);
	}
}

void ShaderManager::initializeLights()
{
	ZoneScoped;

	// Initialize directional light
	//directionalLight = new LightDirectional(
	//	vec3(0.3f, 0.3f, 0.3f),
	//	vec3(0.7f, 0.7f, 0.7f),
	//	vec3(1.f, 1.f, 1.f),
	//	vec3(10.f, -1.f, 10.f));

	directionalLight = new LightDirectional(
		vec3(0.3f, 0.3f, 0.3f),
		vec3(0.7f, 0.7f, 0.7f),
		vec3(1.f, 1.f, 1.f),
		vec3(.5f, 1.f, .5f)
	);

	// Set the number of point lights
	int usedPointLightsNumber = 0;

	// Generate chosen amount of point lights
	actualPointLightsNumber = min(usedPointLightsNumber, engine.props.consts.maxPointLightsNumber);
	for (int i = 0; i < actualPointLightsNumber; i++)
	{
		pointLightsList.push_back(new LightPoint());
	}

	// Set properties of generated lights
	if (false)
	{
		pointLightsList[0] = new LightPoint(
			vec3(0.2f, 0.2f, 0.2f),
			vec3(0.5f, 0.5f, 0.5f),
			vec3(1.f, 1.f, 1.f),
			vec3(5.f, 2.f, 5.f),
			0.5f,
			0.09,
			0.032
		);

		pointLightsList[1] = new LightPoint(
			vec3(0.2f, 0.2f, 0.2f),
			vec3(0.5f, 0.5f, 0.5f),
			vec3(1.f, 1.f, 1.f),
			vec3(64.f, 2.f, 64.f),
			1.0f,
			0.09,
			0.032
		);
	}
}


void ShaderManager::updateLights()
{
	ZoneScoped;

	// Empty
}


void ShaderManager::setDirectionalLight(LightDirectional* directionalLight)
{
	this->directionalLight = directionalLight;
}


void ShaderManager::addPointLight(LightPoint* pointLight)
{
	this->pointLightsList.push_back(pointLight);
}


LightDirectional ShaderManager::getDirectionalLight()
{
	ZoneScoped;

	return *directionalLight;
}


std::vector<LightPoint*> ShaderManager::getPointLightsList()
{
	ZoneScoped;

	return pointLightsList;
}


void ShaderManager::registerShader(Shader& shader)
{
	ZoneScoped;

	this->shaderCollection.insert(std::pair<std::string, Shader*>(shader.name, &shader));
}


Shader* ShaderManager::getShaderByName(std::string name)
{
	if (shaderCollection.count(name) != 0)
	{
		return shaderCollection.at(name);
	}

	return nullptr;
}


void ShaderManager::updateShaders()
{
	ZoneScoped;

	for (auto shaderPair : shaderCollection)
	{
		shaderPair.second->use();
		updateDirectionalLight(shaderPair.second);
		updatePointLights(shaderPair.second);
	}
}


void ShaderManager::updateDirectionalLight(Shader* shaderPtr) const
{
	ZoneScoped;

	if (directionalLight->ifEnabled())
	{
		shaderPtr->setVec3(directionalLightName + ".ambient", directionalLight->getColorAmbient());
		shaderPtr->setVec3(directionalLightName + ".diffuse", directionalLight->getColorDiffuse());
		shaderPtr->setVec3(directionalLightName + ".specular", directionalLight->getColorSpecular());
		shaderPtr->setVec3(directionalLightName + ".direction", directionalLight->getDirectionVal());
	}
}


void ShaderManager::updatePointLights(Shader* shaderPtr) const
{
	ZoneScoped;

	for (int i = 0; i < actualPointLightsNumber; i++)
	{
		std::string current_name = pointLightsName + '[' + static_cast<char>(i + 48) + ']';

		shaderPtr->setVec3(current_name + ".ambient", pointLightsList[i]->getColorAmbient());
		shaderPtr->setVec3(current_name + ".diffuse", pointLightsList[i]->getColorDiffuse());
		shaderPtr->setVec3(current_name + ".specular", pointLightsList[i]->getColorSpecular());
		shaderPtr->setVec3(current_name + ".position", pointLightsList[i]->getPosition());
		shaderPtr->setFloat(current_name + ".constant", pointLightsList[i]->getConstant());
		shaderPtr->setFloat(current_name + ".linear", pointLightsList[i]->getLinear());
		shaderPtr->setFloat(current_name + ".quadratic", pointLightsList[i]->getQuadratic());
	}
}


void ShaderManager::updatePerFrame()
{
	ZoneScoped;

	updateLights();
	updateShaders();
}