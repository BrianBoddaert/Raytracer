//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"

#include "SceneGraph.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Plane.h"



Material_LambertCookTorrance* m_Plastic1;
Material_LambertCookTorrance* m_Plastic2;
Material_LambertCookTorrance* m_Plastic3;
Material_LambertCookTorrance* m_Metal1;
Material_LambertCookTorrance* m_Metal2;
Material_LambertCookTorrance* m_Metal3;
Material_Lambert* m_BunnyMaterial;
Material* m_PlaneMaterial;
void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

void Initialize() 
{
	m_Plastic1 = new Material_LambertCookTorrance(Elite::RGBColor(0.6f, 0.6f, 0.6f), 1.0f, Elite::RGBColor(0.f, 0.f, 0.f), false, 1.f);
	m_Plastic2 = new Material_LambertCookTorrance(Elite::RGBColor(0.6f, 0.6f, 0.6f), 1.0f, Elite::RGBColor(0.f, 0.f, 0.f), false, 0.5f);
	m_Plastic3 = new Material_LambertCookTorrance(Elite::RGBColor(0.6f, 0.6f, 0.6f), 1.0f, Elite::RGBColor(0.f, 0.f, 0.f), false, 0.1f);
	m_Metal1 = new Material_LambertCookTorrance(Elite::RGBColor(0.6f, 0.6f, 0.6f), 1.0f, Elite::RGBColor(0.972f, 0.960f, 0.915f), true, 1.f);
	m_Metal2 = new Material_LambertCookTorrance(Elite::RGBColor(0.6f, 0.6f, 0.6f), 1.0f, Elite::RGBColor(0.972f, 0.960f, 0.915f), true, 0.5f);
	m_Metal3 = new Material_LambertCookTorrance(Elite::RGBColor(0.6f, 0.6f, 0.6f), 1.0f, Elite::RGBColor(0.972f, 0.960f, 0.915f), true, .1f);

	m_PlaneMaterial = new Material_Lambert(RGBColor(1.f, 0.85f, 0.75f), 0.9f);
	m_BunnyMaterial = new Material_Lambert(RGBColor(1.f, 0.f, 0.f), 0.f);
	FPoint3 vertices1[3] = { {0,0,0},{0,0,0},{0,0,0} };

	vertices1[0] = { -.75,3.5,0 };
	vertices1[1] = { -.75,2,0 };
	vertices1[2] = { .75,2,0 };

	FPoint3 vertices2[3] = { {0,0,0},{0,0,0},{0,0,0} };

	vertices2[0] = { -.75+-2,3.5,0 };
	vertices2[1] = { -.75+-2,2,0 };
	vertices2[2] = { .75 +-2,2,0 };

	FPoint3 vertices3[3] = { {0,0,0},{0,0,0},{0,0,0} };

	vertices3[0] = { -.75 + 2,3.5,0 };
	vertices3[1] = { -.75 + 2,2,0 };
	vertices3[2] = { .75 + 2,2,0 };
					
#ifndef WITCHER
	//Scene 0
	SceneGraph::GetInstance()->AddObjectToGraph(new Triangle{vertices1,m_PlaneMaterial ,Cullmode::frontface });
	SceneGraph::GetInstance()->AddObjectToGraph(new Triangle{vertices2,m_PlaneMaterial,Cullmode::backface  });
	SceneGraph::GetInstance()->AddObjectToGraph(new Triangle{vertices3,m_PlaneMaterial ,Cullmode::noculling });

	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(0.f, -3, 0),FVector3( 0,1,0 ),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial });
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(0.f, 7, 0),FVector3(0,-1,0),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial });
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(0.f, 0, -10.f),FVector3(0,0,1),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial });
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(-5.f, 0, 0),FVector3(1,0,0),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial });
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(5.f, 0, 0),FVector3(-1,0,0),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial });

	SceneGraph::GetInstance()->AddObjectToGraph(new Sphere{ FPoint3(2.f, 1, 0),.8f,RGBColor(0,1,0),m_Plastic1 });
	SceneGraph::GetInstance()->AddObjectToGraph(new Sphere{ FPoint3(0, 1, 0),.8f,RGBColor(0,1,0),m_Plastic2 });
	SceneGraph::GetInstance()->AddObjectToGraph(new Sphere{ FPoint3(-2.f, 1,  0),.8f,RGBColor(0,0,1),m_Plastic3 });
	
	SceneGraph::GetInstance()->AddObjectToGraph(new Sphere{ FPoint3(2.f, -1, 0),.8f,RGBColor(0,1,0),m_Metal1 });
	SceneGraph::GetInstance()->AddObjectToGraph(new Sphere{ FPoint3(0, -1, 0),.8f,RGBColor(0,1,0),m_Metal2 });
	SceneGraph::GetInstance()->AddObjectToGraph(new Sphere{ FPoint3(-2.f, -1,  0),.8f,RGBColor(0,0,1),m_Metal3 });

	SceneGraph::GetInstance()->AddLightToGraph(new Light({ 1.5,3,4 }, { 1,1,1 }, 25));
	SceneGraph::GetInstance()->AddLightToGraph(new Light({ -1.5,3,4 }, { 1,1,1 }, 25));
	SceneGraph::GetInstance()->AddLightToGraph(new Light({ -4,1,10 }, { 1,1,1 }, 1.5f, false));

	//Scene 1
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(0.f, -3, 0),FVector3(0,1,0),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial },1);
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(0.f, 7, 0),FVector3(0,-1,0),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial },1);
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(0.f, 0, -10.f),FVector3(0,0,1),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial },1);
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(-5.f, 0, 0),FVector3(1,0,0),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial },1);
	SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(5.f, 0, 0),FVector3(-1,0,0),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial },1);

	SceneGraph::GetInstance()->AddObjectToGraph(new TriangleMesh(m_PlaneMaterial, Cullmode::backface), 1);

	SceneGraph::GetInstance()->AddLightToGraph(new Light({ 1.5,2,4 }, { 1,1,1 }, 25) ,1);
	SceneGraph::GetInstance()->AddLightToGraph(new Light({ -1.5,2,4 }, { 1,1,1 }, 25),1);
	SceneGraph::GetInstance()->AddLightToGraph(new Light({ -4,1,10 }, { 1,1,1 }, 1.5f, false),1);
