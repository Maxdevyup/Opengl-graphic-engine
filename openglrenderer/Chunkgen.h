#pragma once
#include <glad/glad.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class Chunkgen 
{
	
	public:
		Chunkgen();
		~Chunkgen();
		void loadvertices();
		void Draw();
	private:
		GLsizei m_indexcount = 0;
		 VAO m_vao;
		 int Chunksize = 16;
		 int Chunknbr = 16;
		 struct vertexcube
		 {
			 float x, y, z;
			 float r, g, b;
			 float u, v;
		 };
		 struct triangles
		 {
			 GLuint a, b, c;
		 };

};