#include "BRDF.h"

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
