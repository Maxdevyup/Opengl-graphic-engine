#include "Chunkgen.h"
#include <glad/glad.h>
#include <cstddef>
#include <vector>
#include "FastNoiseLite.h"
#include <cstdlib>
#include <ctime>

Chunkgen::Chunkgen() = default;

Chunkgen::~Chunkgen() = default;

void Chunkgen::loadvertices()
{


	vertexcube baseCube[24] =
	{
		//face (south)
		//   x     y       z     r    g    b      u     v
		{ -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f },
		{  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f },
		{  0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.5f, 1.0f },

		//face (north)
		{ -0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.5f, 0.5f },
		{  0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.5f },
		{  0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.5f, 1.0f },

		//face left (west)
		{ -0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.5f },
		{ -0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.5f, 1.0f },

		//face right (east)
		{  0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.5f, 0.5f },
		{  0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.5f },
		{  0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{  0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.5f, 1.0f },

		//face down
		{ -0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.5f, 0.5f },
		{  0.5f, -0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.5f },
		{  0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.5f, 1.0f },

		//face up
		{  0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 0.5f, 0.5f },
		{ -0.5f,  0.5f,  0.0f, 0.83f, 0.70f, 0.44f, 1.0f, 0.5f },
		{  0.5f,  0.5f, -1.0f, 0.83f, 0.70f, 0.44f, 0.5f, 1.0f },
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
	//noise setup
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.02f);
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	noise.SetSeed(rand());
	//list vertices et indices
	std::vector<vertexcube> vertices;
	std::vector<triangles> indices;
	//generation parameter
	GLuint offset = 0;
	GLuint positionx = 0;
	GLuint positiony = 0;
	GLuint positionz = 0;
	int MaxYNoise = 10;
	int genchunkz = 0;
	int offsetx = 0;
	int offsetz = 0;
	//CHUNK generation for loop 
	for (int n = 0;n < Chunknbr;n++)
	{
		for (int s = 0; s < Chunknbr;s++)
		{
			for (int z = 0;z < Chunksize;z++)
			{
				for (int x = 0;x < Chunksize;x++)
				{
					float Chunkx = static_cast<float>(positionx + offsetx);
					float Chunky = static_cast<float>(positiony);
					float Chunkz = static_cast<float>(positionz + offsetz);
					for (int y = 0;y < 1 + static_cast<GLuint>(std::round((noise.GetNoise(Chunkx, Chunkz) + 1) / 2 * MaxYNoise));y++)
					{

						for (int i = 0; i < 24; i++)
						{
							vertexcube cube = baseCube[i];
							cube.x += positionx + offsetx;
							cube.y += positiony;
							cube.z += positionz + offsetz;
							if (positiony < 1 + (std::round((noise.GetNoise(Chunkx, Chunkz) + 1) / 2 * MaxYNoise) - 3))
							{
								cube.u -= 0.5f;
							}
							if (positiony > 1 + (std::round((noise.GetNoise(Chunkx, Chunkz) + 1) / 2 * MaxYNoise) - 2))
							{
								cube.v -= 0.5f;
							}

							vertices.push_back(cube);

						}
						if (true)
						{
							for (int i = 0; i < 12;i++)
							{
								triangles cube = baseCubetriangle[i];
								cube.a += offset;
								cube.b += offset;
								cube.c += offset;
								indices.push_back(cube);
							}
						}
						positiony += 1;
						offset += 24;
					}
					positionx += 1;
					positiony = 0;
				}
				positionx = 0;
				positiony = 0;
				positionz += 1;
			}
			positionz = 0;
			offsetx += Chunksize;
		}
		offsetz += Chunksize;
		offsetx = 0;
	}

	//loading vertices
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
//draw
 void Chunkgen::Draw()
{
	m_vao.Bind();
	glDrawElements(GL_TRIANGLES, m_indexcount, GL_UNSIGNED_INT, nullptr);
	m_vao.Unbind();
}