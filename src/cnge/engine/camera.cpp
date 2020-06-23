
#include "camera.h"

namespace CNGE {
	/// default initialization
	Camera::Camera() : transform(), projection(), projview(), width(0), height(0) {}

	/// a pos up orthographic projection matrix
	/// for 2d games
	void Camera::setOrtho(f32 width, f32 height) {
		this->width = width;
		this->height = height;

		projection.setOrthographic(0, width, 0, height, -1, 1);
	}

	void Camera::setOrthoPixel(u32 width, u32 height) {
		this->width = width;
		this->height = height;

		projection.setOrthographic(0, width, height, 0, -1, 1);
	}

	/// sets the view based off the transform
	void Camera::update() {
		transform.cameraTransform(projview);
		projview = projection * projview;
	}

	/// use this in shaders to get a non moving projection
	f32* Camera::getProjection() const {
		return projection.m;
	}

	/// use this in shaders to get a projection that moves
	/// with the camera
	f32* Camera::getProjview() const {
		return projview.m;
	}

}
