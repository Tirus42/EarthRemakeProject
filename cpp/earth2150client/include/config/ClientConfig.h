#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <irrlicht.h>

/**
* Speichert die Grafikkonfiguration.
* Bietet Methoden um diese in eine XML Datei zu speichern und zu laden.
*/
class ClientConfig {
	private:
		irr::SIrrlichtCreationParameters param;

		/// Gibt den Namen für die Config für den Treibertyp zurück.
		static const wchar_t* DriverName(irr::video::E_DRIVER_TYPE driverType);

		/// Gibt den Enum-Wert für den Namen des Treibertyps zurück.
		static irr::video::E_DRIVER_TYPE DriverType(const irr::core::stringw& name);

	public:
		ClientConfig();
		~ClientConfig();

		/// Setzt die Standardwerte für Auflösung, Ausgabegerät ect.
		void setDefaultValues();

		void loadXMLFile(irr::io::IXMLReader* reader);
		void saveXMLFile(irr::io::IXMLWriter* writer) const;

		/// Gibt Zugriff auf die Parameter für das IrrlichtDevice.
		irr::SIrrlichtCreationParameters& Parameter() {return param;};
		const irr::SIrrlichtCreationParameters& Parameter() const {return param;};

};


#endif // CONFIG_H_INCLUDED
