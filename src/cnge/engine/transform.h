
#pragma once

#include "cnge/math/matrix4.h"
#include "cnge/math/vector2.h"

namespace CNGE {
	class Transform {
	public:
		/*
		 * initializers for the default model and projection value arrays
		 */
		static float defaultModel[16];
		static float defaultProjview[16];

		Vector2f translation;
		Vector2f scale;
		float rotation;

		/// default initialization
		Transform();

		/// modifes a mat4 based on values passed in
		static void transform(Matrix4f&, f32 x, f32 y, f32 rotation, f32 width, f32 height);
		static void transform(Matrix4f&, f32 x, f32 y, f32 z, f32 rotation, f32 width, f32 height);

		/// returns the float values for a model matrix based on this transform
		float* toModel();
		/// return the float values for a model matrix that also has a depth
		float* toZModel(f32 z);

		/// returns the float values for a model matrix based on given params
		/// no instance of a transform needed
		static float* toModel(f32 x, f32 y, f32 rotation, f32 width, f32 height);
		static float* toModelRC(f32 x, f32 y, f32 rotation, f32 width, f32 height);

		/// modifies a mat4 based on this transform's internal values
		void transform(Matrix4f&);

		/// modify this matrix for the camera, everything is in reverse order and negative
		void cameraTransform(Matrix4f&);

	private:
		/// used for generating values
		static Matrix4f matrix;
	};
}
