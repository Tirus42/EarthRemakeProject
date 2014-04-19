#include "GUI/GUIElementEventHandler.h"

#include <cassert>

using namespace irr;

GUIElementEventHandler::GUIElementEventHandler() :
	callbacks() {
}

GUIElementEventHandler::~GUIElementEventHandler() {
	for (core::map<gui::IGUIElement*, CallbackMethod>::Iterator i = callbacks.getRoot(); !i.atEnd(); i++) {
		removeElement(i);
	}
}

void GUIElementEventHandler::addGUIElement(gui::IGUIElement* element, IReferenceCounted* callbackObject, bool (*callbackMethod)(const SEvent&), bool filterMouseOver) {
	CallbackMethod method;
	method.object = callbackObject;
	method.method = callbackMethod;
	method.filterMouseOver = filterMouseOver;

	// Fall vermeiden das ein Element erst ->drop() und dann wieder ->grab() aufgerufen werden muss
	// (Das selbe GUI Element darf hier nicht mehrfach eingetragen werden)
	assert(callbacks.find(element) == 0);

	element->grab();
	callbackObject->grab();

	callbacks.insert(element, method);
}

void GUIElementEventHandler::removeGUIElement(gui::IGUIElement* element) {
	core::map<gui::IGUIElement*, CallbackMethod>::Iterator i = callbacks.find(element);

	if (i.atEnd())
		return;

	removeElement(i);

	// Todo: Verbessern (Laufzeit)
	callbacks.remove(element);
}


void GUIElementEventHandler::removeElement(core::map<gui::IGUIElement*, CallbackMethod>::Iterator i) {
	gui::IGUIElement* element = i->getKey();
	element->drop();

	IReferenceCounted* object = i->getValue().object;
	object->drop();
}

bool GUIElementEventHandler::OnEvent(const SEvent& event) {
	if (event.EventType != EET_GUI_EVENT)
		return false;

	gui::IGUIElement* element = event.GUIEvent.Caller;

	core::map<gui::IGUIElement*, CallbackMethod>::Iterator i = callbacks.find(element);

	if (i.atEnd())
		return false;

	const CallbackMethod& method = i->getValue();

	if (method.filterMouseOver) {
		if (event.GUIEvent.EventType == gui::EGET_ELEMENT_HOVERED
			|| event.GUIEvent.EventType == gui::EGET_ELEMENT_LEFT
			|| event.GUIEvent.EventType == gui::EGET_ELEMENT_FOCUSED
			|| event.GUIEvent.EventType == gui::EGET_ELEMENT_FOCUS_LOST)
			return false;
	}

	bool (*callFunc)(IReferenceCounted* , const SEvent& event) = (bool (*)(IReferenceCounted*, const SEvent& event))method.method;

	return callFunc(method.object, event);
}
