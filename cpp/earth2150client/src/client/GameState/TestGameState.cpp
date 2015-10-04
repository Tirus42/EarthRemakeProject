#include "client/GameState/TestGameState.h"

#include "client/VisualMap.h"
#include "client/MapMarker.h"
#include "client/MousePicker.h"
#include "GUI/IngameGUI.h"
#include "renderer/CompatibleScreenRenderer.h"
#include "GUI/ResizeEvent.h"
#include "renderer/DeferredShadingScreenRenderer.h"

#include "tests/FlyingObjects.h"

#include "renderer/LightData/SPointLightData.h"

#include "tf/time.h"

#include "client/EngineData.h"
#include <irrlicht.h>

#include <cassert>

using namespace irr;

using namespace core;

TestGameState::TestGameState(EngineData& engineData, bool testCreateFlyingObjects) :
	AbstractGameState(engineData),
	camera(0),
	mousePosition(),
	cursorControl(engineData.getIrrlichtDevice()->getCursorControl()),
	subEventReceiver(0),
	testFlyingObjects(testCreateFlyingObjects) {

	assert(engineData.getIrrlichtDevice() != 0);

	cursorControl->grab();
}

TestGameState::~TestGameState() {
	cursorControl->drop();
	removeCamera();
}

AbstractGameState* TestGameState::run() {
	IrrlichtDevice* device = engineData.getIrrlichtDevice();

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	gui::IGUIFont* defaultFont = guienv->getBuiltInFont();

	 // Kleines Text Feld zur FPS Anzeige erstellen
	gui::IGUIStaticText* fpsDisplay = guienv->addStaticText(L"FPS: -", recti(10,10,100,22), true);
	gui::IGUIStaticText* frameTimeDisplay = guienv->addStaticText(L"", recti(10, 25, 120, 37), true);
	gui::IGUIStaticText* renderTimeDisplay = guienv->addStaticText(L"", recti(10, 40, 120, 52), true);

	// Irrlicht Default Font setzen, da diese schön klein ist
	fpsDisplay->setOverrideFont(defaultFont);
	frameTimeDisplay->setOverrideFont(defaultFont);
	renderTimeDisplay->setOverrideFont(defaultFont);

	// Erstelle Kamera
	createCamera(smgr);

	// Karte erstellen
	VisualMap map(driver, smgr, 1024, 1024);

	{	// Material setzen
		video::SMaterial mat;

		//mat.BackfaceCulling = false;
		mat.setTexture(0, driver->getTexture("sand1.jpg"));

		if (!mat.getTexture(0))
			printf("Konnte Textur nicht laden!\n");

		mat.AmbientColor.set(255,255,255,255);
		mat.DiffuseColor.set(255,255,255,255);
		mat.EmissiveColor.set(127,127,127,127);

		map.addMaterial(mat);
	}

	// Raw-Heightmap laden
	if (!map.loadHeightMapRAW("GameData/map1024x1024.bin"))
		return 0;

	// Komplette Map als Mesh aufbauen
	map.build();

	// Lichtquelle erstellen, Radius und Typ festlegen
	scene::ILightSceneNode* light = smgr->addLightSceneNode();
	light->setRadius(1024);
	light->setLightType(video::ELT_DIRECTIONAL);

	// Dieses Licht automatisch Rotieren lassen (Ausrichtung ändern, da Direktionales Licht)
	scene::ISceneNodeAnimator * rotation = smgr->createRotationAnimator(vector3df(0, 0.2f, 0));
	light->addAnimator(rotation);
	rotation->drop();	// Wir brauchen das Handle nicht mehr, das Objekt (light) hat nun selbst eine referenz darauf

	// Erstelle Ingame GUI
	IngameGUI gui(guienv, device, camera);
	gui.setMapName(L"Testkarte");

	// Setzte GUI als Event Receiver
	setSubEventReceiver(&gui);

	// Testweiße Marker für die Maus Position erstellen (siehe Hauptschleife)
	video::ITexture* tex = driver->getTexture("position.png");	// Textur Laden

	// Material für das Rendern bestimmen
	video::SMaterial m;

	m.AmbientColor.set(255,255,255,255);
	m.Lighting = false;
	m.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	m.setTexture(0, tex);

	// Den Marker mit dem Material erstellen/holen
	MapMarker* marker = map.getMapMarkerManager().getMarkerForMaterial(m);

	IScreenRenderer* renderer;

	bool useCompatibleRenderer = engineData.getConfig().getUseCompatibilityRenderer();

	if (useCompatibleRenderer) {
		renderer = new CompatibleScreenRenderer(device, video::SColor(0, 200, 200, 200));
	} else {
		renderer = new DeferredShadingScreenRenderer(device, video::SColor(0, 255, 255, 255));
	}

	renderer->init();

	if (!useCompatibleRenderer) {
		const video::SMaterial mat = ((DeferredShadingScreenRenderer*)renderer)->getMaterial(DeferredShadingScreenRenderer::SHADER_MAP);

		map.getMaterial(0).MaterialType = mat.MaterialType;
		map.updateMaterial();
	}

	/// -- Test mit Fliegenden Würfeln --------
	FlyingObjects flyingObjectsTest(map, smgr);
	if (testFlyingObjects) {
		const int countObjects = 100;

		scene::IMeshSceneNode* cube = smgr->addCubeSceneNode();
		cube->getMaterial(0) = map.getMaterial(0);

		flyingObjectsTest.createRotatingObjects(cube, core::vector2df(256, 256), 128.f, 0.0001f, 10);
		flyingObjectsTest.createRotatingObjects(cube, core::vector2df(256, 768), 200.f, 0.0001f, 20);
		flyingObjectsTest.createRotatingObjects(cube, core::vector2df(512, 512), 386, 0.0001f, countObjects);

        cube->remove();
	}
	/// ----------------------------------

	// Ausgabevariablen von der Zeitmessung
	uint64_t lastRenderTime = 0;
	uint64_t lastFrameTime = 0;

	uint64_t startTimeFrame;
	uint64_t endTimeFrame;


	/// ------------------ Lichtquellen einfügen
	{
		const core::array<scene::ISceneNode*>& nodeArray = flyingObjectsTest.getNodeArray();

		LightManager& lightManager = renderer->getLightManager();

		srand(42);

		for (u32 i = 0; i < nodeArray.size(); ++i) {
			lightManager.addPointLight(nodeArray[i]->getAbsolutePosition(), video::SColor(255, rand() % 0xFF, rand() % 0xFF, rand() % 0xFF), 50);

		}
	}
	/// -----------------------------

	core::recti currentResolution = device->getVideoDriver()->getViewPort();

	MousePicker mPicker(map, smgr->getSceneCollisionManager());

	// Hauptschleife
	while (device->run()) {
		// Testweise hier prüfen ob sich die Auflösung geändert hat
		if (currentResolution != device->getVideoDriver()->getViewPort()) {
			currentResolution = device->getVideoDriver()->getViewPort();

			renderer->resize(core::dimension2du(currentResolution.getWidth(), currentResolution.getHeight()));
		}

		HighResolutionTime(&startTimeFrame);

		if (testFlyingObjects) {
			flyingObjectsTest.update();

			const core::array<scene::ISceneNode*>& nodes = flyingObjectsTest.getNodeArray();

			LightManager& lightManager = renderer->getLightManager();

			for (u32 i = 0; i < nodes.size(); ++i) {
				SPointLightData& pLight = lightManager.getPointLight(i);

				pLight.position = nodes[i]->getAbsolutePosition();
			}
		}

		renderer->render(map);


		// Map Seletor Test
		if (!gui.isMouseOverGUIElement(mousePosition)) {
			if (mPicker.pick(mousePosition)) {
				marker->clear();

				if (mPicker.getPickedType() == MousePicker::PickedGround)
					marker->addField(mPicker.getPickedPosition());
			}

		}


		{	// Schreibe die aktuellen FPS Werte neu in das GUI Element (Textfeld)
			core::stringw text("FPS: ");
			text += driver->getFPS();

			fpsDisplay->setText(text.c_str());
		}{
			core::stringw text("FrameTime:  ");
			text += (u32)(lastFrameTime / 1000);
			text += L"µs";

			frameTimeDisplay->setText(text.c_str());
		}{
			core::stringw text("RenderTime: ");
			text += (u32)(lastRenderTime / 1000);
			text += L"µs";

			renderTimeDisplay->setText(text.c_str());
		}

		HighResolutionTime(&endTimeFrame);

		lastRenderTime = renderer->getLastRenderTime();
		lastFrameTime = HighResolutionDiffNanoSec(startTimeFrame, endTimeFrame);

		static bool windowActive = true;

		// Limitiere FPS Rate wenn Fenster keinen Fokus hat und in der Config ein Wert angegeben ist
		if (!device->isWindowActive()) {
			const u32 inactiveFPSLimit = engineData.getConfig().getInactiveFPSLimit();

			if (inactiveFPSLimit != 0) {
				device->sleep(1000 / inactiveFPSLimit);
				device->run();
			}
		}

		// Wenn Fenster inaktiv war dann Shader neu Laden lassen (zu debugging zwecken)
		if (!windowActive && device->isWindowActive()) {
			renderer->init();
		}

		windowActive = device->isWindowActive();
	}

	delete renderer;

	return 0;
}


