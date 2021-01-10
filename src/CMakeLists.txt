cmake_minimum_required(VERSION 3.9)
project(OptimizationTest)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(OptimizationTest Main.cpp Globals.h Functions.h Stdafx.h Models/Bin.h Models/Entrance.h Models/IIdentity.h Models/Data.h Models/Model.h Algorithms/FullEnumerationAlgorithm.h Models/IPosition.h Models/Data.cpp Models/Model.cpp Algorithms/FullEnumerationAlgorithm.cpp AppSettings.h Algorithms/AlgorithmHelpers.h Algorithms/GeneticAlgorithm.cpp Algorithms/GeneticAlgorithm.h Algorithms/AlgorithmHelpers.cpp)
#standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#other libraries
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
#Boost library
set(Boost_INCLUDE_DIR e:/MinGW/include/)
set(Boost_LIBRARY_DIR e:/MinGW/lib/)
find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(OptimizationTest ${Boost_LIBRARIES})