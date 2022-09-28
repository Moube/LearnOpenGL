#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Core
{
	class ShaderProgram;
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
	};
	
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> _vertices,
			std::vector<unsigned int> _indices,
			std::vector<Texture> _textures);

		void Draw(const ShaderProgram& program);
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

	private:
		unsigned int VAO, VBO, EBO;

		void setupMesh();
	};
}


