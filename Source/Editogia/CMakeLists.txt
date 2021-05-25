ADD_LIBRARY(Editogia.Lib
        ai_city.cpp
        ai_data.cpp
        animal.cpp
        animal_data.cpp
        area.cpp
        area_data.cpp
        building.cpp
        building_data.cpp
        citizen.cpp
        city_achievement.cpp
        city.cpp
        city_type.cpp
        combat.cpp
        date.cpp
        game.cpp
        geometry.cpp
        globals.cpp
        help.cpp
        help_data.cpp
        init.cpp
        interface.cpp
        keys.cpp
        kingdom.cpp
        ManagerResources.cpp
        map.cpp
        map_data.cpp
        map_type_data.cpp
        Message.cpp
        military.cpp
        pathfind.cpp
        player_city.cpp
        race.cpp
        race_data.cpp
        Recipe.cpp
        resource.cpp
        resource_data.cpp
        rng.cpp
        trade.cpp
        world.cpp
        )
SET_TARGET_PROPERTIES(Editogia.Lib PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Editogia.Lib PRIVATE Editogia.Common)
TARGET_LINK_LIBRARIES(Editogia.Lib PRIVATE Levin::Framework)
TARGET_LINK_LIBRARIES(Editogia.Lib PRIVATE ArduinoJson)
TARGET_INCLUDE_DIRECTORIES(Editogia.Lib PUBLIC $<BUILD_INTERFACE:${EDITOGIA_ROOT_DIR}/Include/>)

ADD_EXECUTABLE(Editogia.App main.cpp)
SET_TARGET_PROPERTIES(Editogia.App PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Editogia.App PRIVATE Editogia.Lib)
TARGET_LINK_LIBRARIES(Editogia.App PRIVATE Levin::Framework)

# Move the binary after of build the target to binary root directory
# for found the resources that need the executable
SET_TARGET_PROPERTIES(Editogia.App PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EDITOGIA_BINARY_DIR})