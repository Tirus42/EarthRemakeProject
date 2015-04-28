#include "config/ClientConfig.h"

using namespace irr;

ClientConfig::ClientConfig() :
	param(),
	useCompatibilityRenderer(false),
	path_TheMoonProject(),
	path_LostSouls(),
	inactiveFPSLimit(5),
	latestConfigVersion(true) {
}

void ClientConfig::setDefaultValues() {
	// Erstelle neu mit Irrlicht Standard einstellungen
	param = SIrrlichtCreationParameters();

	// Setze eigene Werte
	param.DriverType 	= video::EDT_OPENGL;
	param.Stencilbuffer = true;
	param.WindowSize	= core::dimension2d<u32>(1024, 768);
	param.Bits			= 32;
	param.Fullscreen	= false;

	path_TheMoonProject = "";
	path_LostSouls = "";

	inactiveFPSLimit = 5;

	latestConfigVersion = true;
}

void ClientConfig::saveXMLFile(io::IXMLWriter* writer) const {
	writer->writeXMLHeader();
	writer->writeLineBreak();

	writer->writeElement(L"config");
	writer->writeLineBreak();

	writer->writeComment(L"type = {null, opengl, dx8, dx9, software}");
	writer->writeLineBreak();
	writer->writeElement(L"driver", true,
						L"type", DriverName(param.DriverType),
						L"antialias", core::stringw(param.AntiAlias).c_str(),
						L"stencilbuffer", core::stringw(param.Stencilbuffer).c_str(),
                        L"stereobuffer", core::stringw(param.Stereobuffer).c_str(),
						L"multithreaded", core::stringw(param.DriverMultithreaded).c_str()
						);
	writer->writeLineBreak();

	writer->writeElement(L"screen", true,
						L"width", core::stringw(param.WindowSize.Width).c_str(),
						L"height", core::stringw(param.WindowSize.Height).c_str(),
						L"bit", core::stringw(param.Bits).c_str(),
						L"fullscreen", core::stringw(param.Fullscreen).c_str(),
						L"vsync", core::stringw(param.Vsync).c_str()
						);
	writer->writeLineBreak();

	writer->writeElement(L"rendering", true,
						L"compatible", core::stringw(useCompatibilityRenderer).c_str(),
						L"inactiveFPSLimit", core::stringw(inactiveFPSLimit).c_str()
						);
	writer->writeLineBreak();

	writer->writeComment(L"Path to orginal games to load gamefiles from them");
	writer->writeLineBreak();
	writer->writeElement(L"path", true,
						L"TheMoonProject", core::stringw(path_TheMoonProject).c_str(),
						L"LostSouls", core::stringw(path_LostSouls).c_str()
						);
	writer->writeLineBreak();

	// Schreibe config in Aktueller Version
	writer->writeElement(L"version", true,
						L"ConfigVersion", core::stringw(CONFIG_VERSION).c_str()
						);
	writer->writeLineBreak();

	// Wurzel schließen
	writer->writeClosingTag(L"config");
}

void ClientConfig::loadXMLFile(io::IXMLReader* reader) {
	latestConfigVersion = false;

	while (reader->read()) {
		// Wir wollen nur Einträge lesen, welche vom Typ "Element" sind
		if (reader->getNodeType() != io::EXN_ELEMENT)
			continue;

		if (core::stringw(reader->getNodeName()) == L"driver") {
			if (reader->getAttributeValue(L"type"))
				param.DriverType = DriverType(core::stringw(reader->getAttributeValue(L"type")));

			if (reader->getAttributeValue(L"antialias"))
				param.AntiAlias = reader->getAttributeValueAsInt(L"antialias");

			if (reader->getAttributeValue(L"stencilbuffer"))
				param.Stencilbuffer = reader->getAttributeValueAsInt(L"stencilbuffer");

			if (reader->getAttributeValue(L"stereobuffer"))
				param.Stereobuffer = reader->getAttributeValueAsInt(L"stereobuffer");

			if (reader->getAttributeValue(L"multithreaded"))
				param.DriverMultithreaded = reader->getAttributeValueAsInt(L"multithreaded");
		}

		if (core::stringw(reader->getNodeName()) == L"screen") {
			if (reader->getAttributeValue(L"width"))
				param.WindowSize.Width = reader->getAttributeValueAsInt(L"width");

			if (reader->getAttributeValue(L"height"))
				param.WindowSize.Height = reader->getAttributeValueAsInt(L"height");

			if (reader->getAttributeValue(L"bit"))
				param.Bits = reader->getAttributeValueAsInt(L"bit");

			if (reader->getAttributeValue(L"fullscreen"))
				param.Fullscreen = reader->getAttributeValueAsInt(L"fullscreen");

			if (reader->getAttributeValue(L"vsync"))
				param.Vsync = reader->getAttributeValueAsInt(L"vsync");
		}

		if (core::stringw(reader->getNodeName()) == L"rendering") {
			if (reader->getAttributeValue(L"compatible"))
				useCompatibilityRenderer = reader->getAttributeValueAsInt(L"compatible");
			if (reader->getAttributeValue(L"inactiveFPSLimit"))
				inactiveFPSLimit = reader->getAttributeValueAsInt(L"inactiveFPSLimit");
		}

		if (core::stringw(reader->getNodeName()) == L"path") {
			if (reader->getAttributeValue(L"TheMoonProject"))
				path_TheMoonProject = reader->getAttributeValue(L"TheMoonProject");

			if (reader->getAttributeValue(L"LostSouls"))
				path_LostSouls = reader->getAttributeValue(L"LostSouls");
		}

		if (core::stringw(reader->getNodeName()) == L"version") {
			if (reader->getAttributeValue(L"ConfigVersion")) {
				if (reader->getAttributeValueAsInt(L"ConfigVersion") == CONFIG_VERSION)
					latestConfigVersion = true;
			}
		}
	}
}

const wchar_t* ClientConfig::DriverName(irr::video::E_DRIVER_TYPE driverType) {
	switch (driverType) {
		case video::EDT_NULL:
			return L"null";
		case video::EDT_OPENGL:
			return L"opengl";
		case video::EDT_DIRECT3D8:
			return L"dx8";
		case video::EDT_DIRECT3D9:
			return L"dx9";
		case video::EDT_BURNINGSVIDEO:
		default:
			return L"software";
	}
}

irr::video::E_DRIVER_TYPE ClientConfig::DriverType(const irr::core::stringw& name) {
	if (name == L"null")
		return video::EDT_NULL;
	if (name == L"opengl")
		return video::EDT_OPENGL;
	if (name == L"dx8")
		return video::EDT_DIRECT3D8;
	if (name == L"dx9")
		return video::EDT_DIRECT3D9;

	// Default
	return video::EDT_BURNINGSVIDEO;
}
