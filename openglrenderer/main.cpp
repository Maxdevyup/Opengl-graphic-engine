#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderClass.h"
#include "Camera.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include<stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Chunkgen.h"



int WindowWidth = 1280;
int WindowHeight = 720;





int main() 
{

	//std::cout << "Set window height ? \n";
	//std::cin >> WindowWidth;
	//std::cout << "Set window length ? \n";
	//std::cin >> WindowHeight;
	//glfw info and init
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//MSAA init
	// unbale to use for to reson, performance (no delta time so moving is very slow) and atlas texture making it glith, i will test fxaa to fix the 1st one 
	//glfwWindowHint(GLFW_SAMPLES, 4);
	//create a window
	GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "window", NULL, NULL);
	//check error
	if (window == NULL)
	{
		std::cout << "an error as occured in the window creation ";
		glfwTerminate();
		return -1;

	}
	//create context
	glfwMakeContextCurrent(window);
	//initializing window
	gladLoadGL();
	//MSAA ENABLED
	glEnable(GL_MULTISAMPLE);
	//load chunkgen constructor after OpenGL is initialized
	Chunkgen chunk;
	glViewport(0, 0, WindowWidth, WindowHeight);



	Shader shaderprogram("default.vert", "default.frag");

	// loading the texture
	int widthImg = 0, heightImg = 0, numColCh = 0;
	unsigned char* bytes = stbi_load("Atlas.png", &widthImg, &heightImg, &numColCh, 0);
	bool textureLoaded = bytes != nullptr;
	GLuint texture = 0;
	glGenTextures(1, &texture);

	if (textureLoaded)
	{
		GLenum format = numColCh == 4 ? GL_RGBA : numColCh == 3 ? GL_RGB : numColCh == 2 ? GL_RG : GL_RED;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// mipmap setting
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// need to use Clamp to avoid other texture from the atlas glitching 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//texture
		glTexImage2D(GL_TEXTURE_2D, 0, format, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cerr << "texture as failed to load : " << stbi_failure_reason() << '\n';
	}

	stbi_image_free(bytes);
	//texture sampler
	GLuint tex0Uni = glGetUniformLocation(shaderprogram.ID, "tex0");
	GLuint hasTextureUni = glGetUniformLocation(shaderprogram.ID, "hasTexture");
	shaderprogram.Activate();
	glUniform1i(tex0Uni, 0);
	glUniform1i(hasTextureUni, textureLoaded ? 1 : 0);
	
	
	//depth buffer
	glEnable(GL_DEPTH_TEST);
	//camera
	Camera camera(WindowWidth, WindowHeight, glm::vec3(0.0f, 12.0f, 105.0f));
	chunk.loadvertices();
	//Main while loop waiting the window to close
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//activate shader
		shaderprogram.Activate();
		//camera mouvement
		camera.Inputs(window);
		//camera
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderprogram, "camMatrix");
		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture);

		//drawing elements
		//glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		chunk.Draw();
		glfwSwapBuffers(window);

		glfwPollEvents();

	}
	//destroying VBO,VAO,EBO and the shaderprogram 

	glDeleteTextures(1, &texture);
	shaderprogram.Delete();
	

	//destroy window
	glfwDestroyWindow(window);
	//terminate glfw
	glfwTerminate();
	return 0;

}