
#include "viewScene.h"

#include "cnge/math/math.h"
#include "cnge/engine/transform.h"
#include "ebetView/res.h"

#include <iostream>

namespace Game {
	ViewScene::ViewScene(std::string&& inputFile) :
		Scene(&Res::viewResources),
		backgroundColor(0x37393f),
		imageTexture(nullptr),
		image(nullptr),
		inputFile(std::move(inputFile)),
		dragX(0),
		dragY(0),
		offsetX(0),
		offsetY(0),
		zoom(0),
		dragging(false)
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
		return i32(roundf(image->getWidth() * zoom / 2.0f)) * 2;
	}
	
	auto ViewScene::getImageHeight() -> i32 {
		return i32(roundf(image->getHeight() * zoom / 2.0f)) * 2;
	}

	auto ViewScene::fitInFrame() -> void {
		const auto screenWidth = i32(aspect.getWidth());
		const auto screenHeight = i32(aspect.getHeight());

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
		
		setShouldRender(true);
	}

	auto ViewScene::update(CNFW::Input* input, CNFW::Timing* timing) -> void {
		if (image != nullptr) {
			auto const currentScroll = input->getScroll();

			if (currentScroll != 0) {
				zoom += currentScroll * 0.25f * zoom;

				if (zoom < 0.5f) {
					zoom = 0.5f;
				}
				else if (zoom > 4.0f) {
					zoom = 4.0f;
				}

				fitInFrame();

				setShouldRender(true);
			}

			auto const clickDown = input->getButtonDown(CNFW::Input::BUTTON_LEFT);

			if (dragging) {
				offsetX += input->getMouseX() - dragX;
				offsetY += input->getMouseY() - dragY;

				dragX = input->getMouseX();
				dragY = input->getMouseY();

				/* limit moving image to edge of screen */
				fitInFrame();

				if (!clickDown)
					dragging = false;

				setShouldRender(true);
				
			} else if (clickDown) {
				dragging = true;

				dragX = input->getMouseX();
				dragY = input->getMouseY();
			}

			auto const middleClick = input->getButtonPressed(CNFW::Input::BUTTON_MIDDLE);

			if (middleClick) {
				resetView();

				setShouldRender(true);
			}

			camera.update();
		}
	}

	auto ViewScene::render() -> void {
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		if (image != nullptr) {
			const auto screenWidth = i32(aspect.getWidth());
			const auto screenHeight = i32(aspect.getHeight());

			const auto halfScreenWidth = screenWidth / 2;
			const auto halfScreenHeight = screenHeight / 2;

			const auto imgWidth = getImageWidth();
			const auto imgHeight = getImageHeight();

			const auto halfImgWidth = imgWidth / 2;
			const auto halfImgHeight = imgHeight / 2;

			imageTexture->bind();
			Res::textureShader.enable(CNGE::Transform::toModel(halfScreenWidth - halfImgWidth + offsetX, halfScreenHeight - halfImgHeight + offsetY, 0, imgWidth, imgHeight), camera.getProjection());
			Res::textureShader.giveParams(1, 1, 1, 1);

			Res::rect.render();
		}
	}

	auto ViewScene::switchScene() -> CNGE::SwitchReturn {
		return noSwitch();
	}
}
