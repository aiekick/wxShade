cmake_minimum_required(VERSION 3.10)

set(PROJECT wxShade)

enable_language(C CXX)

project(${PROJECT} CXX)

if ("${CMAKE_EXE_LINKER_FLAGS}" STREQUAL "/machine:x64")
	add_definitions(-DVERSION_X64)
endif()

if(UNIX)
    if(APPLE)
        add_definitions(-DAPPLE)
    else()
        add_definitions(-DLINUX)
    endif()
    add_definitions(-DUNIX)
	add_definitions(-DMINGW32)	
elseif(WIN32)
		add_definitions(-DWIN32)
	if(MINGW)
		add_definitions(-DMINGW32)
	endif()
endif()
	
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++14")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DMSVC)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++14")
endif ()

set(CMAKE_CXX_STANDARD            11)
set(CMAKE_CXX_STANDARD_REQUIRED   YES)

set(WX_LIBRARIES)

include(cmake/3rdparty.cmake)

file(GLOB_RECURSE PROJECT_SRC_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX src FILES ${PROJECT_SRC_RECURSE})

file(GLOB_RECURSE PROJECT_FRAMEWORK_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/framework/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/framework/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/framework/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/framework/*.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/framework PREFIX framework FILES ${PROJECT_FRAMEWORK_RECURSE})

######################################################################################################
## XPM generation from png's
######################################################################################################

find_package(ImageMagick COMPONENTS convert)

if (${ImageMagick_convert_FOUND})
	file(GLOB_RECURSE PROJECT_ICONS_PNG_RECURSE 
		${CMAKE_CURRENT_SOURCE_DIR}/icons/*.png)
		
	foreach( PNG_FILE ${PROJECT_ICONS_PNG_RECURSE})
		string(REGEX REPLACE "[.]png$" ".xpm" XPM_FILE ${PNG_FILE})
		execute_process(COMMAND ${ImageMagick_convert_EXECUTABLE} ${PNG_FILE} ${XPM_FILE})
		message("PNG Icon => XPM : ${XPM_FILE}")
	endforeach()
	
	file(GLOB_RECURSE PROJECT_ICONS_XPM_RECURSE 
		${CMAKE_CURRENT_SOURCE_DIR}/icons/*.xpm)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/icons PREFIX icons FILES ${PROJECT_ICONS_XPM_RECURSE})
	message("PROJECT_ICONS_XPM_RECURSE : ${PROJECT_ICONS_XPM_RECURSE}")
endif()

######################################################################################################
##
######################################################################################################

if (MSVC)
	set(ICON res/MSVCInfo.rc)
elseif (APPLE)
	set(ICON MACOSX_BUNDLE res/icon.icns)
	set_source_files_properties(res/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

add_executable(
	${PROJECT} 
	WIN32 ## va mettre Windows (/SUBSYSTEM:WINDOWS) 
	${ICON}
	${PROJECT_SRC_RECURSE}
	${PROJECT_FRAMEWORK_RECURSE}
	${PROJECT_ICONS_XPM_RECURSE}
)

if ("${CMAKE_EXE_LINKER_FLAGS}" STREQUAL "/machine:x64")
	set(ARCH x64)
else()
	set(ARCH x32)
endif()

if (APPLE)
	set_target_properties(${PROJECT} PROPERTIES
		OUTPUT_NAME "${PROJECT}_${ARCH}"
		MACOSX_BUNDLE_EXECUTABLE_NAME "${PROJECT}_${ARCH}"
		MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT}_${ARCH}"
		FOLDER "${PROJECT}_${ARCH}"
		##RESOURCE res/icon.icns
		##MACOSX_BUNDLE_SHORT_VERSION_STRING "0.5"
		##MACOSX_BUNDLE_LONG_VERSION_STRING "Beta 0.5"
		##MACOSX_BUNDLE_ICON_FILE res/icon.icns
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/res/MacOSXBundleInfo.plist.in")
elseif(WIN32)
	if (MINGW)
		set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${PROJECT}_Mingw32_${ARCH}")
	elseif (CYGWIN)
		set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${PROJECT}_Cygwin_${ARCH}")
	elseif(MSVC)
		set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${PROJECT}_Msvc_${ARCH}")
	endif()
elseif(UNIX)
	set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${PROJECT}_Linux_${ARCH}")
endif()

set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/bin")
install(TARGETS ${PROJECT} DESTINATION ${CMAKE_INSTALL_PREFIX})

include_directories(
	include
	src
	framework
	3rdparty
)

target_link_libraries(${PROJECT} PRIVATE 
	${WX_LIBRARIES}
	${GLAD_LIBRARIES}
	${TINYXML2_LIBRARIES})
