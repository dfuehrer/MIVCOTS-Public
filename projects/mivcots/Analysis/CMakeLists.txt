add_library(Analysis AnalysisChild.cpp Analysis.cpp AnomalyDetection.cpp)
find_package(wxWidgets)
include(${wxWidgets_USE_FILE})
target_link_libraries(Analysis CarData)
