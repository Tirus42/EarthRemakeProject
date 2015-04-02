#include "Util/UIDManager.h"

#include <cassert>

UIDManager::UIDRange::UIDRange(uint32_t element) :
	first(element),
	last(element) {}

UIDManager::UIDRange::UIDRange(uint32_t first, uint32_t last) :
	first(first),
	last(last) {

	assert(first <= last);
}

uint32_t UIDManager::UIDRange::getCount() const {
	assert(first <= last);

	return 1 + (last - first);
}

uint32_t UIDManager::UIDRange::appendOne() {
	assert(first <= last);

	last++;

	return last;
}

bool UIDManager::UIDRange::isInRange(uint32_t value) const {
	return (value >= first) && (value <= last);
}

UIDManager::UIDManager() :
	givenUIDs() {

	givenUIDs.reserve(16);
}


uint32_t UIDManager::getNewUID() {
	// Haben wir lücken in der Range?
	if (givenUIDs.size() <= 1) {
		// Überhaupt schon eine UID vergeben?
		if (givenUIDs.size() == 0) {
			givenUIDs.push_back(UIDRange(0));
			return 0;
		}

		// Gebe nächste UID, wenn keine vornedran frei
		if (givenUIDs[0].first == 0) {
			return givenUIDs[0].appendOne();
		}
	}

	// Beginnt die erste Range bei 0?
	if (givenUIDs[0].first > 0) {
		// Gebe vorherige UID
		givenUIDs[0].first--;

		return givenUIDs[0].first;
	}

	// Fülle Lücke
	uint32_t gapSize = givenUIDs[1].first - (givenUIDs[0].last + 1);

	assert(gapSize > 0);

	if (gapSize > 1)
		return givenUIDs[0].appendOne();

	// Lücke schließen
	uint32_t retUID = givenUIDs[0].last + 1;

	// Mache ersten Range komplett
	givenUIDs[0].last = givenUIDs[1].last;

	// Entferne zweiten Range Eintrag, da nun überflüssig
	givenUIDs.erase(givenUIDs.begin() + 1);

	return retUID;
}

void UIDManager::releaseUID(uint32_t uid) {
	assert(isUIDGiven(uid));

	// Finde Position der uid
	for (uint32_t i = 0; i < givenUIDs.size(); ++i) {
		if (givenUIDs[i].last < uid)
			continue;

		removeUIDFrom(i, uid);
		return;
	}

	assert(false);
}

void UIDManager::removeUIDFrom(uint32_t index, uint32_t uid) {
	assert(index < givenUIDs.size());
	assert(givenUIDs[index].isInRange(uid));

    UIDRange& range = givenUIDs[index];

    bool removeRange = false;

    if (range.first == uid) {
		if (range.getCount() > 1) {
			range.first++;
			return;
		}
		else {
			removeRange = true;
		}
    } else if (range.last == uid) {
		if (range.getCount() > 1) {
			range.last--;
			return;
		}
		else {
			removeRange = true;
		}
    }

    if (removeRange) {
		givenUIDs.erase(givenUIDs.begin() + index);
		return;
    }

	// Range ist zu teilen
	UIDRange newRange(uid + 1, range.last);

	range.last = uid - 1;

	givenUIDs.insert(givenUIDs.begin() + index + 1, newRange);
}

void UIDManager::releaseAll() {
	givenUIDs.clear();
}

bool UIDManager::isUIDGiven(uint32_t uid) const {
	for (uint32_t i = 0; i < givenUIDs.size(); ++i) {
		if (givenUIDs[i].last < uid)
			continue;

		return givenUIDs[i].isInRange(uid);
	}

	return false;
}

uint32_t UIDManager::getGivenUIDCount() const {
	uint32_t sum = 0;

	for (uint32_t i = 0; i < givenUIDs.size(); ++i) {
		sum += givenUIDs[i].getCount();
	}

	return sum;
}

uint32_t UIDManager::getUIDGapCount() const {
	if (givenUIDs.size() == 0)
		return 0;

	uint32_t count = 0;

	if (givenUIDs[0].first > 0)
		count++;

	return count + givenUIDs.size() - 1;
}
