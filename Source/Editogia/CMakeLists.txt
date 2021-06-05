ADD_LIBRARY(Editogia.Lib
        Entity/Org/ai_city.cpp
        Entity/Org/area.cpp
        Entity/Org/city.cpp
        Entity/Org/city_achievement.cpp
        Entity/Org/city_type.cpp
        Entity/Org/combat.cpp
        Entity/Org/kingdom.cpp
        Entity/Org/military.cpp
        Entity/Org/player_city.cpp
        Entity/Org/trade.cpp
        Entity/animal.cpp
        Entity/citizen.cpp
        Entity/race.cpp
        Gea/map.cpp
        Gea/resource.cpp
        Gea/world.cpp
        Math/geometry.cpp
        Math/pathfind.cpp
        Math/rng.cpp
        Mechanism/building.cpp
        Mechanism/ManagerResources.cpp
        Mechanism/Recipe.cpp
        Render/View/help.cpp
        Render/View/Message.cpp
        Scene/Menu.cpp
        Scene/SceneManager.cpp
        Util/Time/Date.cpp
        ai_data.cpp
        animal_data.cpp
        area_data.cpp
        building_data.cpp
        game.cpp
        globals.cpp
        help_data.cpp
        init.cpp
        map_data.cpp
        map_type_data.cpp
        race_data.cpp
        resource_data.cpp
        )
SET_TARGET_PROPERTIES(Editogia.Lib PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Editogia.Lib PRIVATE Editogia.Common)
TARGET_LINK_LIBRARIES(Editogia.Lib PRIVATE Editogia.Cuss)
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