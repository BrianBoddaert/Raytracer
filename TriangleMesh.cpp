#include "TriangleMesh.h"
#include <fstream>
#include <iostream>

TriangleMesh::TriangleMesh(Material* material, const Cullmode& cullmode)
	: m_Cullmode{ cullmode }
{
	m_pMaterial = material;
	m_VertexBuffer.clear();
	m_IndexBuffer.clear();

#ifdef WITCHER
	WitcherObjParser();
#endif
#ifndef WITCHER
	ObjParser();
#endif
	InitializeBoundingBox();
}

void TriangleMesh::InitializeBoundingBox()
{
	FPoint3 highestXYZ;
	FPoint3 lowestXYZ;

	for (size_t i = 0; i < m_VertexBuffer.size(); i++)
	{
		if (i == 0)
		{
			highestXYZ = *m_VertexBuffer[i];
			lowestXYZ = *m_VertexBuffer[i];
		}

		if (m_VertexBuffer[i]->x > highestXYZ.x)
		{
			highestXYZ.x = m_VertexBuffer[i]->x;
		}
		if (m_VertexBuffer[i]->y > highestXYZ.y)
		{
			highestXYZ.y = m_VertexBuffer[i]->y;
		}
		if (m_VertexBuffer[i]->z > highestXYZ.z)
		{
			highestXYZ.z = m_VertexBuffer[i]->z;
		}

		if (m_VertexBuffer[i]->x < lowestXYZ.x)
		{
			lowestXYZ.x = m_VertexBuffer[i]->x;
		}
		if (m_VertexBuffer[i]->y < lowestXYZ.y)
		{
			lowestXYZ.y = m_VertexBuffer[i]->y;
		}
		if (m_VertexBuffer[i]->z < lowestXYZ.z)
		{
			lowestXYZ.z = m_VertexBuffer[i]->z;
		}

	}

	m_BoundingBox.SetMaxMin(lowestXYZ, highestXYZ);
}

TriangleMesh::~TriangleMesh()
{
	for (size_t i = 0; i < m_IndexBuffer.size(); i++)
	{
		delete m_IndexBuffer[i];
	}
	for (size_t i = 0; i < m_VertexBuffer.size(); i++)
	{
		delete m_VertexBuffer[i];
	}
	for (size_t i = 0; i < m_VertexBuffer.size(); i++)
	{
		delete m_Normals[i];
	}

}

