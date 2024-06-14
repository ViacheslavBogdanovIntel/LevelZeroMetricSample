cmake_minimum_required(VERSION 3.16)

project(LevelZeroMetricSample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LevelZeroMetricSample
    src/main.cpp
    src/NPUCollector.cpp
    src/NPUCollector.h
    src/NPUDevice.cpp
    src/NPUDevice.h
    src/NPUEngine.cpp
    src/NPUEngine.h
    src/NPUMetric.cpp
    src/NPUMetric.h
    src/NPUMetricGroup.cpp
    src/NPUMetricGroup.h
    src/NPUMetricStreamer.cpp
    src/NPUMetricStreamer.h
    src/common.h
    src/argparser.h
)

target_include_directories(LevelZeroMetricSample
PRIVATE
	3rdparty/level_zero/include
)

target_link_directories(LevelZeroMetricSample
PRIVATE
	3rdparty/level_zero/lib/${CMAKE_BUILD_TYPE}
)

target_link_libraries(LevelZeroMetricSample 
PRIVATE 
	ze_loader.lib
)
