
#ifndef CNGE_CAMERA
#define CNGE_CAMERA

#include "transform.h"

#include "types.h"

namespace CNGE {
	class Camera {
	public:
		f32 width;
		f32 height;

		Transform transform;

		/// default initialization
		Camera();

		/// a pos up orthographic projection matrix
		/// for 2d games
		void setOrtho(f32, f32);

		/// a pos down orthographic projection matrix
		/// for gui applications
		void setOrthoPixel(u32, u32);
		
		/// sets the view based off the transform
		void update();

		/// use this in shaders to get a non moving projection
		[[nodiscard]] f32* getProjection() const;

		/// use this in shaders to get a projection that moves
		/// with the camera
		[[nodiscard]] f32* getProjview() const;

	private:
		Matrix4f projection;
		Matrix4f projview;
	};
}

#endif
