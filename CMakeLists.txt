# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Arkanoid VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Locate SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_ttf_INCLUDE_DIRS})

# Source files
set(SOURCES
    main.cpp
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SDL2 library
#target_link_libraries(Arkanoid ${SDL2_LIBRARIES}, ${SDL2_ttf_LIBRARIES})
target_link_libraries(${PROJECT_NAME}  PUBLIC SDL2::SDL2 SDL2_ttf::SDL2_ttf)

# Set a symbolic link so exe can find the /assets folder
set(link_src "${CMAKE_SOURCE_DIR}/assets")
if(UNIX AND NOT APPLE) 
    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets") 
else() 
    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets") 
endif() 
add_custom_command( 
    TARGET ${PROJECT_NAME} 
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${link_src} ${link_dst} 
    DEPENDS ${link_dst} 
    COMMENT "symbolic link ${link_src} => ${link_dst}" 
)

