ADD_LIBRARY(Editogia.Common
        Backend/Curses/Curses.cpp
        View/Glyph.cpp
        View/TextColor.cpp
        color.cpp
        files.cpp
        stringfunc.cpp
        window.cpp
        )
SET_TARGET_PROPERTIES(Editogia.Common PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Editogia.Common PUBLIC $<BUILD_INTERFACE:${EDITOGIA_ROOT_DIR}/Include/>)