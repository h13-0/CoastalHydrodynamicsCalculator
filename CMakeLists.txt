# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("CoastalHydrodynamicsCalculator")

# Check dependencies.
set(CMAKE_AUTOMOC ON) # Meta-Object Compiler
set(CMAKE_AUTOUIC ON) # User Interface Compiler
find_package(Qt6 COMPONENTS Widgets Core Gui Charts REQUIRED)

Qt6_WRAP_UI(UI_Files "${PROJECT_SOURCE_DIR}/QtUI/CalculatorWindow.ui")

# Set header paths.
include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}"
	"${Qt6Widgets_INCLUDE_DIRS}"
	"${PROJECT_SOURCE_DIR}/QtUI"
	"${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator"
	"${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator/MATLAB/WaveRefractionCalc/for_redistribution_files_only"
	"C:/Program Files/Polyspace/R2019b/extern/include"
)

# Add source files.
aux_source_directory("${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator" SOURCE_FILES)
aux_source_directory("${PROJECT_SOURCE_DIR}/QtUI" SOURCE_FILES)
set(
	SOURCE_FILES
	"${SOURCE_FILES}"
	"${PROJECT_SOURCE_DIR}/QtUI/CalculatorWindow.ui"
	"${PROJECT_SOURCE_DIR}/main.cpp"
	"${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator.rc"
)

# Compile executable file.
link_directories(
	"${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator/MATLAB/"
	"${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator/MATLAB/WaveRefractionCalc/for_redistribution_files_only"
)
add_executable(CoastalHydrodynamicsCalculator WIN32 ${SOURCE_FILES})

target_link_libraries(
	CoastalHydrodynamicsCalculator
	Qt6::Widgets
	Qt6::Core
	Qt6::Gui
	Qt6::Charts
	WaveRefractionCalc
	mclmcrrt
	libmx
	libmat
	mclmcr
)
