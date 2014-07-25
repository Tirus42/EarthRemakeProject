#ifndef BUILDINGUPDATER_H_INCLUDED
#define BUILDINGUPDATER_H_INCLUDED

class Map;
class MapUpdateCore;

class BuildingUpdater {
	private:
		BuildingUpdater(const BuildingUpdater&);
		BuildingUpdater operator=(const BuildingUpdater&);

	public:
		BuildingUpdater(const Map& map);
		~BuildingUpdater();


		void doTick(Map& map, MapUpdateCore& updateCore);

};


#endif // BUILDINGUPDATER_H_INCLUDED
