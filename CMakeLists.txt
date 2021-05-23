CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
PROJECT(Editogia)
ADD_EXECUTABLE(Editogia.Cussed
        cuss.cpp
        cuss_editor.cpp
        window.cpp
        glyph.cpp
        color.cpp
        stringfunc.cpp
        files.cpp
        )
ADD_EXECUTABLE(Editogia.Cusstest
        cuss.cpp
        cusstest.cpp
        window.cpp
        glyph.cpp
        color.cpp
        stringfunc.cpp
        files.cpp
        )
ADD_EXECUTABLE(Editogia.App
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
        color.cpp
        combat.cpp
        cuss.cpp
        date.cpp
        files.cpp
        game.cpp
        geometry.cpp
        globals.cpp
        glyph.cpp
        help.cpp
        help_data.cpp
        init.cpp
        interface.cpp
        keys.cpp
        kingdom.cpp
        main.cpp
        map.cpp
        map_data.cpp
        map_type_data.cpp
        military.cpp
        pathfind.cpp
        player_city.cpp
        race.cpp
        race_data.cpp
        resource.cpp
        resource_data.cpp
        rng.cpp
        stringfunc.cpp
        trade.cpp
        window.cpp
        world.cpp
        )

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/notes DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cuss DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/save DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

FIND_PACKAGE(Curses)

IF (CURSES_FOUND)
    TARGET_INCLUDE_DIRECTORIES(Editogia.App PRIVATE ${CURSES_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(Editogia.App PRIVATE ${CURSES_LIBRARIES})
ELSE()
    MESSAGE(FATAL_ERROR "Not support to NCurses")
ENDIF()