#include <stdlib.h>
#include <Template.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/color4.h>

int main()
{
    glm::vec4 vec(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    aiColor4t<float> assimpColor4(1.0f, 1.0f, 1.0f, 1.0f);
    system("pause");
    return 0;
}