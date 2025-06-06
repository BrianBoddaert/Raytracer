## Raytracer

![Spheres](https://user-images.githubusercontent.com/35961897/229355122-22c3db9c-3186-4ed8-8618-4a6f12404890.jpg)

![Bunny](https://user-images.githubusercontent.com/35961897/229355095-615aa730-fd89-4d6b-9ad7-c7ad3433f8d0.jpg)

My raytracer is made in C++ using SDL for rendering. I implemented plane-, sphere-, triangle-, and trianglemesh intersection functions. I applied Lambert reflectance for diffuse lighting and Cook-Torrance for BRDF ( same as in Unreal engine 4). Cook-Torrance applies geometry shadowing, normal distribution and fresnel. In the picture above you see three spheres with dielectric materials and three with metal materials in condescending order of roughness.

I also coded an obj file loader and attached it to my raytracer, I then ran it with a low poly stanford bunny. For these models I use phong shading instead of Cook-Torrance. I also implemented back- or frontface culling and optimized my frame rate by adding bounding boxes.

### Tracing

The formula around the pixels make it so that our camera is perspective instead of ortographic and considers FOV and the XY screen ratio.

```
void Renderer::Render()
{
	SDL_LockSurface(m_pBackBuffer);

	//Loop over all the pixels
	for (uint32_t r = 0; r < m_Height; ++r)
	{
		for (uint32_t c = 0; c < m_Width; ++c)
		{
			float x = ((2 * ((c + 0.5f) / m_Width)) - 1) * m_pCamera->GetAspectRatio() * m_pCamera->GetFOV();		//Camera space, World space
			float y = (1 - (2 * ((r + 0.5f) / m_Height))) * m_pCamera->GetFOV();

			FPoint4 XYZInPoint4Format{x,y,1,0};
			FPoint3 rayOrigin(m_pCamera->GetLookAtMatrix() * XYZInPoint4Format);
			FVector3 rayDirection = Elite::GetNormalized(rayOrigin - m_pCamera->GetPosition());
			Ray ray(m_pCamera->GetPosition(), rayDirection);

			HitRecord closestHitRecord;
			if (SceneGraph::GetInstance()->ClosestIntersection(ray, closestHitRecord))
			{
				RGBColor finalColorPixel{ 0,0,0 };
				finalColorPixel += SceneGraph::GetInstance()->DiffuseLighting(closestHitRecord);
				finalColorPixel.MaxToOne();

				m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format, Uint8(finalColorPixel.r * 255), Uint8(finalColorPixel.g * 255), Uint8(finalColorPixel.b * 255));
			}
			else
			{
				m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format, 0, 0, 0);
			}
		}
	}

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}
```

### Triangle Intersection

For triangle intersection I use the following code in which I first make a plane intersection, and then see if the intersection point is within the three points by using the power of the signed area of the cross product (Sphere and plane intersection can be found on my Github page).

```
bool Triangle::Hit(const Ray& ray, HitRecord& hitrecord) const
{
	float normalReflectedOnRayDirection = Dot(m_Normal, ray.direction);

	if (abs(normalReflectedOnRayDirection) <= 0.0001)
		return false;
	if (m_Cullmode == Cullmode::backface && normalReflectedOnRayDirection > 0)
		return false;
	if (m_Cullmode == Cullmode::frontface && normalReflectedOnRayDirection < 0)
		return false;

	FVector3 L = m_Center - ray.origin;
	float t = Dot(L, m_Normal) / normalReflectedOnRayDirection;

	if (t < ray.tMin || t > ray.tMax)
		return false;

	FPoint3 p = ray.origin + t * ray.direction;
	FVector3 edge1 = m_Vertices[1] - m_Vertices[0];
	FVector3 pointToSide1 = p - m_Vertices[0];
	FVector3 edge2 = m_Vertices[2] - m_Vertices[1];
	FVector3 pointToSide2 = p - m_Vertices[1];
	FVector3 edge3 = m_Vertices[0] - m_Vertices[2];
	FVector3 pointToSide3 = p - m_Vertices[2];

	if (Dot(m_Normal, Cross(pointToSide1, edge1)) > 0 || Dot(m_Normal, Cross(pointToSide2, edge2)) > 0 || Dot(m_Normal, Cross(pointToSide3, edge3)) > 0)
		return false;

	hitrecord.distance = t;
	hitrecord.hitPos = p;
	hitrecord.normal = m_Normal;
	hitrecord.pMaterial = m_pMaterial;
	return true;
};
```

### BRDFs

Lambert, Phong and CookTorrance.

```
RGBColor BRDF::Lambert(float reflectance, RGBColor color)
{
	return  (color * reflectance) / float(E_PI);
}

RGBColor BRDF::Phong(float reflectance, float phongExponent, const FVector3& w0, const FVector3& w1, const FVector3& normal)
{
	Elite::FVector3 reflect = w0 - 2*Dot(normal,w0)*normal;
	
	return RGBColor(reflectance, reflectance, reflectance) * pow((Dot(reflect, w1)),phongExponent);
}

RGBColor BRDF::Schlick(const FVector3& halfVector, const FVector3& viewDirection, const RGBColor& baseSurfaceReflectivity)
{
	//half vector between view direction and light direction

	return (baseSurfaceReflectivity + (RGBColor(1,1,1)- baseSurfaceReflectivity)*pow(1-Dot(halfVector, viewDirection),5));
}
float BRDF::TrowbridgeReitzGGX(const FVector3& halfVector, const FVector3& hitPointNormal,float roughness)
{
	//Halfvector between view direction and light direction
	return float(pow(roughness, 2)) / float(E_PI * pow((pow(Dot(hitPointNormal, halfVector), 2) * (roughness * roughness - 1) + 1), 2));
}

float BRDF::SchlickGGX(const FVector3& normalSurfaceHitPoint, const FVector3& vector, float roughness, bool direct)
{
	//Halfvector between view direction and light direction
	float roughnessRemapped = direct ? (pow(roughness + 1,2)) / 8 : roughness * roughness / 2;
	return (Dot(normalSurfaceHitPoint, vector)) / ((Dot(normalSurfaceHitPoint, vector))*(1- roughnessRemapped)+ roughnessRemapped);
}

RGBColor BRDF::CookTorrance(const RGBColor& F, float D, float G, const FVector3& n, const FVector3& v, const FVector3& l)
{
	return (F * D * G) / (4 * Dot(v, n) * Dot(l, n));
}

```
