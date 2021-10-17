#pragma once

#include "Structs.h"
#include "ERGBColor.h"
#include "EMath.h"


using namespace Elite;

class Material
{
public:
	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const = 0;

};

class Material_Lambert : public Material
{
public:
	Material_Lambert(const RGBColor& diffuseColor, float diffuseReflectance);
	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;
	RGBColor m_DiffuseColor;
	float	 m_DiffuseReflectance;
};

class Material_LambertPhong : public Material_Lambert
{
public:
	Material_LambertPhong(const RGBColor& diffuseColor, float diffuseReflectance, float specularReflectance, float phongExponent);
	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;
	float m_PhongExponent;
	float m_SpecularReflectance;
};

class Material_LambertCookTorrance : public Material_Lambert
{
public:
	Material_LambertCookTorrance(const RGBColor& diffuseColor, float diffuseReflectance, const RGBColor& albedoColor, bool metalness, float roughness);
	virtual RGBColor Shade(const HitRecord& hitRecord, const FVector3& w0, const FVector3& w1) const override;
	RGBColor m_AlbedoColor;
	bool m_Metalness;
	float m_Roughness;
};