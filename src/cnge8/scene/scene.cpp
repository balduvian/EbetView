
#include "scene.h"

namespace CNGE {	
	auto Scene::noSwitch() -> SwitchReturn {
		return {nullptr, nullptr};
	}

	Scene::Scene(ResourceBundle* bundle)
		: bundle(bundle) {}

	auto Scene::getBundle() const -> ResourceBundle* {
		return bundle;
	}
}
