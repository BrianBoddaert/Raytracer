#include "Triangle.h"
#include "Material.h"

using namespace Elite;

Triangle::Triangle(const FPoint3 vertices[3], Material* material,const Cullmode& cullmode, const FVector3& normal)
	: m_Cullmode{cullmode}
	, m_Normal{ normal }
{

	m_Center = (vertices[0] + vertices[1] + vertices[2]) / 3.f; //3 because a triangle consists of 3 vertices.
	m_Vertices[0] = vertices[0]; // Right
	m_Vertices[1] = vertices[1]; // Middle
	m_Vertices[2] = vertices[2]; // Left
	m_pMaterial = material;

	if (normal == FVector3(0, 0, 0))
	{
		FVector3 a = m_Vertices[2] - m_Vertices[1];
		FVector3 b = m_Vertices[0] - m_Vertices[1];
		m_Normal = GetNormalized(Cross(a, b));
	}
}


bool Triangle::Hit(const Ray& ray, HitRecord& hitrecord) const
{

	float normalReflectedOnRayDirection = Dot(m_Normal, ray.direction);

	if (abs(normalReflectedOnRayDirection) <= 0.0001)
	{
		return false;
	}
	if (m_Cullmode == Cullmode::backface && normalReflectedOnRayDirection > 0)
	{
		return false;
	}
	else if (m_Cullmode == Cullmode::frontface && normalReflectedOnRayDirection < 0)
	{
		return false;
	}

	FVector3 L = m_Center - ray.origin;
	float t = Dot(L, m_Normal) / normalReflectedOnRayDirection;

	if (t < ray.tMin || t > ray.tMax)
	{
		return false;
	}

	FPoint3 p = ray.origin + t * ray.direction;
	FVector3 edge1 = m_Vertices[1] - m_Vertices[0];
	FVector3 pointToSide1 = p - m_Vertices[0];
	FVector3 edge2 = m_Vertices[2] - m_Vertices[1];
	FVector3 pointToSide2 = p - m_Vertices[1];
	FVector3 edge3 = m_Vertices[0] - m_Vertices[2];
	FVector3 pointToSide3 = p - m_Vertices[2];

	if (Dot(m_Normal, Cross(pointToSide1, edge1)) > 0 || Dot(m_Normal, Cross(pointToSide2, edge2)) > 0 || Dot(m_Normal, Cross(pointToSide3, edge3)) > 0)
	{
		return false;
	}

	hitrecord.distance = t;
	hitrecord.hitPos = p;
	hitrecord.normal = m_Normal;
	hitrecord.pMaterial = m_pMaterial;
	return true;

};

void Triangle::Rotate(float angle)
{
	FVector3 vertex0Vector = FVector3(m_Vertices[0]);
	FVector3 vertex1Vector = FVector3(m_Vertices[1]);
	FVector3 vertex2Vector = FVector3(m_Vertices[2]);

	vertex0Vector = vertex0Vector - FVector3(m_Center);
	vertex1Vector = vertex1Vector - FVector3(m_Center);
	vertex2Vector = vertex2Vector - FVector3(m_Center);

	Elite::FMatrix4 rotation = Elite::MakeRotationY(Elite::ToRadians(angle));

	Elite::FVector4 tempV0{}, tempV1{}, tempV2{};
	tempV0 = rotation * Elite::FVector4(vertex0Vector, 1);
	tempV1 = rotation * Elite::FVector4(vertex1Vector, 1);
	tempV2 = rotation * Elite::FVector4(vertex2Vector, 1);

	vertex0Vector = FVector3(tempV0) + FVector3(m_Center);
	vertex1Vector = FVector3(tempV1) + FVector3(m_Center);
	vertex2Vector = FVector3(tempV2) + FVector3(m_Center);

	m_Vertices[0] = FPoint3(vertex0Vector);
	m_Vertices[1] = FPoint3(vertex1Vector);
	m_Vertices[2] = FPoint3(vertex2Vector);

	FVector3 m_A = vertex1Vector - vertex0Vector;
	FVector3 m_B = vertex2Vector - vertex0Vector;

	m_Normal = Elite::Cross(m_A, m_B);

}
