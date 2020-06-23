
#include "scene.h"

namespace CNGE {	
	auto Scene::noSwitch() -> SwitchReturn {
		return {nullptr, nullptr};
	}

	auto Scene::setShouldRender(bool const shouldRender) -> void {
		this->shouldRender = shouldRender;
	}

	Scene::Scene(ResourceBundle* bundle)
		: bundle(bundle), shouldRender(false) {}

	auto Scene::getBundle() const -> ResourceBundle* {
		return bundle;
	}

	auto Scene::getShouldRender() const -> bool {
		return shouldRender;
	}
}
