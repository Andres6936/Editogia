ADD_EXECUTABLE(Editogia.Cussed cuss_editor.cpp)
SET_TARGET_PROPERTIES(Editogia.Cussed PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Editogia.Cussed PRIVATE Editogia.Common)

ADD_EXECUTABLE(Editogia.Cusstest cusstest.cpp)
SET_TARGET_PROPERTIES(Editogia.Cusstest PROPERTIES CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(Editogia.Cusstest PRIVATE Editogia.Common)