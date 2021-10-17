#include "Sphere.h"
#include "Material.h"

Sphere::Sphere(const FPoint3& origin, const float& radius, const RGBColor& color,Material* material)
	: m_Origin{ origin }
	, m_Radius{ radius }
{
	m_pMaterial = material;
	m_Color = color;
}

bool Sphere::Hit(const Ray& ray, HitRecord& hitrecord) const
{
	// Intersection Sphere:
	// Als Vector tussen P en Osphere == r dan is het op de grens
	// Dit kan je herschrijven als (p - Osphere) dot (p - Osphere) == r^2
	// Want Dot product met zichzelf is Squared magnitude
	// Formule veel draaien dan krijg je een kwadratische formule
	// t = (-B plusminus sqrt(B^2 - 4AC)) / (2A)
	// B^2 -4AC = Discriminant
	// < 0   ray does not intersect
	// == 0  ray intersects once
	// > 0 ray intersects twice
	FVector3 rayOToSphereO = ray.origin - m_Origin;

	float a = Dot(ray.direction, ray.direction);
	float b = Dot(2 * ray.direction, rayOToSphereO);
	float c = Dot(rayOToSphereO, rayOToSphereO) - (m_Radius * m_Radius);

	hitrecord.discriminant = b * b - 4 * a * c;
	hitrecord.pMaterial = m_pMaterial;

	/*hitrecord.distance = (-b)/()*/
	if (hitrecord.discriminant > 0)
	{
		float squareRootPart = sqrt(hitrecord.discriminant);
		hitrecord.distance = (-b - squareRootPart) / (2 * a);
		if (hitrecord.distance >= ray.tMin && hitrecord.distance <= ray.tMax) {
		//FVector3 L = m_Origin - ray.origin;
		//float tca = Dot(L, ray.direction);
		//float od = SqrMagnitude(Reject(L,ray.direction));
		//float thc = sqrt(m_Radius * m_Radius - od);
		//hitrecord.distance = tca - thc;
		hitrecord.hitPos = ray.origin + hitrecord.distance * ray.direction;
		hitrecord.normal = GetNormalized( hitrecord.hitPos - m_Origin);
		
			return true;
		}
	}

	return false;
	

};

