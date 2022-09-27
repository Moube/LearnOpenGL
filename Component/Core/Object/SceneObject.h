#pragma once
#include <glm/glm.hpp>

namespace Core
{
	class SceneObject
	{
	public:
		SceneObject(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
		SceneObject(float posX, float posY, float posZ);

	public:
		void Tick(float delta);

	public:
		glm::vec3 GetPosition() { return Position; }
		void SetPosition(glm::vec3 newPos) { Position = newPos; }

	private:
		glm::vec3 Position;

	private:
		//SceneObject(const SceneObject&) = delete;
		//SceneObject& operator=(const SceneObject&) = delete;
		//SceneObject(SceneObject&& other) noexcept = default;
		//SceneObject& operator=(SceneObject&& other) noexcept = default;
	};
}