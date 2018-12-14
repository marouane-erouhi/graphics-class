#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "MMath.h"
#include "Mesh.h"
#include "Entity.h"
#include <SDL.h>
#include <SDL_image.h>
namespace GAME {

using namespace MATH;

class Model : public Entity {

protected:

	class Shader *shader;

	Matrix4 modelMatrix;
	std::vector<Mesh*> meshes;

	const char* textureName;
	GLuint textureID;
	GLuint texture;

	GLint projectionMatrixID;
	GLint viewMatrixID;
	GLint modelMatrixID;
	GLint normalMatrixID;
	GLint lightPosID;
	GLint elapsedTimeID;
	GLint deltaTimeID;
	GLint noiseTextureID;

public:
	Model(){}
	Model(const Vec3 _pos, const Vec3 _orientation, class Shader* shader = nullptr, const char* textureName = nullptr);
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	Model& operator = (const Model&) = delete;
	Model& operator = (Model&&) = delete;

	virtual ~Model();

	void setPos(const Vec3& pos_) override;
	void setOrientation(const Vec3& orienration_) override;

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Render() const;
	virtual void Update(const float deltaTime);

	virtual bool LoadMesh(const char* filename, GLenum drawMode_);

	void setShader(class Shader* shader);
	Shader* getShader();
protected:

	void updateModelMatrix();
};
} /// end of namespace

#endif