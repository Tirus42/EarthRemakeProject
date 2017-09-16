cmake_minimum_required (VERSION 2.6)

project (Earth2150_Client)

include_directories("include")
include_directories("${EARTH2150_PATH}/include")
include_directories("${IRRLICHT_ENGINE_INCLUDE}")

link_directories("${IRRLICHT_ENGINE_LIB}")

add_executable(Earth2150_Client
	src/config/ClientConfig.cpp								include/config/ClientConfig.h

	include/renderer/IScreenRenderer.h
	src/renderer/RenderViewport.cpp							include/renderer/RenderViewport.h
	src/renderer/LightManager.cpp							include/renderer/LightManager.h
	src/renderer/ScreenRendererHelper.cpp					include/renderer/ScreenRendererHelper.h
	src/renderer/DeferredShadingScreenRenderer.cpp			include/renderer/DeferredShadingScreenRenderer.h
	src/renderer/CompatibleScreenRenderer.cpp				include/renderer/CompatibleScreenRenderer.h

	include/renderer/LightData/SPointLightData.h

	include/renderer/ShaderCallback/GlobalLightCallback.h
	include/renderer/ShaderCallback/PointLightCallback.h
	include/renderer/ShaderCallback/TerrainShaderCallback.h

	src/client/main.cpp
	src/client/EngineData.cpp								include/client/EngineData.h
	src/client/MapGlobalLight.cpp							include/client/MapGlobalLight.h
	src/client/MousePicker.cpp								include/client/MousePicker.h
	src/client/VisualMap.cpp								include/client/VisualMap.h
	src/client/VisualMapPart.cpp							include/client/VisualMapPart.h
	src/client/MapMarker.cpp								include/client/MapMarker.h
	src/client/MapMarkerManager.cpp							include/client/MapMarkerManager.h
	src/client/AbstractGameState.cpp						include/client/AbstractGameState.h

	src/client/GameState/MainMenu.cpp						include/client/GameState/MainMenu.h
	src/client/GameState/ProfileSelect.cpp					include/client/GameState/ProfileSelect.h
	src/client/GameState/GameStateFactory.cpp				include/client/GameState/GameStateFactory.h
	src/client/GameState/TestGameState.cpp					include/client/GameState/TestGameState.h

	include/GUI/IGUI.h
	include/GUI/IGUIWindow.h
	include/GUI/ResizeEvent.h
	include/GUI/BasicGameInterface.h
	src/GUI/IngameGUI.cpp									include/GUI/IngameGUI.h
	src/GUI/ResearchWindow.cpp								include/GUI/ResearchWindow.h
	src/GUI/BasicGameInterface.cpp							include/GUI/BasicGameInterface.h
	src/GUI/GUIElementEventHandler.cpp						include/GUI/GUIElementEventHandler.h

	src/tests/FlyingObjects.cpp								include/tests/FlyingObjects.h
)

target_link_libraries(Earth2150_Client LibEarth2150 LibFF LibTF Irrlicht)
