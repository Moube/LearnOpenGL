#include <vector>
#include <string>
#include <assimp/scene.h>
#include "Mesh.h"

namespace Core
{
	class ShaderProgram;
	class Model
	{
	public:
		Model(std::string path);

		void Draw(ShaderProgram program);
	private:
		std::vector<Mesh> meshs;
		std::string directory;
		std::vector<Texture> textures_loaded;
		
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		static unsigned int TextureFromFile(const char* path, std::string directory);

	};
}


