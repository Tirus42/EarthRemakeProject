cmake_minimum_required (VERSION 2.6)

project (Earth2150_Client)

include_directories("include")
include_directories("${EARTH2150_PATH}/include")
include_directories("${IRRLICHT_ENGINE_INCLUDE}")

link_directories("${IRRLICHT_ENGINE_LIB}")

add_executable(Earth2150_Client
	src/config/ClientConfig.cpp
	src/renderer/RenderViewport.cpp
	src/renderer/LightManager.cpp
	src/renderer/ScreenRendererHelper.cpp
	src/renderer/DeferredShadingScreenRenderer.cpp
	src/renderer/CompatibleScreenRenderer.cpp
	src/client/MapMarkerManager.cpp
	src/client/EngineData.cpp
	src/client/GameState/MainMenu.cpp
	src/client/GameState/ProfileSelect.cpp
	src/client/GameState/GameStateFactory.cpp
	src/client/GameState/TestGameState.cpp
	src/client/MapGlobalLight.cpp
	src/client/VisualMap.cpp
	src/client/MousePicker.cpp
	src/client/VisualMapPart.cpp
	src/client/main.cpp
	src/client/AbstractGameState.cpp
	src/client/MapMarker.cpp
	src/GUI/IngameGUI.cpp
	src/GUI/ResearchWindow.cpp
	src/GUI/BasicGameInterface.cpp
	src/GUI/GUIElementEventHandler.cpp
	src/tests/FlyingObjects.cpp
)

target_link_libraries(Earth2150_Client LibEarth2150 LibFF LibTF Irrlicht)

