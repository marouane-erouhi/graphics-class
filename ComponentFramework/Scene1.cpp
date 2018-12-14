#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "Scene1.h"
#include "SceneEnvironment.h"
#include "Trackball.h"

#include "ObjLoader.h"
#include "Noise.hpp"

using namespace GAME;
using namespace MATH;


Scene1::Scene1(Window& windowRef):Scene(windowRef) {

}


bool Scene1::OnCreate() {
	
	camera = nullptr;
	skyBox = new SkyBox("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg");
	skyBox->OnCreate();


	toonSkull = new Model(MATH::Vec3(0.01f), MATH::Vec3(),
		new Shader("toonVert.glsl", "toonFrag.glsl", 3,
			0, "vVertex", 1, "vNormal", 2, "texCoords"),
		"skull_texture.jpg");
	toonSkull->OnCreate();
	toonSkull->LoadMesh("skull.obj", GL_TRIANGLES);



	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}

//bool GAME::Scene1::addModel(const char* filename)
//{
//	auto model = new Model(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
//	
//	models.push_back(model );
//
//	
//	models[models.size() - 1]->OnCreate();
//
//	if (models[models.size() - 1]->LoadMesh(filename) == false) {
//		return false;
//	}
//	return true;
//}

void Scene1::OnResize(int w_, int h_){
	windowPtr->setWindowSize(w_,h_);
	glViewport(0,0,windowPtr->getWidth(),windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_, Vec3(0.0f, 0.0f, 10.0f));
	Camera::currentCamera = camera;
	Trackball::getInstance()->setWindowDimensions(windowPtr->getWidth(), windowPtr->getHeight());
}

void Scene1::Update(const float deltaTime) {
	for (Model* model : models) {
		model->Update(deltaTime);
	}

	toonSkull->Update(deltaTime);
}

void Scene1::Render() const{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //backface culling
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_TEXTURE_3D);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	skyBox->Render(camera->getProjectionMatrix(), camera->getViewMatrix(), MATH::Matrix4());
	toonSkull->Render();
	
	/// Draw your scene here
	for (Model* model : models) {
		model->Render();
	}
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());
}

void Scene1::HandleEvents(const SDL_Event& SDLEvent){
	if (SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
		Trackball::getInstance()->onLeftMouseDown(SDLEvent.button.x, SDLEvent.button.y);
	}else if(SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP){
		Trackball::getInstance()->onLeftMouseUp(SDLEvent.button.x, SDLEvent.button.y);
	}else if (SDLEvent.type == SDL_EventType::SDL_MOUSEMOTION && 
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Trackball::getInstance()->onMouseMove(SDLEvent.button.x, SDLEvent.button.y);
	}
}

Scene1::~Scene1() {
	OnDestroy();
}

void Scene1::OnDestroy() {
	/// Cleanup Assets
	if (camera) delete camera;
	for (Model* model : models) {
		if (model) delete model;
	}
}