#endif
#ifdef WITCHER
	// Witcher scene

	//SceneGraph::GetInstance()->AddObjectToGraph(new Plane{ FPoint3(0.f, 0, -10.f),FVector3(0,0,1),RGBColor(0.88f,0.88f,0.88f),m_PlaneMaterial }, 0);

	SceneGraph::GetInstance()->AddObjectToGraph(new TriangleMesh(m_PlaneMaterial, Cullmode::backface), 0);

	SceneGraph::GetInstance()->AddLightToGraph(new Light({ 0,10,0 }, { 1,1,1 }, 2.0f, false), 0);
#endif
}
int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	float speed = 20.f;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - **Brian Boddaert**",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;

	bool m1Down = false;
	bool m2Down = false;

	int* mouseX = new int();
	int* mouseY = new int();

	Camera* camera = Camera::getInstance();

	SceneGraph* sceneGraph = SceneGraph::GetInstance();
	Elite::FVector3 orientationOffset;

#ifdef WITCHER
	
	float xMovementOffset = 0;
	float yMovementOffset = 200;
	float zMovementOffset = -40;
	orientationOffset = { -90,45,0 };
	
#endif
#ifndef WITCHER
	float xMovementOffset = 0;
	float yMovementOffset = 0;
	float zMovementOffset = 0;
	orientationOffset = { 0,0,0 };
