#include <irrlicht.h>

#include "client/VisualMap.h"
#include "client/MapMarker.h"
#include "GUI/IngameGUI.h"
#include "GUI/IngameGUIEventReceiver.h"

#include "config/ClientConfig.h"

#include "tf/time.h"

using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look
at them, you can read a detailed description of them in the
documentation by clicking on the top menu item 'Namespace List'
or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
Like the irr Namespace, we do not want these 5 sub namespaces now,
to keep this example simple. Hence we tell the compiler again
that we do not want always to write their names:
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// Temp, Todo: In eigene Klasse Packen
static const irr::s32 ID_MAPPICK = 1 << 0;

scene::ICameraSceneNode* cam;

// Kleine Eventhandler Klasse, um das Fangen der Maus Umschalten zu können
class CamMouseDisabler : public IEventReceiver {
	private:
		IEventReceiver* recv;

		CamMouseDisabler(const CamMouseDisabler&);
		CamMouseDisabler operator=(const CamMouseDisabler&);
	public:
		core::position2d<s32> mousePosition;

		CamMouseDisabler() : recv(0), mousePosition() {}

		void setSubEventReceiver(IEventReceiver* receiver) {
			recv = receiver;
		}

		bool OnEvent(const SEvent& event) {
			if (event.EventType == irr::EET_MOUSE_INPUT_EVENT &&
				event.MouseInput.isRightPressed()) {
				cam->setInputReceiverEnabled(!cam->isInputReceiverEnabled());

				return true;
			}
			// Bei Mausbewegung aktuelle Position speichern
			else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT &&
				event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {

				mousePosition.X = event.MouseInput.X;
				mousePosition.Y = event.MouseInput.Y;
			}

			if (recv != 0)
				return recv->OnEvent(event);

			return false;
		}


};

