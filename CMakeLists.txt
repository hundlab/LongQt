cmake_minimum_required(VERSION 3.16)

project(LongQt VERSION 0.4)

# Find includes in corresponding build directories
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#For linux and unix only
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#set(CMAKE_BUILD_TYPE Release)

find_package(Qt5 COMPONENTS Core Gui Widgets Concurrent PrintSupport REQUIRED)
find_package(LongQt-model REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/src/LongQtMain/LongQtMain.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/src/Grapher/Grapher.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/src/GridEditor/GridEditor.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/src/Utils.cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(WIN32 AND NOT CYGWIN)
	set(icon_file "LongQt.rc")
elseif(APPLE)
	set(icon_file "LongQtIcon.icns")
	set_source_files_properties(LongQtIcon.icns PROPERTIES
		MACOSX_PACKAGE_LOCATION "Resources") 
else()
	set(icon_file "")
endif()

set(CMAKE_AUTOUIC_SEARCH_PATHS ${includes})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

## Build LongQt
add_executable(LongQt
	"src/LongQt.cpp" 
	${sources}
	"LongQt.qrc"
	${icon_file}
)

target_include_directories(LongQt PUBLIC
    "$<BUILD_INTERFACE:${includes}>")

set_target_properties(LongQt PROPERTIES
    VERSION ${PROJECT_VERSION_MAJOR}
    SOVERSION ${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE)

target_link_libraries(LongQt PUBLIC 
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Concurrent
	LongQt-model
)

## Build LongQtGrapher
add_executable(LongQtGrapher
	"src/LongQtGrapher.cpp" 
	${sources}
	"LongQtGrapher.qrc"
	${icon_file}
)

target_include_directories(LongQtGrapher PUBLIC
    "$<BUILD_INTERFACE:${includes}>")

set_target_properties(LongQtGrapher PROPERTIES
    VERSION ${PROJECT_VERSION_MAJOR}
    SOVERSION ${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE)

target_link_libraries(LongQtGrapher PUBLIC 
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Concurrent
	LongQt-model
)

add_executable(LQGridEditor
	"src/LQGridEditorMain.cpp" 
	${sources}
	"LongQt.qrc"
	${icon_file}
)

target_include_directories(LQGridEditor PUBLIC
    "$<BUILD_INTERFACE:${includes}>")

set_target_properties(LQGridEditor PROPERTIES
    VERSION ${PROJECT_VERSION_MAJOR}
    SOVERSION ${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE)

target_link_libraries(LQGridEditor PUBLIC 
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Concurrent
	LongQt-model
)
## Install executables
install(TARGETS LongQt LongQtGrapher LQGridEditor
	EXPORT LongQt
)

## Install dependancies for windows and OSX 
## for windows more dependances will need to
## be copied by Qt's windeploy tool
## but on OSX this should be everything that is
## needed
if(WIN32)
	install(FILES
		$<TARGET_FILE:Qt5::Core>
		$<TARGET_FILE:Qt5::Gui>
		$<TARGET_FILE:Qt5::Widgets>
		$<TARGET_FILE:Qt5::PrintSupport>
		$<TARGET_FILE:Qt5::Concurrent>
		TYPE RUNTIME
	)
elseif(APPLE)
	install(FILES
		$<TARGET_FILE:Qt5::Core>
		$<TARGET_FILE:Qt5::Gui>
		$<TARGET_FILE:Qt5::Widgets>
		$<TARGET_FILE:Qt5::PrintSupport>
		$<TARGET_FILE:Qt5::Concurrent>
		TYPE FRAMEWORK
	)
endif()

## cmake has a builtin way to install dependancy
## runtime libs, however at them moment this is
## too agressive and installs everything under the
## sun. Also it errors.
## Note: there is a line you need to add to install
## TARGETS directive for the below code to work
#if(WIN32 OR APPLE)
#	install(RUNTIME_DEPENDENCY_SET LongQt
#		OPTIONAL
#	)
#endif()
