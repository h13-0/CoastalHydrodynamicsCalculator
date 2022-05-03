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
	${CMAKE_CURRENT_BINARY_DIR}
	${Qt6Widgets_INCLUDE_DIRS}
	"${PROJECT_SOURCE_DIR}/QtUI"
	
	"${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator"
)

# Add source files.
aux_source_directory("${PROJECT_SOURCE_DIR}/CoastalHydrodynamicsCalculator" SOURCE_FILES)
aux_source_directory("${PROJECT_SOURCE_DIR}/QtUI" SOURCE_FILES)
set(
	SOURCE_FILES
	${SOURCE_FILES}
	"${PROJECT_SOURCE_DIR}/QtUI/CalculatorWindow.ui"
	"${PROJECT_SOURCE_DIR}/main.cpp"
)


# Compile executable file.
add_executable(CoastalHydrodynamicsCalculator ${SOURCE_FILES})
target_link_libraries(CoastalHydrodynamicsCalculator Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Charts)
