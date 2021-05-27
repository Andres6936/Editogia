ADD_LIBRARY(Editogia.Cuss
        Elements/Alignment.cpp
        Elements/Drawing.cpp
        Elements/Element.cpp
        Elements/List.cpp
        Elements/Menu.cpp
        Elements/Number.cpp
        Elements/TextBox.cpp
        Elements/TextEntry.cpp
        Elements/Type.cpp
        ActionID.cpp
        Binding.cpp
        Interface.cpp
        )
SET_TARGET_PROPERTIES(Editogia.Lib PROPERTIES CXX_STANDARD 17)
TARGET_INCLUDE_DIRECTORIES(Editogia.Lib PUBLIC $<BUILD_INTERFACE:${EDITOGIA_ROOT_DIR}/Include/>)