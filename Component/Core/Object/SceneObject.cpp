#include "SceneObject.h"

namespace Core
{

	SceneObject::SceneObject(glm::vec3 position)
		: Position(position)
	{
		
	}

	SceneObject::SceneObject(float posX, float posY, float posZ)
		: Position(glm::vec3(posX, posY, posZ))
	{
		
	}

	void SceneObject::Tick(float delta)
	{

	}

}