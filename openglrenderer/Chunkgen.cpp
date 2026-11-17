#include "Chunkgen.h"
#include <glad/glad.h>
#include <cstddef>
#include <vector>

Chunkgen::Chunkgen() = default;

Chunkgen::~Chunkgen() = default;

void Chunkgen::loadvertices()
{


	vertexcube baseCube[24] =
	{
		//face (south)
		//   x     y       z     r    g    b      u     v
		{ -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.0f, 1.0f },

		//face (north)
		{ -0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f },
		{  0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.0f },
		{  0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.0f, 1.0f },

		//face left (west)
		{ -0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f },
		{ -0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.0f },
		{ -0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.0f, 1.0f },

		//face right (east)
		{  0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f },
		{  0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.0f },
		{  0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{  0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.0f, 1.0f },

		//face down
		{ -0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f },
		{  0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.0f },
		{  0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.0f, 1.0f },

		//face up
		{  0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f },
		{ -0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.0f },
		{  0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
	};

	triangles baseCubetriangle[12]
	{
		{0, 1, 3},
		{1, 2, 3},
		{4, 5, 7},
		{5, 6, 7},
		{8, 9, 11},
		{9, 10, 11},
		{12, 13, 15},
		{13, 14, 15},
		{16, 17, 19},
		{17, 19, 18},
		{20, 21, 23},
		{20, 22, 23}
	};
	std::vector<vertexcube> vertices;
	std::vector<triangles> indices;
	GLuint offset = 0;
	GLuint positionx = 0;
	GLuint positiony = 0;
	GLuint positionz = 0;

	for (int z = 0;z < Chunksize;z++)
	{
		for (int y = 0;y < Chunksize;y++)
		{
			for (int x = 0;x < Chunksize;x++)
			{
				for (int i = 0; i < 24; i++)
				{
					vertexcube cube = baseCube[i];
					cube.x += positionx;
					cube.y += positiony;
					cube.z += positionz;
					vertices.push_back(cube);

				}
				for (int i = 0; i < 12;i++)
				{
					triangles cube = baseCubetriangle[i];
					cube.a += offset;
					cube.b += offset;
					cube.c += offset;
					indices.push_back(cube);
				}
				positionx += 1;
				offset += 24;
			}
			positiony += 1;
			positionx = 0;
		}
		positionx = 0;
		positiony = 0;
		positionz += 1;
	}
	m_indexcount = indices.size() * 3;
	m_vao.Bind();
	VBO VBO1((GLfloat*)vertices.data(), vertices.size() * sizeof(vertexcube));
	EBO EBO1((GLuint*)indices.data(), indices.size() * sizeof(triangles));

	// Position, color et texture coordinates
	m_vao.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(vertexcube), (void*)offsetof(vertexcube, x));
	m_vao.LinkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(vertexcube), (void*)offsetof(vertexcube, r));
	m_vao.LinkAttrib(VBO1, 2, 2, GL_FLOAT, sizeof(vertexcube), (void*)offsetof(vertexcube, u));

	m_vao.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
}
 void Chunkgen::Draw()
{
	m_vao.Bind();
	glDrawElements(GL_TRIANGLES, m_indexcount, GL_UNSIGNED_INT, nullptr);
	m_vao.Unbind();
}