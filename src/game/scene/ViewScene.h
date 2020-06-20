
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include "engine/camera.h"
#include "util/fullAspect.h"
#include "scene/scene.h"
#include "../cnge8/util/color.h"
#include "engine/texture/texture.h"
#include "image/image.h"

namespace Game {
	class ViewScene : public CNGE::Scene {
	private:
		CNGE::Color backgroundColor;

		CNGE::Camera camera;
		CNGE::FullAspect aspect;

		std::unique_ptr<CNGE::Texture> imageTexture;
		CNGE::Image1D* image;
		
		bool shouldRender;
		std::string inputFile;
		std::string errMessage;

		f32 offsetX, offsetY;
		f32 zoom;

		bool dragging;
		f32 dragX, dragY;
		
	public:
		ViewScene(std::string&& inputFile);
		
		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render(CNGE::Window* window) -> void override;

		auto switchScene() -> CNGE::SwitchReturn override;

		/* image functions */
		
		auto resetView() -> void;
		
		auto getImageWidth() -> i32;
		auto getImageHeight() -> i32;

		auto fitInFrame() -> void;
	};
}

#endif
