
#ifndef CNGE_SCENE
#define CNGE_SCENE

#include "../types.h"
#include "../load/loadScreen.h"
#include "../load/resource.h"
#include "../engine/loop.h"
#include "../engine/window.h"
#include "../load/bundle/resourceBundle.h"

namespace CNGE {
    struct SwitchReturn;
	
	class Scene {	
	private:
        ResourceBundle* bundle;

	protected:
        static auto noSwitch() -> SwitchReturn;
		
	public:
        virtual ~Scene() = default;
        explicit Scene(ResourceBundle* bundle);

        virtual auto start() -> void = 0;

        virtual auto resized(u32 width, u32 height) -> void = 0;

        virtual auto update(Input* input, Timing* time) -> void = 0;

        virtual auto render(Window* window) -> void = 0;

        virtual auto switchScene() -> SwitchReturn = 0;

        [[nodiscard]] auto getBundle() const -> ResourceBundle*;
	};

	struct SwitchReturn {
		std::unique_ptr<Scene> newScene;
		std::unique_ptr<LoadScreen> newLoadScreen;
	};
}

#endif
