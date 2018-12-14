#include "Model.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "SceneEnvironment.h"
#include "Trackball.h"
#include <iostream>
#include "Noise.hpp"

namespace GAME {

	Model::Model(const Vec3 pos_, const Vec3 orientation_, class Shader* shader, const char* textureName) {
		pos = pos_;
		orientation = orientation_;
		this->shader = shader;
		this->textureName = textureName;
	}

	Model::~Model() {
		OnDestroy();
	}

	void Model::setPos(const Vec3& pos_) {
		Entity::setPos(pos_);
		updateModelMatrix();
	}

	void Model::setOrientation(const Vec3& orientation_) {
		Entity::setOrientation(orientation_);
		updateModelMatrix();
	}

	void Model::updateModelMatrix(){
		modelMatrix = MMath::translate(pos);

		/// This transform is based on Euler angles - let's do it later
		///modelMatrix = MMath::translate(pos) * MMath::rotate(orientation.z, Vec3(0.0f, 0.0f, 1.0f)) * MMath::rotate(orientation.x, Vec3(1.0f, 0.0f, 0.0f)) * MMath::rotate(orientation.y, Vec3(0.0f, 1.0f, 0.0f));
	}

	bool Model::OnCreate() {
		if(shader == nullptr)
			shader = new Shader("phongVert.glsl", "phongFrag.glsl", 3, 0, "vVertex", 1, "vNormal", 2, "texCoords");
		//TODO: get this shit working on loading a texture
		////make texture
		//glGenTextures(1, &texture);
		//glBindTexture(GL_TEXTURE_2D, texture);
		////filters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		////load image data
		//SDL_Surface *textureSurface = nullptr;

		//textureSurface = IMG_Load(textureName);
		//if (textureSurface == nullptr) return false;
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
		//	textureSurface->w, textureSurface->h, 0,
		//	GL_RGB, GL_FLOAT, textureSurface->pixels);
		//SDL_FreeSurface(textureSurface);
		//textureSurface = nullptr;

		//textureID = glGetUniformLocation(shader->getProgram(), "texture0");

		//get uniform IDs
		projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
		viewMatrixID = glGetUniformLocation(shader->getProgram(), "viewMatrix");
		modelMatrixID = glGetUniformLocation(shader->getProgram(), "modelMatrix");
		normalMatrixID = glGetUniformLocation(shader->getProgram(), "normalMatrix");
		lightPosID = glGetUniformLocation(shader->getProgram(), "lightPos");
		textureID = glGetUniformLocation(shader->getProgram(), "myTexture0");
		elapsedTimeID = glGetUniformLocation(shader->getProgram(), "elapsedTime");
		deltaTimeID = glGetUniformLocation(shader->getProgram(), "deltaTime");
		noiseTextureID = Noise().CreateNoise3D();



		glBindTexture(GL_TEXTURE_3D, noiseTextureID);
		

		return true;
	}

	

	bool Model::LoadMesh(const char* filename, GLenum drawMode) {
		if (ObjLoader::loadOBJ(filename) == false) {
			return false;
		}
		/// Get the data out of the ObjLoader and into our own mesh
		meshes.push_back(new Mesh(drawMode, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords));
		return true;
	}

	void Model::setShader(Shader * shader) {
		this->shader = shader;
	}
	Shader* Model::getShader() {
		return shader;
	}

	void Model::Update(const float deltaTime) {
		/// See Entity.h
		///Rotate(Vec3(0.0f, 50.0f * deltaTime, 0.0f));
	}

	void Model::Render() const {
		glUseProgram(shader->getProgram());

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, Camera::currentCamera->getProjectionMatrix());
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, Camera::currentCamera->getViewMatrix());
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix * Trackball::getInstance()->getMatrix4());
		glUniform1ui(elapsedTimeID, SDL_GetTicks());
		glUniform1f(deltaTimeID, 0.16f);//get delta time
		glEnable(GL_TEXTURE_3D);

		/// Assigning the 4x4 modelMatrix to the 3x3 normalMatrix 
		/// copies just the upper 3x3 of the modelMatrix
		Matrix3 normalMatrix = modelMatrix * Trackball::getInstance()->getMatrix4();
		glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);

		glUniform3fv(lightPosID, 1, SceneEnvironment::getInstance()->getLight());

		for (Mesh* mesh : meshes) {
			mesh->Render();
		}


	}
	void Model::OnDestroy() {
		if (shader) delete shader;
	}
}