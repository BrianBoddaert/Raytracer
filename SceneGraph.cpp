#include "SceneGraph.h"
#include "EMath.h"
#include "Camera.h"
#include "Triangle.h"
using namespace Elite;

SceneGraph* SceneGraph::m_pInstance{ nullptr };

SceneGraph* SceneGraph::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneGraph;
		
	}
	return m_pInstance;
}

SceneGraph::SceneGraph()
{
	m_Scenes.push_back(new Scene(0));
	m_Scenes.push_back(new Scene(1));
	m_pCurrentSceneIndex = 0;
	m_pCurrentScene = m_Scenes[m_pCurrentSceneIndex];
}
void SceneGraph::ReleaseInstance()
{
		delete m_pInstance;
		m_pInstance = nullptr;
}

SceneGraph::~SceneGraph()
{
	for (size_t j = 0; j < m_Scenes.size(); j++)
	{
		for (size_t i = 0; i < m_Scenes[j]->shapesInScene.size(); i++)
		{
			delete  m_Scenes[j]->shapesInScene[i];
		}

		for (size_t i = 0; i < m_Scenes[j]->lightsInScene.size(); i++)
		{
			delete m_Scenes[j]->lightsInScene[i];
		}
	}
}

void SceneGraph::AddLightToGraph(Light* light, int index)
{
	m_Scenes[index]->lightsInScene.push_back(light);
}


void SceneGraph::AddObjectToGraph(Shape* shape, int index)
{
	m_Scenes[index]->shapesInScene.push_back(shape);
}



bool SceneGraph::ClosestIntersection(const Ray& ray, HitRecord& ClosestIntersectionHitrecord)
{
	ClosestIntersectionHitrecord.distance = FLT_MAX;
	HitRecord hitrecord;
	
	for (size_t i = 0; i < m_pCurrentScene->shapesInScene.size(); i++)
	{
		if (m_pCurrentScene->shapesInScene[i]->CheckBoundingBoxIntersection(ray, hitrecord)) {
			if (m_pCurrentScene->shapesInScene[i]->Hit(ray, hitrecord))
			{
				if (ClosestIntersectionHitrecord.distance > hitrecord.distance && hitrecord.distance > 0)
				{
					ClosestIntersectionHitrecord = hitrecord;
				}

			}
		}
	}

	if (ClosestIntersectionHitrecord.distance != FLT_MAX)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SceneGraph::CheckIntersection(const Ray& ray)
{

	HitRecord hitrecord;
	for (size_t i = 0; i < m_pCurrentScene->shapesInScene.size(); i++)
	{
		if (m_pCurrentScene->shapesInScene[i]->Hit(ray, hitrecord))
		{
			return true;

		}
	}
	return false;
}

void SceneGraph::RotateTriangles(float angle)
{
	for (size_t i = 0; i < m_pCurrentScene->shapesInScene.size(); i++)
	{
		m_pCurrentScene->shapesInScene[i]->Rotate(angle);
	}
}
void SceneGraph::ToggleHardShadows() 
{
	m_HardShadowsOn = !m_HardShadowsOn;
}

void SceneGraph::ToggleScene()
{
	m_pCurrentSceneIndex++;
	if (m_pCurrentSceneIndex >= m_Scenes.size())
	{
		m_pCurrentSceneIndex = 0;
	}
	m_pCurrentScene = m_Scenes[m_pCurrentSceneIndex];
}

void SceneGraph::ToggleLight(int index)
{
	if (m_pCurrentScene->lightsInScene.size()-1 >= index)
	{
		m_pCurrentScene->lightsInScene[index]->active = !(m_pCurrentScene->lightsInScene[index]->active);
	}
}
RGBColor SceneGraph::DiffuseLighting(const HitRecord& closestIntersectionHitrecord)
{


	RGBColor diffuseColor{ 0,0,0 };

	for (size_t i = 0; i < m_pCurrentScene->lightsInScene.size(); i++)
	{
		if (m_pCurrentScene->lightsInScene[i]->active)
		{
			FVector3 lightingRayDirection;
			if (m_pCurrentScene->lightsInScene[i]->isPointLight)
			{
				lightingRayDirection = (m_pCurrentScene->lightsInScene[i]->position - FVector3{ closestIntersectionHitrecord.hitPos });
			}
			else
			{
				lightingRayDirection = m_pCurrentScene->lightsInScene[i]->direction;
			}
			 lightingRayDirection = (m_pCurrentScene->lightsInScene[i]->position - FVector3{ closestIntersectionHitrecord.hitPos });
			
			Ray lightingRay(closestIntersectionHitrecord.hitPos, (lightingRayDirection),0.0001f, Normalize(lightingRayDirection));

			if (!SceneGraph::GetInstance()->CheckIntersection(lightingRay) || !m_HardShadowsOn)
			{ 
				RGBColor Ergb;
				FVector3 lightDirection = GetNormalized(lightingRayDirection);
				if (m_pCurrentScene->lightsInScene[i]->isPointLight)
				{
				Ergb = m_pCurrentScene->lightsInScene[i]->color * m_pCurrentScene->lightsInScene[i]->intensity / pow(Magnitude(m_pCurrentScene->lightsInScene[i]->position - FVector3{ closestIntersectionHitrecord.hitPos }), 2);
				
				}
				else
				{
				Ergb = m_pCurrentScene->lightsInScene[i]->color * m_pCurrentScene->lightsInScene[i]->intensity;
				}
				
				float LambertLaw = Dot(closestIntersectionHitrecord.normal, lightDirection);
				

				if (LambertLaw >= 0 )
				{
					FVector3 hitToCamDirection{ GetNormalized(FVector3(closestIntersectionHitrecord.hitPos,Camera::getInstance()->GetPosition()))};
					diffuseColor += Ergb * closestIntersectionHitrecord.pMaterial->Shade(closestIntersectionHitrecord, lightDirection, hitToCamDirection) * LambertLaw;
				}
			}


		}
	}
	return diffuseColor;
}