#endif

	float rotatingSpeed = 2;

	IVector2 previousMousePos{ INT_MAX,INT_MAX };
	float deltaT;

	Initialize();
	std::cout << "V to toggle scenes" << std::endl;
	while (isLooping)
	{
		//--------- Get input events ---------
	//Controls: M1+Drag = Forward or backward camera, rotate left or right
	// M2+Drag  = Rotate only
	// M1+M2+drag = up and down
	// WASD to move around V
	// Shift of speed for movement V
	// Deltatime from time class
	
		if (pTimer->GetFPS() != 0) {
		deltaT = 1.0f / float(pTimer->GetFPS());
		}
		else
		{
		deltaT = .001f;
		}

		SDL_Event e;



		while (SDL_PollEvent(&e))
		{


			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_J)
				{
					sceneGraph->ToggleLight(2);
					std::cout << "Directional light toggled" << std::endl;

				}
				if (e.key.keysym.scancode == SDL_SCANCODE_K)
				{
					sceneGraph->ToggleLight(0);
					std::cout << "Light 1 toggled" << std::endl;

				}
				if (e.key.keysym.scancode == SDL_SCANCODE_L)
				{
					sceneGraph->ToggleLight(1);
					std::cout << "Light 2 toggled" << std::endl;

				}
				if (e.key.keysym.scancode == SDL_SCANCODE_Z)
				{
					sceneGraph->ToggleHardShadows(); 
					std::cout << "Shadows toggled" << std::endl;

				}
				if (e.key.keysym.scancode == SDL_SCANCODE_V)
				{
					sceneGraph->ToggleScene();
					std::cout << "Scene toggled" << std::endl;

				}
				if(e.key.keysym.scancode == SDL_SCANCODE_X)
				{

					std::cout << "Forward vector : ";
					std::cout << Camera::getInstance()->GetForwardVector().x << std::endl;
					std::cout << Camera::getInstance()->GetForwardVector().y << std::endl;
					std::cout << Camera::getInstance()->GetForwardVector().z << std::endl;
					std::cout << "Pos : ";
					std::cout << Camera::getInstance()->GetPosition().x << std::endl;
					std::cout << Camera::getInstance()->GetPosition().y << std::endl;
					std::cout << Camera::getInstance()->GetPosition().z << std::endl;
					takeScreenshot = true;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_UP)
				{
					speed += 10;
					std::cout << "Speed Increased!" << std::endl;
				}
				else if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
				{
					if (speed > 10){
						speed -= 10;
					std::cout << "Speed Decreased!" << std::endl;
					}
					else
					{
						std::cout << "Slowest speed limit reached!" << std::endl;
					}
				}
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_W)
				{
					zMovementOffset += speed * deltaT;
					
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_S)
				{
					zMovementOffset -= speed * deltaT;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_A)
				{
					
					xMovementOffset -= speed * deltaT;
				
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_D)
				{
					xMovementOffset += speed * deltaT;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_Q)
				{
					orientationOffset.y -= deltaT * speed;
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_E)
				{
					orientationOffset.y += deltaT * speed;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ( e.button.button  == SDL_BUTTON_LEFT)
				{
					m1Down = true;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					m2Down = true;
				}
				break;
			case SDL_MOUSEMOTION:

				SDL_GetGlobalMouseState(mouseX, mouseY);
				if (m1Down && m2Down)
				{
					yMovementOffset = yMovementOffset - (*mouseY - previousMousePos.y) * deltaT;
				}					
				else if (m1Down)
				{
					zMovementOffset = { zMovementOffset - (*mouseY - previousMousePos.y) * deltaT };
					orientationOffset.y += (*mouseX - previousMousePos.x) * deltaT * rotatingSpeed;
				}
				else if (m2Down)
				{
					orientationOffset.y += ((*mouseX - previousMousePos.x) * rotatingSpeed) * deltaT;
					orientationOffset.x += ((*mouseY - previousMousePos.y) * rotatingSpeed) * deltaT;
				}
				previousMousePos = IVector2{*mouseX,*mouseY};

				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					m1Down = false;
				}
				else if (e.button.button == SDL_BUTTON_RIGHT)
				{
					m2Down = false;
				}
				break;

			}
		}
		
		if (xMovementOffset != 0)
		{
			camera->TranslateX(xMovementOffset);
		}
		if (yMovementOffset !=0)
		{
			camera->TranslateY(yMovementOffset);
		}
		if (zMovementOffset != 0)
		{
			camera->TranslateZ(zMovementOffset);
		}
		if (orientationOffset.x != 0)
		{ 
			camera->Pitch(orientationOffset.x);
		}
		if (orientationOffset.y != 0)
		{
			camera->Yaw(orientationOffset.y);
		}
		orientationOffset = { 0,0,0 };
		xMovementOffset = 0	 ;
		yMovementOffset = 0	 ;
		zMovementOffset = 0	 ;
		//--------- Render ---------
		pRenderer->Render();
		
		//--------- Timer ---------
		pTimer->Update();

		SceneGraph::GetInstance()->RotateTriangles(45.f*deltaT);

		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS() << std::endl;
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBackbufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete mouseX;
	delete mouseY;
	delete pTimer;
	SceneGraph::ReleaseInstance();
	delete Camera::getInstance();
	delete m_Plastic1;
	delete m_Plastic2;
	delete m_Plastic3;
	delete m_Metal1;
	delete m_Metal2;
	delete m_Metal3;
	delete m_BunnyMaterial;
	ShutDown(pWindow);
	return 0;
}