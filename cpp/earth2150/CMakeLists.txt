cmake_minimum_required (VERSION 2.6)

project (LibEarth2150)

include_directories("include")

add_library(LibEarth2150 STATIC
	src/Player/PlayerRelationTable.cpp					include/Player/PlayerRelationTable.h
	src/Player/PlayerBuildingList.cpp					include/Player/PlayerBuildingList.h

	include/Network/NetworkPacket.h
	src/Network/PlayerNetworkConnection.cpp				include/Network/PlayerNetworkConnection.h
	src/Network/SendBuffer.cpp							include/Network/SendBuffer.h
	src/Network/TestServer.cpp							include/Network/TestServer.h

	src/Unit/Unit.cpp									include/Unit/Unit.h
	src/Unit/MovingUnit.cpp								include/Unit/MovingUnit.h
	src/Unit/UnitChassis.cpp							include/Unit/UnitChassis.h
	src/Unit/UnitChassisList.cpp						include/Unit/UnitChassisList.h

	include/Map/MapPositionIterator.h
	include/Map/IMapBuildingsObserver.h
	src/Map/Map.cpp										include/Map/Map.h
	src/Map/MapBuildings.cpp							include/Map/MapBuildings.h
	src/Map/MapPositionFloat.cpp						include/Map/MapPositionFloat.h
	src/Map/MapPosition.cpp								include/Map/MapPosition.h
	src/Map/MapBitLayer.cpp								include/Map/MapBitLayer.h
	src/Map/MapViewerManager.cpp						include/Map/MapViewerManager.h
	src/Map/MapRectArea.cpp								include/Map/MapRectArea.h
	src/Map/MapCircleArea.cpp							include/Map/MapCircleArea.h
	src/Map/MapManipulator.cpp							include/Map/MapManipulator.h
	src/Map/HeightMap.cpp								include/Map/HeightMap.h

	include/PathFinder/INavigator.h
	src/PathFinder/TNavigator.cpp						include/PathFinder/TNavigator.h
	src/PathFinder/TSearcher.cpp						include/PathFinder/TSearcher.h
	src/PathFinder/NavigatorFactory.cpp					include/PathFinder/NavigatorFactory.h

	include/Building/Building.h
	src/Building/BuildingReference.cpp					include/Building/BuildingReference.h
	src/Building/BuildingChassis.cpp					include/Building/BuildingChassis.h
	src/Building/BuildingGround.cpp						include/Building/BuildingGround.h
	src/Building/BuildingDirection.cpp					include/Building/BuildingDirection.h
	src/Building/GlobalBuildingList.cpp					include/Building/GlobalBuildingList.h

	include/Util/MemoryBlock.h
	include/Util/ReferenceCounter.h
	src/Util/UIDManager.cpp								include/Util/UIDManager.h
	src/Util/MemoryHelper.cpp							include/Util/MemoryHelper.h

	include/e2150/main.h
	include/e2150/GameContent.h
	include/e2150/Utils.h
	src/e2150/AStar.cpp									include/e2150/AStar.h
	src/e2150/AStarNode.cpp								include/e2150/AStarNode.h
	src/e2150/JPSNavigator.cpp							include/e2150/JPSNavigator.h
	src/e2150/MapUpdateCore.cpp							include/e2150/MapUpdateCore.h
	src/e2150/Player.cpp								include/e2150/Player.h
	src/e2150/HumanPlayer.cpp							include/e2150/HumanPlayer.h
	src/e2150/BuildingUpdater.cpp						include/e2150/BuildingUpdater.h
	src/e2150/Faction.cpp								include/e2150/Faction.h
	src/e2150/FactionManager.cpp						include/e2150/FactionManager.h
	src/e2150/Entity.cpp								include/e2150/Entity.h

	src/Tests/PathFinderTester.cpp						include/Tests/PathFinderTester.h

	include/Event/IEntityEventReceiver.h
)

if (BUILD_SERVER)
	add_executable(Earth2150_Server
		src/e2150/main.cpp
	)

	target_link_libraries(Earth2150_Server LibEarth2150 LibFF LibTF)
endif()
