
#ifndef CNGE_SCENE
#define CNGE_SCENE

#include "types.h"
#include "cnge/load/loadScreen.h"
#include "cnge/load/resource.h"
#include "cnfw/window.h"
#include "cnfw/timing.h"
#include "cnge/load/bundle/resourceBundle.h"

namespace CNGE {
    struct SwitchReturn;
	
	class Scene {	
	private:
        ResourceBundle* bundle;

		bool shouldRender;
		
	protected:
        static auto noSwitch() -> SwitchReturn;
		
	public:
        virtual ~Scene() = default;
        explicit Scene(ResourceBundle* bundle);

        virtual auto start() -> void = 0;

        virtual auto resized(u32 width, u32 height) -> void = 0;

        virtual auto update(CNFW::Input* input, CNFW::Timing* time) -> void = 0;

        virtual auto render() -> void = 0;

        virtual auto switchScene() -> SwitchReturn = 0;

		auto setShouldRender(bool) -> void;
		
        [[nodiscard]] auto getBundle() const -> ResourceBundle*;
		[[nodiscard]] auto getShouldRender() const -> bool;
	};

	struct SwitchReturn {
		std::unique_ptr<Scene> newScene;
		std::unique_ptr<LoadScreen> newLoadScreen;
	};
}

#endif
