cmake_minimum_required(VERSION 3.16)

project(LongQt VERSION 0.4)

# Find includes in corresponding build directories
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#For linux and mac only
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
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
set(license_file "LICENSE")

set(CMAKE_AUTOUIC_SEARCH_PATHS ${includes})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
	get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(qt5_bin_location ${_qt5_qmake_location} DIRECTORY)
	if(EXISTS "${qt5_bin_location}/windeployqt.exe")
		add_executable(Qt5::windeployqt IMPORTED)
		set_target_properties(Qt5::windeployqt PROPERTIES
			IMPORTED_LOCATION "${qt5_bin_location}/windeployqt.exe")
	endif()
	message("Qt5::windeployqt added")
elseif(APPLE AND TARGET Qt5::qmake AND NOT TARGET Qt5::macdeployqt)
	get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(qt5_bin_location ${_qt5_qmake_location} DIRECTORY)
	if(EXISTS "${qt5_bin_location}/macdeployqt.exe")
		add_executable(Qt5::macdeployqt IMPORTED)
		set_target_properties(Qt5::macdeployqt PROPERTIES
			IMPORTED_LOCATION "${qt5_bin_location}/macdeployqt.exe")
	endif()
	message("Qt5::macdeployqt added")

endif()

## Build LongQt
add_executable(LongQt WIN32 MACOSX_BUNDLE
	"src/LongQt.cpp" 
	${sources}
	"LongQt.qrc"
	${icon_file}
)

target_include_directories(LongQt PUBLIC
    "$<BUILD_INTERFACE:${includes}>")

target_link_libraries(LongQt PUBLIC 
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Concurrent
	LongQt-model
)

## Build LongQtGrapher
add_executable(LongQtGrapher WIN32 MACOSX_BUNDLE
	"src/LongQtGrapher.cpp" 
	${sources}
	"LongQtGrapher.qrc"
	${icon_file}
)

target_include_directories(LongQtGrapher PUBLIC
    "$<BUILD_INTERFACE:${includes}>")

target_link_libraries(LongQtGrapher PUBLIC 
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Concurrent
	LongQt-model
)

add_executable(LQGridEditor WIN32 MACOSX_BUNDLE
	"src/LQGridEditorMain.cpp" 
	${sources}
	"LongQt.qrc"
	${icon_file}
)

target_include_directories(LQGridEditor PUBLIC
    "$<BUILD_INTERFACE:${includes}>")

target_link_libraries(LQGridEditor PUBLIC 
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Concurrent
	LongQt-model
)


## Use qt deploy tools to copy binaries
#		COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/libdeploy"
if(TARGET Qt5::windeployqt)
	add_custom_command(TARGET LongQt
		POST_BUILD
		COMMAND Qt5::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/libdeploy" "$<TARGET_FILE_DIR:LongQt>/$<TARGET_FILE_NAME:LongQt>"
	)
	add_custom_command(TARGET LongQtGrapher
		POST_BUILD
		COMMAND Qt5::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/libdeploy" "$<TARGET_FILE_DIR:LongQtGrapher>/$<TARGET_FILE_NAME:LongQtGrapher>"
	)
	add_custom_command(TARGET LQGridEditor
		POST_BUILD
		COMMAND Qt5::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/libdeploy" "$<TARGET_FILE_DIR:LQGridEditor>/$<TARGET_FILE_NAME:LQGridEditor>"
	)
elseif(TARGET Qt5::macdeployqt)
	add_custom_command(TARGET LongQt
		POST_BUILD
		COMMAND Qt5::macdeployqt "$<TARGET_FILE_DIR:LongQt>/$<TARGET_FILE_NAME:LongQt>"
	)
	add_custom_command(TARGET LongQtGrapher
		POST_BUILD
		COMMAND Qt5::macdeployqt "$<TARGET_FILE_DIR:LongQtGrapher>/$<TARGET_FILE_NAME:LongQtGrapher>"
	)
	add_custom_command(TARGET LQGridEditor
		POST_BUILD
		COMMAND Qt5::macdeployqt "$<TARGET_FILE_DIR:LQGridEditor>/$<TARGET_FILE_NAME:LQGridEditor>"
	)

endif()

## Install executables
install(TARGETS LongQt LongQtGrapher LQGridEditor
	EXPORT LongQt
	BUNDLE DESTINATION ./bundle
)
#	FRAMEWORK DESTINATION "Frameworks"

install(FILES ${license_file}
	TYPE INFO)

## Install dependancies for windows and OSX 
## for windows more dependances will need to
## be copied by Qt's windeploy tool
## but on OSX this should be everything that is
## needed
if(WIN32)
	install(
		DIRECTORY
		"${CMAKE_CURRENT_BINARY_DIR}/libdeploy/"
		TYPE BIN)
	#	install(FILES
	#		$<TARGET_FILE:Qt5::Core>
	#		$<TARGET_FILE:Qt5::Gui>
	#		$<TARGET_FILE:Qt5::Widgets>
	#		$<TARGET_FILE:Qt5::PrintSupport>
	#		$<TARGET_FILE:Qt5::Concurrent>
	#		TYPE BIN
	#	)
	#elseif(APPLE)
	#	install(CODE
	#		COMMAND Qt5::macdeployqt "${CMAKE_INSTALL_PREFIX}/bundle/LongQt.app"
	#		COMMAND Qt5::macdeployqt "${CMAKE_INSTALL_PREFIX}/bundle/LongQtGrapher.app"
	#		COMMAND Qt5::macdeployqt "${CMAKE_INSTALL_PREFIX}/bundle/LQGridEditor.app"
	#	)

	#	install(CODE
	#		"include(BundleUtilites)"
	#		COMMAND "fixup_bundle(LongQt)"
	#		COMMAND "fixup_bundle(LongQtGrapher)"
	#		COMMAND "fixup_bundle(LQGridEditor)"
	#	)
	#	install(
	#		DIRECTORY
	#		"${CMAKE_CURRENT_BINARY_DIR}/libdeploy/"
	#		TYPE LIB)
	#
	#	install(FILES
	#		$<TARGET_FILE:Qt5::Core>
	#		$<TARGET_FILE:Qt5::Gui>
	#		$<TARGET_FILE:Qt5::Widgets>
	#		$<TARGET_FILE:Qt5::PrintSupport>
	#		$<TARGET_FILE:Qt5::Concurrent>
	#		TYPE LIB
	#	)
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
