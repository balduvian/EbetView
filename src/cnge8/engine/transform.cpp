
#include <iostream>

#include "transform.h"

namespace CNGE {
	Matrix4f Transform::matrix{};

	float Transform::defaultModel[16] {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float Transform::defaultProjview[16] {
		2, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 1, 0,
		-1, -1, 0, 1
	};

	/// default initialization
	Transform::Transform() : translation(0.f, 0.f), scale(1.f, 1.f), rotation(0) {}

	/// modifes a mat4 based on values passed in
	void Transform::transform(Matrix4f& mat, f32 x, f32 y, f32 rotation, f32 width, f32 height) {
		mat.setTranslate2D(x, y).rotate2D(rotation).scale2D(width, height);
	}

	void Transform::transform(Matrix4f& mat, f32 x, f32 y, f32 z, f32 rotation, f32 width, f32 height) {
		mat.setTranslate(x, y, z).rotate2D(rotation).scale2D(width, height);
	}

	/// returns the float values for a model matrix based on this transform
	float* Transform::toModel() {
		transform(matrix);

		return matrix.m;
	}

	float* Transform::toZModel(f32 z) {
		transform(matrix, translation.x(), translation.y(), z, rotation, scale.x(), scale.y());

		return matrix.m;
	}

	/// returns the float values for a model matrix based on given params
	/// no instance of a transform needed
	float* Transform::toModel(f32 x, f32 y, f32 rotation, f32 width, f32 height) {
		transform(matrix, x, y, rotation, width, height);

		return matrix.m;
	}

	float* Transform::toModelRC(f32 x, f32 y, f32 rotation, f32 width, f32 height) {
		matrix.setTranslate2D(x + width / 2, y + height / 2).rotate2D(rotation).translate2D(-width / 2, -height / 2).scale2D(width, height);

		return matrix.m;
	}

	/// modifies a mat4 based on this transform
	void Transform::transform(Matrix4f& mat) {
		transform(mat, translation.x(), translation.y(), rotation, scale.x(), scale.y());
	}

	/// modify this matrix for the camera, everything is in reverse order and negative
	void Transform::cameraTransform(Matrix4f& mat) {
		mat.setScale2D(scale).rotate2D(-rotation).translate2D(-translation);
	}
}
