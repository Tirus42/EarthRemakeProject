#include <irrlicht.h>
#include "driverChoice.h"

#include "client/VisualMap.h"

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

/*
This is the main method. We can use void main() on every platform.
On Windows platforms, we could also use the WinMain method
if we would want to get rid of the console window, which pops up when
starting a program with main(), but to keep this example simple,
we use main().
*/
int main(int argc, char** argv) {

	video::E_DRIVER_TYPE driverType = driverChoiceConsole(false);

	if (driverType == video::EDT_COUNT)
		return -1;

    IrrlichtDevice *device =
        createDevice(driverType, dimension2d<u32>(1024, 768));

    /*
    Get a pointer to the video driver, the SceneManager and the
    graphical user interface environment, so that
    we do not always have to write device->getVideoDriver(),
    device->getSceneManager() and device->getGUIEnvironment().
    */
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

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

    /*
    To look at the mesh, we place a camera into 3d space at the position
    (0, 30, -40). The camera looks from there to (0,5,0).
    */
    //smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
	cam->setFarValue(100000.0f);

	VisualMap map(driver, 1024, 1024);

	if (!map.loadHeightMapRAW("map1024x1024.bin"))
		return EXIT_FAILURE;

	map.build(smgr);


	/// Testweiße und zur Orientierung einen Cube hinzufügen
	scene::ISceneNode* cube = smgr->addCubeSceneNode(10);


	scene::ILightSceneNode* light = smgr->addLightSceneNode();
	light->setRadius(1024);

	light->setLightType(video::ELT_DIRECTIONAL);

	scene::ISceneNodeAnimator * rotation = smgr->createRotationAnimator(vector3df(0, 0.2f, 0));
	light->addAnimator(rotation);

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

