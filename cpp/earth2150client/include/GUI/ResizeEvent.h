#ifndef RESIZEEVENT_H_INCLUDED
#define RESIZEEVENT_H_INCLUDED

#include <irrlicht.h>

/**
* Hilfsklasse um Events in Irrlicht zu erstellen, wenn das Anwendungsfenster
* seine Auflösung ändert.
* Eigendlich kann das Irrlicht auch selbst, scheint aber noch verbuggt zu sein.
* (Jedenfalls unter Windows)
* Todo: Mit irrlicht 1.9 prüfen ob dies noch nötig ist und ggf. entfernen.
*/
class ResizeEvent {
	private:
		static const irr::u32 EVENT_TYPE = 0xFFFF;

	public:
		static void createResizeEvent(irr::SEvent& eventObject, const irr::core::dimension2du& newSize) {

			eventObject.EventType = (irr::EEVENT_TYPE)EVENT_TYPE;

			eventObject.UserEvent.UserData1 = newSize.Width;
			eventObject.UserEvent.UserData2 = newSize.Height;
		}

		static bool isResizeEvent(const irr::SEvent& eventObject) {
			return eventObject.EventType == (irr::EEVENT_TYPE)EVENT_TYPE;
		}

		static irr::core::dimension2du getResizeEventData(const irr::SEvent& eventObject) {
			return irr::core::dimension2du(
											eventObject.UserEvent.UserData1,
											eventObject.UserEvent.UserData2);
		}

};

#endif // RESIZEEVENT_H_INCLUDED
