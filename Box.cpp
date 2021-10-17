#include "Box.h"

Box::Box(const FPoint3& min, const FPoint3& max)
	:m_Min{ min }
	, m_Max{ max }
{

};
Box::Box()
{
    m_Min = { 0,0,0 };
    m_Max = { 0,0,0 };

};

void Swap(float& a, float& b)
{
    float c = a;
    a = b;
    b = c;
}

bool Box::Hit(const Ray& ray, HitRecord& hitrecord) const
{
        float tmin = (m_Min.x - ray.origin.x) / ray.direction.x;
        float tmax = (m_Max.x - ray.origin.x) / ray.direction.x;

        if (tmin > tmax) Swap (tmin, tmax);

        float tymin = (m_Min.y - ray.origin.y) / ray.direction.y;
        float tymax = (m_Max.y - ray.origin.y) / ray.direction.y;

        if (tymin > tymax) Swap (tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (m_Min.z - ray.origin.z) / ray.direction.z;
        float tzmax = (m_Max.z - ray.origin.z) / ray.direction.z;

        if (tzmin > tzmax) Swap (tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;

       
        return true;
    
}


void Box::SetMaxMin(const FPoint3& min, const FPoint3& max)
{
	m_Max = max;
	m_Min = min;
}