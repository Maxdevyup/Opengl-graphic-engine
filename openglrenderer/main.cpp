#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include<stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



int WindowH = 800;
int WindowL = 800;




int main() 
{

	//std::cout << "Set window height ? \n";
	//std::cin >> WindowH;
	//std::cout << "Set window length ? \n";
	//std::cin >> WindowL;
	//glfw info and init
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};


	//create a window
	GLFWwindow* window = glfwCreateWindow(WindowH, WindowL, "window", NULL, NULL);
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
	glViewport(0, 0, WindowH, WindowL);


	Shader shaderprogram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));

	EBO EBO1(indices, sizeof(indices));
	//link VAO

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 *sizeof(float),(void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//Unbind
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	GLuint uniID = glGetUniformLocation(shaderprogram.ID, "scale");
	//texture
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load("dirt.png", &widthImg, &heightImg, &numColCh, 0);
	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	glGenerateMipmap(GL_TEXTURE_2D);

	//clean texture image
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
	//jsp
	GLuint tex0Uni = glGetUniformLocation(shaderprogram.ID, "tex0");
	shaderprogram.Activate();
	glUniform1i(tex0Uni, 0);
	
	//rotation and rotation speed value
	float rotation = 0.0f;
	double prevtime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	//window closing condition
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//activate shader
		shaderprogram.Activate();
		//rotation schedul
		double crntTime = glfwGetTime();
		if (crntTime - prevtime >=  1.0/120.0)
		{
			rotation += 0.1f;
			prevtime = crntTime;
		}




		//glm activation
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		//model rotation
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		// view traslation
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		//??
		proj = glm::perspective(glm::radians(45.0f), (float) (WindowL / WindowH),0.1f,100.0f);
		//locate model
		int modelLoc = glGetUniformLocation(shaderprogram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//locate view
		int viewLoc = glGetUniformLocation(shaderprogram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//locate model
		int projLoc = glGetUniformLocation(shaderprogram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	
		glUniform1f(uniID, 0.5f);

		glBindTexture(GL_TEXTURE_2D, texture);

		VAO1.Bind();
		//drawing elements
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();

	}
	//destroying VBO,VAO,EBO and the shaderprogram 
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);
	shaderprogram.Delete();
	

	//destroy window
	glfwDestroyWindow(window);
	//terminate glfw
	glfwTerminate();
	return 0;

}