int main(int argc, char** argv) {

	ClientConfig config;

	// Hochauflösenden Timer initiieren (für exakte Zeitmessungen)
	InitHighResolutionTimer();

	{	// Config Datei Laden wenn vorhanden, andernfalls mit Standardwerten erstellen.
		IrrlichtDevice *device = createDevice(EDT_NULL);

		IXMLReader *configReader = device->getFileSystem()->createXMLReader("clientconfig.xml");

		if (configReader) {
			config.loadXMLFile(configReader);

			configReader->drop();
		} else {
			config.setDefaultValues();

			IXMLWriter *configWriter = device->getFileSystem()->createXMLWriter("clientconfig.xml");

			if (configWriter) {
				config.saveXMLFile(configWriter);

				configWriter->drop();
			}
		}

		device->drop();
	}

    IrrlichtDevice *device = createDeviceEx(config.Parameter());

	// Wenn Device nicht gestartet werden konnte, dann abbrechen
	if (!device)
		return EXIT_FAILURE;

    /*
    Get a pointer to the video driver, the SceneManager and the
    graphical user interface environment, so that
    we do not always have to write device->getVideoDriver(),
    device->getSceneManager() and device->getGUIEnvironment().
    */
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

	// Mache Fenster frei Skalierbar
	device->setResizable(true);

	core::stringw caption("Earth 2150 Remake Projekt - ");
	caption += driver->getName();

	device->setWindowCaption(caption.c_str());

    // Kleines Text Feld zur FPS Anzeige erstellen
    IGUIStaticText* fpsDisplay = guienv->addStaticText(L"FPS: -", rect<int>(10,10,100,22), true);
    IGUIStaticText* frameTimeDisplay = guienv->addStaticText(L"", rect<s32>(10, 25, 100, 37), true);
    IGUIStaticText* renderTimeDisplay = guienv->addStaticText(L"", rect<s32>(10, 40, 100, 52), true);


    // FirstPerson Kamera erstellen (Steuerung mit Maus + Pfeiltasten)
    cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.25f);

	// Karte erstellen
	VisualMap map(driver, smgr, 1024, 1024);

	// Raw-Heightmap laden
	if (!map.loadHeightMapRAW("map1024x1024.bin"))
		return EXIT_FAILURE;

	// Selections-ID setzen
	map.setMeshID(ID_MAPPICK);

	// Komplette Map als Mesh aufbauen
	map.build();

	/// Testweiße und zur Orientierung einen Cube hinzufügen
	scene::ISceneNode* cube = smgr->addCubeSceneNode(10);

	// Lichtquelle erstellen, Radius und Typ festlegen
	scene::ILightSceneNode* light = smgr->addLightSceneNode();
	light->setRadius(1024);
	light->setLightType(video::ELT_DIRECTIONAL);

	// Dieses Licht automatisch Rotieren lassen (Ausrichtung ändern, da Direktionales Licht)
	scene::ISceneNodeAnimator * rotation = smgr->createRotationAnimator(vector3df(0, 0.2f, 0));
	light->addAnimator(rotation);

	/*
	* Objekte erstellen die nötig sind, um den Kollisionspunkt von der
	* Maus Position auf dem Bildschirm die darunter liegende Karte zu finden
	* Das getroffene Dreieck wird (Testweiße) Rot eingezeichnet).
	*/
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
	// Tracks the current intersection point with the level or a mesh
	core::vector3df intersection;
	// Used to show with triangle has been hit
	core::triangle3df hitTriangle;

	// Material mit dem gezeichnet werden soll (Nur Drahtgitter und ohne Beleuchtung)
	video::SMaterial matWireframe;
	matWireframe.Lighting = false;
	matWireframe.setTexture(0, 0);
	matWireframe.Wireframe = true;

	// Erstelle Ingame GUI
	IngameGUI gui(guienv, cam);

	// Setze eigenen EventReceiver, um die Kamera-Steuerung unterbinden zu können.
	CamMouseDisabler* mouseHandler = new CamMouseDisabler();
	device->setEventReceiver(mouseHandler);

	// Füge in diesen EventReceiver den GUI-EventReceiver ein (Themoräre Lösung...)
	mouseHandler->setSubEventReceiver(new IngameGUIEventReceiver(&gui));


	// Testweiße eine Markierung auf der Karte plazieren
	//{
		video::ITexture* tex = driver->getTexture("position.png");
		video::SMaterial m;

		m.AmbientColor.set(255,255,255,255);
		m.Lighting = false;
		m.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

		m.MaterialTypeParam = video::pack_textureBlendFunc(video::EBF_SRC_ALPHA, video::EBF_ONE_MINUS_SRC_ALPHA, video::EMFN_MODULATE_1X, video::EAS_TEXTURE | video::EAS_VERTEX_COLOR);
		m.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_BORDER;
		m.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_BORDER;

		m.setTexture(0, tex);

		//video::pack_textureBlendFunc()

		MapMarker* marker = map.getMapMarkerManager().getMarkerForMaterial(m);
		marker->addField(MapPosition(5, 5));
		marker->addField(MapPosition(7, 5));
		marker->addField(MapPosition(5, 1020));
		marker->addField(MapPosition(7, 1020));
	//}

	// Ausgabevariablen von der Zeitmessung
	uint64_t lastRenderTime = 0;
	uint64_t lastFrameTime = 0;

	uint64_t startTime;
	uint64_t endTimeRender;
	uint64_t endTimeFrame;

    // Hauptschleife
    while (device->run()) {
		HighResolutionTime(&startTime);

        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();

        HighResolutionTime(&endTimeRender);

		// Mesh Seletor Test
		core::line3d<f32> ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(mouseHandler->mousePosition, cam);

		MapPosition pos = map.pickMapPosition(ray.start, ray.getVector());

		if (pos.isValid()) {
			marker->clear();
			marker->addField(pos);
		}

		scene::ISceneNode * selectedSceneNode = 0;
			/*collMan->getSceneNodeAndCollisionPointFromRay(
					ray,
					intersection, // This will be the position of the collision
					hitTriangle, // This will be the triangle hit in the collision
					ID_MAPPICK, // This ensures that only nodes that we have
							// set up to be pickable are considered
					0); // Check the entire scene (this is actually the implicit default)
		*/

		// Wenn durch das Tracing eine Stelle auf der Map gefunden wurde, Zeichne das
		// getroffene Dreieck rot ein.
		/*if (selectedSceneNode) {
			driver->setTransform(video::ETS_WORLD, core::matrix4());
			driver->setMaterial(matWireframe);
			driver->draw3DTriangle(hitTriangle, video::SColor(0, 255, 0, 0));
		}*/

		//



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

		lastRenderTime = HighResolutionDiffNanoSec(startTime, endTimeRender);
		lastFrameTime = HighResolutionDiffNanoSec(startTime, endTimeFrame);

		// Wenn das Fenster den Fokus verliert, dann nicht weiter rendern
        while (!device->isWindowActive()) {
			device->sleep(10);
			device->run();
        }
    }

    /*
    After we are finished, we have to delete the Irrlicht Device
    created before with createDevice(). In the Irrlicht Engine,
    you will have to delete all objects you created with a method or
    function which starts with 'create'. The object is simply deleted
    by calling ->drop().
    See the documentation at
    http://irrlicht.sourceforge.net//docu/classirr_1_1IUnknown.html#a3
    for more information.
    */
    device->drop();

    return 0;
}

