# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Arkanoid VERSION 2.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Locate SDL2
if (WIN32)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2-2.30.9/cmake")
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2_ttf-2.22.0/cmake")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Source files
FILE(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
    main.cpp
)

# Add the executable
if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2::SDL2main SDL2_ttf::SDL2_ttf)
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2_ttf::SDL2_ttf)
endif()

# Link SDL2 library

# Include SDL2 headers
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_ttf_INCLUDE_DIRS})

# Include my headers
target_include_directories(${PROJECT_NAME} PUBLIC headers)

# Set a symbolic link so exe can find the /assets folder
set(link_src "${CMAKE_SOURCE_DIR}/assets")
if(UNIX AND NOT APPLE) 
    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets") 
    add_custom_command( 
        TARGET ${PROJECT_NAME} 
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${link_src} ${link_dst} 
        DEPENDS ${link_dst} 
        COMMENT "symbolic link ${link_src} => ${link_dst}" 
    )
else() 
    # Copy the assets folder to the build directory
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/assets"
        COMMENT "Copying assets folder to build directory"
    )

    # Copy SDL2.dll and SDL2_ttf.dll to the build directory
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM
    )
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM
    )
endif()

