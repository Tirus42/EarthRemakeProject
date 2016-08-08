cmake_minimum_required (VERSION 2.6)

project (LibEarth2150)

include_directories("include")

add_library(LibEarth2150 STATIC
	src/Player/PlayerRelationTable.cpp
	src/Player/PlayerBuildingList.cpp
	src/Network/PlayerNetworkConnection.cpp
	src/Network/TestServer.cpp
	src/Network/SendBuffer.cpp
	src/Unit/MovingUnit.cpp
	src/Unit/Unit.cpp
	src/Unit/UnitChassis.cpp
	src/Unit/UnitChassisList.cpp
	src/Map/Map.cpp
	src/Map/MapBuildings.cpp
	src/Map/MapPosition.cpp
	src/Map/MapBitLayer.cpp
	src/Map/MapViewerManager.cpp
	src/Map/MapRectArea.cpp
	src/Map/MapPositionFloat.cpp
	src/Map/MapCircleArea.cpp
	src/Map/MapManipulator.cpp
	src/Map/HeightMap.cpp
	src/PathFinder/TSearcher.cpp
	src/PathFinder/NavigatorFactory.cpp
	src/PathFinder/TNavigator.cpp
	src/Building/BuildingReference.cpp
	src/Building/BuildingChassis.cpp
	src/Building/BuildingGround.cpp
	src/Building/BuildingDirection.cpp
	src/Building/GlobalBuildingList.cpp
	src/Util/UIDManager.cpp
	src/Util/MemoryHelper.cpp
	src/e2150/AStar.cpp
	src/e2150/JPSNavigator.cpp
	src/e2150/Faction.cpp
	src/e2150/Player.cpp
	src/e2150/MapUpdateCore.cpp
	src/e2150/AStarNode.cpp
	src/e2150/HumanPlayer.cpp
	src/e2150/BuildingUpdater.cpp
	src/e2150/FactionManager.cpp
	src/e2150/Entity.cpp
	src/Tests/PathFinderTester.cpp
)

if (BUILD_SERVER)
	add_executable(Earth2150_Server
		src/e2150/main.cpp
	)

	target_link_libraries(Earth2150_Server LibEarth2150 LibFF LibTF)
endif()
