
#include "ViewScene.h"

#include "math/math.h"
#include "engine/transform.h"
#include "../res.h"

#include <iostream>

namespace Game {
	ViewScene::ViewScene(std::string&& inputFile) :
		Scene(&Res::viewResources),
		backgroundColor(0x37393f),
		imageTexture(nullptr),
		image(nullptr),
		shouldRender(false),
		inputFile(std::move(inputFile))
	{}

	auto ViewScene::start() -> void {
		try {
			imageTexture = std::make_unique<CNGE::Texture>(inputFile.c_str(), CNGE::TextureParams().setDefaultMinFilter(GL_LINEAR).setDefaultMagFilter(GL_NEAREST));
			imageTexture->quickGather();
			imageTexture->process();

			image = imageTexture->getImage();
			
		} catch (std::exception& ex) {
			errMessage = ex.what();
			std::cout << errMessage << std::endl;
		}

		resetView();
		
		dragging = false;
	}

	auto ViewScene::resetView() -> void {
		offsetX = 0;
		offsetY = 0;
		zoom = 1.0f;
	}

	auto ViewScene::getImageWidth() -> i32 {
		return static_cast<i32>(roundf(image->getWidth() * zoom / 2.0f)) * 2;
	}
	
	auto ViewScene::getImageHeight() -> i32 {
		return static_cast<i32>(roundf(image->getHeight() * zoom / 2.0f)) * 2;
	}

	auto ViewScene::fitInFrame() -> void {
		const auto screenWidth = static_cast<i32>(aspect.getWidth());
		const auto screenHeight = static_cast<i32>(aspect.getHeight());

		const auto halfScreenWidth = screenWidth / 2;
		const auto halfScreenHeight = screenHeight / 2;

		const auto imgWidth = getImageWidth();
		const auto imgHeight = getImageHeight();

		const auto halfImgWidth = imgWidth / 2;
		const auto halfImgHeight = imgHeight / 2;
		
		if (imgWidth > screenWidth) {
			if (halfScreenWidth - halfImgWidth + offsetX > 0)
				offsetX = -halfScreenWidth + halfImgHeight;

			else if (halfScreenWidth + halfImgWidth + offsetX < screenWidth)
				offsetX = halfScreenWidth - halfImgWidth;
			
		} else {
			if (halfScreenWidth + halfImgWidth + offsetX > screenWidth)
				offsetX = halfScreenWidth - halfImgWidth;

			else if (halfScreenWidth - halfImgWidth + offsetX < 0)
				offsetX = -halfScreenWidth + halfImgWidth;
		}
		
		if (imgHeight > screenHeight) {
			if (halfScreenHeight - halfImgHeight + offsetY > 0)
				offsetY = -halfScreenHeight + halfImgHeight;

			else if (halfScreenHeight + halfImgHeight + offsetY < screenHeight)
				offsetY = halfScreenHeight - halfImgHeight;
			
		} else {
			if (halfScreenHeight + halfImgHeight + offsetY > screenHeight)
				offsetY = halfScreenHeight - halfImgHeight;

			else if (halfScreenHeight - halfImgHeight + offsetY < 0)
				offsetY = -halfScreenHeight + halfImgHeight;
		}
	}

	auto ViewScene::resized(u32 width, u32 height) -> void {
		aspect.update(width, height);
		camera.setOrthoPixel(aspect.getWidth(), aspect.getHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());

		if (image != nullptr)
			fitInFrame();
		
		shouldRender = true;
	}

	auto ViewScene::update(CNGE::Input* input, CNGE::Timing* timing) -> void {
		if (image != nullptr) {
			auto currentScroll = input->getScrollY();

			if (currentScroll != 0) {
				shouldRender = true;

				zoom += currentScroll * 0.25f * zoom;

				if (zoom < 0.5f) {
					zoom = 0.5f;
				}
				else if (zoom > 4.0f) {
					zoom = 4.0f;
				}

				fitInFrame();
			}

			auto clickDown = input->getMouseDown(GLFW_MOUSE_BUTTON_1);

			if (dragging) {
				auto velocityX = input->getCursorX() - dragX;
				auto velocityY = input->getCursorY() - dragY;

				offsetX += velocityX;
				offsetY += velocityY;

				dragX = input->getCursorX();
				dragY = input->getCursorY();

				/* limit moving image to edge of screen */
				fitInFrame();

				if (!clickDown) {
					dragging = false;
				}

				shouldRender = true;

			}
			else {
				if (clickDown) {
					dragging = true;

					dragX = input->getCursorX();
					dragY = input->getCursorY();
				}
			}

			auto middleClick = input->getMousePressed(GLFW_MOUSE_BUTTON_3) == CNGE::Input::BUTTON_PRESSED;

			if (middleClick) {
				resetView();

				shouldRender = true;
			}

			if (shouldRender)
				camera.update();
		}
	}

	auto ViewScene::render(CNGE::Window* window) -> void {		
		if (shouldRender, true) {
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			if (image != nullptr) {
				auto screenWidth = (i32)aspect.getWidth();
				auto screenHeight = (i32)aspect.getHeight();

				auto imgWidth = getImageWidth();
				auto imgHeight = getImageHeight();

				auto left = (screenWidth / 2) - (imgWidth / 2) + offsetX;
				auto up = (screenHeight / 2) - (imgHeight / 2) + offsetY;

				imageTexture->bind();
				Res::textureShader.enable(CNGE::Transform::toModel(left, up, 0, imgWidth, imgHeight), camera.getProjection());
				Res::textureShader.giveParams(1, 1, 1, 1);

				Res::rect.render();	
			}
			
			window->swap();
			
			shouldRender = false;
		}
	}

	auto ViewScene::switchScene() -> CNGE::SwitchReturn {
		return noSwitch();
	}
}
