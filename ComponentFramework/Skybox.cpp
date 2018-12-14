#pragma once
#include "Skybox.h"
#include <SDL_image.h>
#include "Shader.h"
#include "ObjLoader.h"
#include "Trackball.h"
using namespace GAME;
GAME::SkyBox::SkyBox(const char * positive_x_image, const char * negative_x_image, 
					 const char * positive_y_image, const char * negative_y_image, 
					 const char * positive_z_image, const char * negative_z_image) :
	positive_x_image(positive_x_image), negative_x_image(negative_x_image), 
	positive_y_image(positive_y_image), negative_y_image(negative_y_image),
	positive_z_image(positive_z_image), negative_z_image(negative_z_image){}
GAME::SkyBox::~SkyBox() {}
bool SkyBox::OnCreate() {

	ObjLoader obj;
	if (obj.loadOBJ("cube.obj") == false) {
		return false;
	}

	meshes.push_back(new Mesh(GL_TRIANGLES, obj.vertices, obj.normals, obj.uvCoords));

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	SDL_Surface *textureSurface = nullptr;
	//loading all the image
	textureSurface = IMG_Load(positive_x_image);
	if (textureSurface == nullptr) return false;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, textureSurface->w, textureSurface->h,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;

	textureSurface = IMG_Load(negative_x_image);
	if (textureSurface == nullptr) return false;
	///mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, textureSurface->w, textureSurface->h,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;


	textureSurface = IMG_Load(positive_y_image);
	if (textureSurface == nullptr) return false;
	///mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, textureSurface->w, textureSurface->h,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;

	textureSurface = IMG_Load(negative_y_image);
	if (textureSurface == nullptr) return false;
	///mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, textureSurface->w, textureSurface->h,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;


	textureSurface = IMG_Load(positive_z_image);
	if (textureSurface == nullptr) return false;
	///mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, textureSurface->w, textureSurface->h,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;


	textureSurface = IMG_Load(negative_z_image);
	if (textureSurface == nullptr) return false;
	///mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, textureSurface->w, textureSurface->h,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	textureSurface = nullptr;


	shader = new Shader("skyboxVert.glsl", "skyboxFrag.glsl", 1, 0, "vVertex");
	projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
	/// You may prefer to send the view and model matrices separately 
	modelViewMatrixID = glGetUniformLocation(shader->getProgram(), "modelViewMatrix");
	textureID = glGetUniformLocation(shader->getProgram(), "cubeTexture");


	return true;
}

void GAME::SkyBox::OnDestroy() {
}

void GAME::SkyBox::Update(const float time) {
}

void GAME::SkyBox::Render(const MATH::Matrix4 & projectionMatrix, const MATH::Matrix4 & modelViewMatrix, const MATH::Matrix4 & normalMatrix) const {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glUseProgram(shader->getProgram());

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, modelMatrix * Trackball::getInstance()->getMatrix4());

	glDrawArrays(GL_TRIANGLES, 0, 36);
	for (Mesh * mesh : meshes) {
		mesh->Render();
	}
	glEnableVertexAttribArray(0);//disable VAO

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

GLuint GAME::SkyBox::getTexture() {
	return texture;
}
