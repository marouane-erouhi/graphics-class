#pragma once
#include "Model.h"
#include <SDL.h>
namespace GAME {
	class SkyBox : public Model {
	private:
		const char * positive_x_image;
		const char * negative_x_image;
		const char * positive_y_image;
		const char * negative_y_image;
		const char * positive_z_image;
		const char * negative_z_image;
		//class Shader *shader;
		// these are redefined incase i change the model class later
		GLuint textureID;
		GLuint texture;
		//*****************
		GLuint projectionMatrixID;
		GLuint modelViewMatrixID;
	public:
		SkyBox(const char * positive_x_image,
			   const char * negative_x_image,
			   const char * positive_y_image,
			   const char * negative_y_image,
			   const char * positive_z_image,
			   const char * negative_z_image);
		virtual ~SkyBox();


		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float time);
		virtual void Render(const MATH::Matrix4& projectionMatrix, const MATH::Matrix4& modelViewMatrix, const MATH::Matrix4& normalMatrix) const;
		GLuint getTexture();
	};
}