bool TestGameState::OnEvent(const irr::SEvent& event) {
	// Bei betätigen der rechten Maustaste zwischen Maus fangen umschalten
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP) {

		// Setzte Maus in Mitte um unnötigen Kameraschwenk zu vermeiden
		cursorControl->setPosition(0.5f, 0.5f);

		// Toggle Maus fangen
		camera->setInputReceiverEnabled(!camera->isInputReceiverEnabled());

		return true;
	}

	// Speichere Maus Position für weitere behandlung
	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT &&
		event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {

		mousePosition.X = event.MouseInput.X;
		mousePosition.Y = event.MouseInput.Y;
	}

	if (ResizeEvent::isResizeEvent(event)) {
		const core::dimension2du newSize = ResizeEvent::getResizeEventData(event);

		// Setze Aspekt Ratio der Kamera auf neue Auflösung
		camera->setAspectRatio((float)newSize.Width / (float)newSize.Height);

		return true;
	}

	if (subEventReceiver != 0)
		return subEventReceiver->OnEvent(event);

	return false;
}

void TestGameState::createCamera(irr::scene::ISceneManager* smgr) {
	// Tastenbelegung für die Kamera zuweißen
	SKeyMap keymap[8];

	// Vorwärts
	keymap[0].Action = keymap[1].Action = EKA_MOVE_FORWARD;
	keymap[0].KeyCode = KEY_UP; keymap[1].KeyCode = KEY_KEY_W;

	// Rückwärts
	keymap[2].Action = keymap[3].Action = EKA_MOVE_BACKWARD;
	keymap[2].KeyCode = KEY_DOWN; keymap[3].KeyCode = KEY_KEY_S;

	// Nach rechts
	keymap[4].Action = keymap[5].Action = EKA_STRAFE_RIGHT;
	keymap[4].KeyCode = KEY_RIGHT; keymap[5].KeyCode = KEY_KEY_D;

	// Nach links
	keymap[6].Action = keymap[7].Action = EKA_STRAFE_LEFT;
	keymap[6].KeyCode = KEY_LEFT; keymap[7].KeyCode = KEY_KEY_A;

	// FirstPerson Kamera erstellen (Steuerung mit Maus + Pfeiltasten)
	camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.25f, -1, keymap, 8);

	camera->setPosition(core::vector3df(100, 100, 100));
	camera->setTarget(core::vector3df(512, 0, 512));
}

void TestGameState::removeCamera() {
	camera->remove();
	camera = 0;
}

void TestGameState::setSubEventReceiver(irr::IEventReceiver* receiver) {
	subEventReceiver = receiver;
}
