#ifndef GUIELEMENTEVENTHANDLER_H_INCLUDED
#define GUIELEMENTEVENTHANDLER_H_INCLUDED

#include <irrlicht.h>

/**
* Klasse welche eine zuordnung von GUI Elementen zu einer
* aufzurufenden Methode eines Objekts speichert.
* Sobald ein Event für dieses GUI Element eintritt,
* wird das Objekt mit der angegebenen Methode aufgerufen (Callback).
*/
class GUIElementEventHandler : public irr::IEventReceiver {
	private:
		struct CallbackMethod {
			irr::IReferenceCounted* object;
			bool (*method)(const irr::SEvent&);
			bool filterMouseOver;
		};


		/// Zuordnungliste von GUI Element zu der Aufzurufenden Methode
		irr::core::map<irr::gui::IGUIElement*, CallbackMethod> callbacks;

		void removeElement(irr::core::map<irr::gui::IGUIElement*, CallbackMethod>::Iterator i);
	public:
		GUIElementEventHandler();
		~GUIElementEventHandler();

		/** Fügt ein GUI Element hinzu worauf bei Events reagiert werden soll
		* \param element Das GUI Element (darf nur einmal eingetragen werden)
		* \param callbackObject Das Object welches als "this" aufgerufen wird
		* \param callbackMethod Die Methode die aufgrund des Events aufgerufen wird
		* \param filterMouseOver Gibt an ob MouseOver sowie Fokus Events ignoriert werden sollen
		*/
		void addGUIElement(irr::gui::IGUIElement* element, irr::IReferenceCounted* callbackObject, bool (*callbackMethod)(const irr::SEvent&), bool filterMouseOver = true);

		/// Entfernt das Event Handling für das GUI Element
		void removeGUIElement(irr::gui::IGUIElement* element);

		/// Event Handler
		bool OnEvent(const irr::SEvent& event);
};

#endif // GUIELEMENTEVENTHANDLER_H_INCLUDED
