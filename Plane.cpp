#include "Plane.h"
#include "Material.h"

using namespace Elite;

Plane::Plane(const Elite::FPoint3& origin,const Elite::FVector3& normal, const Elite::RGBColor& color, Material* material)
	: m_Origin{ origin }
	, m_Normal{ normal }

{
	//m_Normal = Elite::GetNormalized(normal);
	m_Color = color;
	m_pMaterial = material;
}

bool Plane::Hit(const Ray& ray, HitRecord& hitrecord) const
{
		// Intersection Infinite 2D plane : 
		// Point p dot NormalOfPlane = 0 dan is het op de plane want 0 is 90 degrees
		// Point p :::: Dot product werkt alleen op vectoren dus OriginVanPlan vector naar Point p\
		// t = (oplane - oray * n) / (d * n)
		// CHECK IF t is inbetween TMin and TMax
		// P = Oray + t d

		FVector3 originRayToOriginPlane{ ray.origin,m_Origin };
		float distance = Dot(m_Normal, originRayToOriginPlane) / Dot(ray.direction, m_Normal);

		if (distance < ray.tMin || distance > ray.tMax)
		{
			return false;
		}
		FPoint3 hitPos = ray.origin + distance * ray.direction;/* +t d;*/
		FVector3 VectorPToPlaneOrigin = { hitPos,m_Origin };
		hitrecord.discriminant = Dot(VectorPToPlaneOrigin, m_Normal);

		if (abs(hitrecord.discriminant) < 0.001)
		{
			hitrecord.normal = m_Normal;
			hitrecord.distance = distance;
			hitrecord.hitPos = hitPos;
			hitrecord.pMaterial = m_pMaterial;

			return true;
		}

		return false;

};