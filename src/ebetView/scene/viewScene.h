
#ifndef EBETVIEW_GAME_SCENE 
#define EBETVIEW_GAME_SCENE

#include "cnge/engine/camera.h"
#include "cnge/util/fullAspect.h"
#include "cnge/scene/scene.h"
#include "cnge/util/color.h"
#include "cnge/engine/texture/texture.h"
#include "cnge/image/image.h"

namespace Game {
	class ViewScene : public CNGE::Scene {
	private:
		CNGE::Color backgroundColor;

		CNGE::Camera camera;
		CNGE::FullAspect aspect;

		std::unique_ptr<CNGE::Texture> imageTexture;
		CNGE::Image1D* image;
		
		std::string inputFile;
		std::string errMessage;

		i32 offsetX, offsetY;
		f32 zoom;

		bool dragging;
		i32 dragX, dragY;
		
	public:
		ViewScene(std::string&& inputFile);
		
		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNFW::Input*, CNFW::Timing*) -> void override;

		auto render() -> void override;

		auto switchScene() -> CNGE::SwitchReturn override;

		/* image functions */
		
		auto resetView() -> void;
		
		auto getImageWidth() -> i32;
		auto getImageHeight() -> i32;

		auto fitInFrame() -> void;
	};
}

#endif
