
#pragma once

#include <GL/glew.h>
#include <GL/GL.h>

#include "types.h"
#include "cnge/load/resource.h"

namespace CNGE {
	class Shader : public Resource {
	public:
		constexpr static char const * MODEL_NAME = "model";
		constexpr static char const * PROJVIEW_NAME = "projView";

		/*
		 * load
		 */
		/// constructs the shader
		Shader(bool, const char*, const char*);

		/*
		 * use 
		 */
		/// getter
		int getUniform(const char* name);

		/// startLoading using this shader
		/// after calling this then give other things
		void enable(f32 model[], f32 projview[]);

		/// enables the shader and covers the entire ndc coordinates
		void enable();

		/*
		 * GIVERS
		 */

		void giveFloat(const int location, const float x);

		void giveVector2(const int location, const float x, const float y);

		void giveVector3(const int location, const float x, const float y, const float z);

		void giveVector4(const int location, const float x, const float y, const float z, const float w);

		void giveVector4(const int location, const float values[]);

		void giveMatrix4(const int location, const float values[]);

		~Shader();

	protected:
		/// implement this
		virtual void getUniforms() = 0;

	private:
		/* loading */
		const char* vertexPath;
		const char* fragmentPath;

		char* vertexData;
		char* fragmentData;

		auto customGather() -> void override;
		auto customProcess() -> void override;
		auto customDiscard() -> void override;
		auto customUnload() -> void override;

		/* use */
		GLuint program;

		GLint modelLocation;
		GLint projviewLocation;
	};

}
