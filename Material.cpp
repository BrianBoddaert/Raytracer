#include "Material.h"
#include "BRDF.h"



Material_Lambert::Material_Lambert(const RGBColor& diffuseColor, float diffuseReflectance)
	:Material{}
	, m_DiffuseColor{ diffuseColor }
	, m_DiffuseReflectance{diffuseReflectance}
{

}

RGBColor Material_Lambert::Shade(const HitRecord& hitRecord, const FVector3& lightdirection, const FVector3& viewdirection) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);

}


Material_LambertPhong::Material_LambertPhong(const RGBColor& diffuseColor, float diffuseReflectance, float specularReflectance, float phongExponent)
	:Material_Lambert{ diffuseColor ,diffuseReflectance}
	,m_PhongExponent{phongExponent}
	,m_SpecularReflectance{ specularReflectance }
{
}

RGBColor Material_LambertPhong::Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + BRDF::Phong(m_SpecularReflectance,m_PhongExponent,w0,w1,hitRecord.normal);
}

Material_LambertCookTorrance::Material_LambertCookTorrance(const RGBColor& diffuseColor, float diffuseReflectance, const RGBColor& albedoColor, bool metalness, float roughness)
	:Material_Lambert{ diffuseColor ,diffuseReflectance }
	, m_AlbedoColor{ albedoColor }
	, m_Metalness{ metalness }
	, m_Roughness{ roughness }
{
	m_AlbedoColor = (m_Metalness == 0) ? RGBColor(0.04f, 0.04f, 0.04f) : albedoColor;
	m_Roughness = pow(m_Roughness, 2);
}
// test
RGBColor Material_LambertCookTorrance::Shade(const HitRecord& hitRecord, const FVector3& lightdirection, const FVector3& viewdirection) const
{
	//Calculate half vector between view directionand light direction
	//FVector3 squaredvl = (viewdirection + lightdirection);
	//squaredvl.x = pow(squaredvl.x, 2);
	//squaredvl.y = pow(squaredvl.y, 2);
	//squaredvl.z = pow(squaredvl.z, 2);
	//
	//FVector3 halfVector = squaredvl / (SqrMagnitude(viewdirection + lightdirection));
	Elite::FVector3 halfVector = (viewdirection + lightdirection) / (Elite::Magnitude(viewdirection + lightdirection));
	//Calculate Fresnel(F)
	RGBColor F = BRDF::Schlick(halfVector, viewdirection, m_AlbedoColor);
	//Calculate Normal Distribution(D)
	float D = BRDF::TrowbridgeReitzGGX(halfVector, hitRecord.normal, m_Roughness);
	//Calculate Geometry(G)
	float G = BRDF::SchlickGGX(hitRecord.normal, viewdirection, m_Roughness, true)* BRDF::SchlickGGX(hitRecord.normal, lightdirection, m_Roughness, true);
	//Calculate specular = > Cook - Torrance >(DFG) / 4(dot(v, n)dot(l, n))
	//Determine kd -> 1 - Fresnel, cancel out if it’s a metal(kd = 0)
	RGBColor kd{ 0,0,0 };
	if (!m_Metalness)
	{
		kd = RGBColor(1, 1, 1) - F;
	}
	//Calculate Diffuse = > BRDF::Lambert using the kd
	//BRDF::Lambert(kd, m_DiffuseColor);
	//Return final color->diffuse + specular
	return(kd * BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) +/* RGBColor(m_SpecularReflectance, m_SpecularReflectance, m_SpecularReflectance)*/BRDF::CookTorrance(F, D, G, hitRecord.normal, viewdirection, lightdirection));
	
	
	//return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + BRDF::Phong(m_SpecularReflectance, m_PhongExponent, lightdirection, viewdirection, hitRecord.normal);
}
