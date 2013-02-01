#include <irrlicht.h>
#include "driverChoice.h"

#include "client/VisualMap.h"
#include "GUI/IngameGUI.h"
#include "GUI/IngameGUIEventReceiver.h"

/*
In the Irrlicht Engine, everything can be found in the namespace
'irr'. So if you want to use a class of the engine, you have to
write an irr:: before the name of the class. For example to use
the IrrlichtDevice write: irr::IrrlichtDevice. To get rid of the
irr:: in front of the name of every class, we tell the compiler
that we use that namespace from now on, and we will not have to
write that 'irr::'.
*/
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

	public:
		core::position2d<s32> mousePosition;

		CamMouseDisabler() : recv(0) {}

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

/*
This is the main method. We can use void main() on every platform.
On Windows platforms, we could also use the WinMain method
if we would want to get rid of the console window, which pops up when
starting a program with main(), but to keep this example simple,
we use main().
*/
int main(int argc, char** argv) {

	video::E_DRIVER_TYPE driverType = driverChoiceConsole();

	if (driverType == video::EDT_COUNT)
		return -1;

    IrrlichtDevice *device =
        createDevice(driverType, dimension2d<u32>(1024, 768));

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

    /*
    We add a hello world label to the window, using the GUI environment.
    */
    IGUIStaticText* fpsDisplay = guienv->addStaticText(L"FPS: -", rect<int>(10,10,100,22), true);


    //IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
    IAnimatedMeshSceneNode* node = 0; //smgr->addAnimatedMeshSceneNode( mesh );

    if (node) {
        node->setMaterialFlag(EMF_LIGHTING, false);
        node->setFrameLoop(0, 310);
        node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
    }

    // FirstPerson Kamera erstellen (Steuerung mit Maus + Pfeiltasten)
    cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.25f);

	// Karte erstellen
	VisualMap map(driver, 1024, 1024);

	// Raw-Heightmap laden
	if (!map.loadHeightMapRAW("map1024x1024.bin"))
		return EXIT_FAILURE;

	// Selections-ID setzen
	map.setMeshID(ID_MAPPICK);

	// Komplette Map als Mesh aufbauen
	map.build(smgr);

	/// Testweiße und zur Orientierung einen Cube hinzufügen
	scene::ISceneNode* cube = smgr->addCubeSceneNode(10);


	scene::ILightSceneNode* light = smgr->addLightSceneNode();
	light->setRadius(1024);

	light->setLightType(video::ELT_DIRECTIONAL);

	scene::ISceneNodeAnimator * rotation = smgr->createRotationAnimator(vector3df(0, 0.2f, 0));
	light->addAnimator(rotation);


	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
	// Tracks the current intersection point with the level or a mesh
	core::vector3df intersection;
	// Used to show with triangle has been hit
	core::triangle3df hitTriangle;

	video::SMaterial matWireframe;
	matWireframe.Lighting = false;
	matWireframe.setTexture(0, 0);
	matWireframe.Wireframe = true;

	// Erstelle Ingame GUI
	IngameGUI gui(guienv);

	// Setze eigenen EventReceiver, um die Kamera-Steuerung unterbinden zu können.
	CamMouseDisabler* mouseHandler = new CamMouseDisabler();
	device->setEventReceiver(mouseHandler);

	// Füge in diesen EventReceiver den GUI-EventReceiver ein (Themoräre Lösung...)
	mouseHandler->setSubEventReceiver(new IngameGUIEventReceiver(&gui));

    /*
    Ok, now we have set up the scene, lets draw everything:
    We run the device in a while() loop, until the device does not
    want to run any more. This would be when the user closed the window
    or pressed ALT+F4 in windows.
    */
    while (device->run()) {
        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

		// Mesh Seletor Test
		core::line3d<f32> ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(mouseHandler->mousePosition, cam);

		scene::ISceneNode * selectedSceneNode =
			collMan->getSceneNodeAndCollisionPointFromRay(
					ray,
					intersection, // This will be the position of the collision
					hitTriangle, // This will be the triangle hit in the collision
					ID_MAPPICK, // This ensures that only nodes that we have
							// set up to be pickable are considered
					0); // Check the entire scene (this is actually the implicit default)

		if (selectedSceneNode) {
			driver->setTransform(video::ETS_WORLD, core::matrix4());
			driver->setMaterial(matWireframe);
			driver->draw3DTriangle(hitTriangle, video::SColor(0, 255, 0, 0));
		}

		//

        driver->endScene();

		{
			core::stringw text("FPS: ");
			text += driver->getFPS();

			fpsDisplay->setText(text.c_str());
		}


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

