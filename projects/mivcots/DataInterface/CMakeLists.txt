add_library(DataInterface DataInterface.cpp)
find_package(wxWidgets)
include(${wxWidgets_USE_FILE})
target_link_libraries(DataInterface CarData)
