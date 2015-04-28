#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <irrlicht.h>

/**
* Speichert die Grafikkonfiguration.
* Bietet Methoden um diese in eine XML Datei zu speichern und zu laden.
*/
class ClientConfig {
	private:
		/// Versionsnr der Config Datei, diese Nr sollte bei jedem Ändern dieser Klasse erhöht werden.
		/// Immer wenn diese Nr != der gespeicherten ist, sollte die Config neu geschrieben werden.
		static const irr::u16 CONFIG_VERSION = 3;

		/// Irrlicht Parameter für Grafikeinstellungen
		irr::SIrrlichtCreationParameters param;

		/// Wenn gesetzt werden keine modernen GPU features vorrausgesetzt
		bool useCompatibilityRenderer;

		/// Pfard Angabe zu den Spieldaten von The Moon Project
		irr::io::path path_TheMoonProject;

		/// Pfard Angabe zu den Spieldaten von Lost Souls
		irr::io::path path_LostSouls;

		irr::u32 inactiveFPSLimit;

		/// Angabe ob die geladene Config in der aktuellen Version ist (siehe CONFIG_VERSION)
		bool latestConfigVersion;

		/// Gibt den Namen für die Config für den Treibertyp zurück.
		static const wchar_t* DriverName(irr::video::E_DRIVER_TYPE driverType);

		/// Gibt den Enum-Wert für den Namen des Treibertyps zurück.
		static irr::video::E_DRIVER_TYPE DriverType(const irr::core::stringw& name);

	public:
		ClientConfig();

		/// Setzt die Standardwerte für Auflösung, Ausgabegerät ect.
		void setDefaultValues();

		void loadXMLFile(irr::io::IXMLReader* reader);
		void saveXMLFile(irr::io::IXMLWriter* writer) const;

		/// Gibt Zugriff auf die Parameter für das IrrlichtDevice.
		irr::SIrrlichtCreationParameters& Parameter() {return param;};
		const irr::SIrrlichtCreationParameters& Parameter() const {return param;};

		/// Gibt an ob der Kompatibilitätsrenderer verwendet werden soll (keine modernen GPU features)
		bool getUseCompatibilityRenderer() const {return useCompatibilityRenderer;}

		/// Gibt den Pfard zu dem Spielordner von The Moon Project zurück (kann "" sein)
		const irr::io::path& getTheMoonProjectPath() const {return path_TheMoonProject;}

		/// Gibt den Pfard zu dem Spielordner von Lost Souls zurück (kann "" sein)
		const irr::io::path& getLostSoulsPath() const {return path_LostSouls;}

		irr::u32 getInactiveFPSLimit() const {return inactiveFPSLimit;}

		/// Gibt zurück ob die geladene Config in der aktuellen Version ist (siehe CONFIG_VERSION)
		bool isLatestConfigVersion() const {return latestConfigVersion;}
};


#endif // CONFIG_H_INCLUDED
