#pragma once
#include "EMath.h"
#include <string>
#include "ERGBColor.h"
#include <vector>

class Material;

class Shape;
using namespace Elite;

struct Ray
{
	Ray(const FPoint3& origin, const FVector3& direction, float tMin = 0.0001f,float tMax = FLT_MAX);

	FPoint3 origin;
	FVector3 direction;
	float tMin;
	float tMax;
};

struct HitRecord
{
	HitRecord();
	float distance;
	float discriminant;
	FVector3 normal;
	FPoint3 hitPos;
	Material* pMaterial;
};

struct Light
{
	Light(FVector3, RGBColor, float,bool = true);
	RGBColor color;
	FVector3 position;
	FVector3 direction;
	float    intensity;
	bool     isPointLight;
	bool     active;
};


struct Scene
{
	Scene(int id2 =0);
	int id;
	std::vector<Light*> lightsInScene;
	std::vector<Shape*> shapesInScene;
};