#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <assimp/types.h>

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
		aiString path;
	};
	
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> _vertices,
			std::vector<unsigned int> _indices,
			std::vector<Texture> _textures);

		void Draw(const ShaderProgram& program);
		void DrawFaceOnly();
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

	private:
		unsigned int VAO, VBO, EBO;

		void setupMesh();
	};
}