void TriangleMesh::WitcherObjParser()
{
	std::ifstream input;

	std::string line;

	int vertexCounter{ 0 };

	input.open("../ObjFilesToLoad/sword_01__silver.obj", std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		while (std::getline(input, line, '\r'))
		{
			if ((line.find('v') != std::string::npos || line.find('f') != std::string::npos) && line.find('#') == std::string::npos)
			{
				for (size_t i = 0; i < line.size(); i++)
				{
					if (line[i] == 'v' && line[i + 1] == ' ')
					{

						std::string lineOnlyNumbers = line;
						lineOnlyNumbers.erase(lineOnlyNumbers.begin(), lineOnlyNumbers.begin() + i + 2);

						std::string FirstNumber = lineOnlyNumbers;
						size_t NextSpaceIndex = FirstNumber.find_first_of(' ');
						FirstNumber.erase(FirstNumber.begin() + NextSpaceIndex, FirstNumber.end());

						std::string SecondNumber = lineOnlyNumbers;
						NextSpaceIndex = SecondNumber.find_first_of(' ');
						SecondNumber.erase(SecondNumber.begin(), SecondNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = SecondNumber.find_first_of(' ');
						SecondNumber.erase(SecondNumber.begin() + NextSpaceIndex, SecondNumber.end());

						std::string ThirdNumber = lineOnlyNumbers;
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);

						FPoint3* vertexPointer = new FPoint3(std::stof(FirstNumber), std::stof(SecondNumber), std::stof(ThirdNumber));
						m_VertexBuffer.push_back(vertexPointer);

					}
					else if (line[i] == 'f' && line[i + 1] == ' ')
					{

						std::string lineOnlyNumbers = line;
						lineOnlyNumbers.erase(lineOnlyNumbers.begin(), lineOnlyNumbers.begin() + i + 2);

						std::string FirstNumber = lineOnlyNumbers;
						size_t NextSpaceIndex = FirstNumber.find_first_of('/');
						FirstNumber.erase(FirstNumber.begin() + NextSpaceIndex, FirstNumber.end());

						std::string SecondNumber = lineOnlyNumbers;
						NextSpaceIndex = SecondNumber.find_first_of(' ');
						SecondNumber.erase(SecondNumber.begin(), SecondNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = SecondNumber.find_first_of('/');
						SecondNumber.erase(SecondNumber.begin() + NextSpaceIndex, SecondNumber.end());

						std::string ThirdNumber = lineOnlyNumbers;
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = ThirdNumber.find_first_of('/');
						ThirdNumber.erase(ThirdNumber.begin() + NextSpaceIndex, ThirdNumber.end());

						m_IndexBuffer.push_back(new unsigned int(std::stoi(FirstNumber)));
						m_IndexBuffer.push_back(new unsigned int(std::stoi(SecondNumber)));
						m_IndexBuffer.push_back(new unsigned int(std::stoi(ThirdNumber)));


						FVector3 a = *m_VertexBuffer[*m_IndexBuffer[vertexCounter + 2] - 1] - *m_VertexBuffer[*m_IndexBuffer[vertexCounter + 1] - 1];
						FVector3 b = *m_VertexBuffer[*m_IndexBuffer[vertexCounter] - 1] - *m_VertexBuffer[*m_IndexBuffer[vertexCounter + 1] - 1];

						vertexCounter += 3;

						m_Normals.push_back(new FVector3(GetNormalized(Cross(a, b))));

					}
				}

			}


		}
		input.close();
	}
}

void TriangleMesh::ObjParser()
{
	std::ifstream input;

	std::string line;

	int vertexCounter{ 0 };

	input.open("../ObjFilesToLoad/lowpoly_bunny.obj", std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		while (std::getline(input, line, '\r'))
		{
			if ((line.find('v') != std::string::npos || line.find('f') != std::string::npos) && line.find('#') == std::string::npos)
			{
				for (size_t i = 0; i < line.size(); i++)
				{
					if (line[i] == 'v' && line[i + 1] == ' ' && line[i + 2] == ' ')
					{

						std::string lineOnlyNumbers = line;
						lineOnlyNumbers.erase(lineOnlyNumbers.begin(), lineOnlyNumbers.begin() + i + 3);

						std::string FirstNumber = lineOnlyNumbers;
						size_t NextSpaceIndex = FirstNumber.find_first_of(' ');
						FirstNumber.erase(FirstNumber.begin() + NextSpaceIndex, FirstNumber.end());

						std::string SecondNumber = lineOnlyNumbers;
						NextSpaceIndex = SecondNumber.find_first_of(' ');
						SecondNumber.erase(SecondNumber.begin(), SecondNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = SecondNumber.find_first_of(' ');
						SecondNumber.erase(SecondNumber.begin() + NextSpaceIndex, SecondNumber.end());

						std::string ThirdNumber = lineOnlyNumbers;
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);

						FPoint3* vertexPointer = new FPoint3(std::stof(FirstNumber), std::stof(SecondNumber), std::stof(ThirdNumber));
						m_VertexBuffer.push_back(vertexPointer);

					}
					else if (line[i] == 'f' && line[i + 1] == ' ')
					{
						std::string lineOnlyNumbers = line;
						lineOnlyNumbers.erase(lineOnlyNumbers.begin(), lineOnlyNumbers.begin() + i + 2);

						std::string FirstNumber = lineOnlyNumbers;
						size_t NextSpaceIndex = FirstNumber.find_first_of(' ');
						FirstNumber.erase(FirstNumber.begin() + NextSpaceIndex, FirstNumber.end());

						std::string SecondNumber = lineOnlyNumbers;
						NextSpaceIndex = SecondNumber.find_first_of(' ');
						SecondNumber.erase(SecondNumber.begin(), SecondNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = SecondNumber.find_first_of(' ');
						SecondNumber.erase(SecondNumber.begin() + NextSpaceIndex, SecondNumber.end());

						std::string ThirdNumber = lineOnlyNumbers;
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);
						NextSpaceIndex = ThirdNumber.find_first_of(' ');
						ThirdNumber.erase(ThirdNumber.begin(), ThirdNumber.begin() + NextSpaceIndex + 1);

						m_IndexBuffer.push_back(new unsigned int(std::stoi(FirstNumber)));
						m_IndexBuffer.push_back(new unsigned int(std::stoi(SecondNumber)));
						m_IndexBuffer.push_back(new unsigned int(std::stoi(ThirdNumber)));


						FVector3 a = *m_VertexBuffer[*m_IndexBuffer[vertexCounter + 2] - 1] - *m_VertexBuffer[*m_IndexBuffer[vertexCounter + 1] - 1];
						FVector3 b = *m_VertexBuffer[*m_IndexBuffer[vertexCounter] - 1] - *m_VertexBuffer[*m_IndexBuffer[vertexCounter + 1] - 1];

						vertexCounter += 3;

						m_Normals.push_back(new FVector3(GetNormalized(Cross(a, b))));

					}
				}

			}


		}
		input.close();
	}
}

bool TriangleMesh::CheckBoundingBoxIntersection(const Ray& ray, HitRecord& hitrecord) const
{
	if (m_BoundingBox.Hit(ray, hitrecord))
	{
		return true;
	}

	return false;
}

bool TriangleMesh::Hit(const Ray& ray, HitRecord& ClosestIntersectionHitrecord) const
{
	FPoint3 verticesLocation[3];

	HitRecord hitrecord;
	ClosestIntersectionHitrecord.distance = FLT_MAX;
	int normalCounter{ 0 };

	for (size_t i = 0; i < m_IndexBuffer.size(); i += 3)
	{

		verticesLocation[0] = *m_VertexBuffer[*m_IndexBuffer[i] - 1];
		verticesLocation[1] = *m_VertexBuffer[*m_IndexBuffer[i + 1] - 1];
		verticesLocation[2] = *m_VertexBuffer[*m_IndexBuffer[i + 2] - 1];

		Triangle triangle{ verticesLocation, m_pMaterial, m_Cullmode, *m_Normals[normalCounter] };

		if (triangle.Hit(ray, hitrecord))
		{
			if (ClosestIntersectionHitrecord.distance > hitrecord.distance && hitrecord.distance > 0)
			{
				ClosestIntersectionHitrecord = hitrecord;
			}
		}
		normalCounter++;
	}

	if (ClosestIntersectionHitrecord.distance != FLT_MAX)
	{
		return true;
	}

	return false;
}
