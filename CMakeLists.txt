cmake_minimum_required(VERSION 3.0.0)
project(tetris VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


include(CTest)
enable_testing()

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)
find_package(ZLIB REQUIRED)

add_definitions(-w)

add_executable(
    tetris 
    src/game/main.cpp
    
    src/engine/error.cpp 
    src/engine/renderer.cpp
    src/engine/shader.cpp 
    src/engine/stb.cpp

    src/game/application.cpp 
    src/game/game.cpp 
    src/game/gameRender.cpp 
    src/game/memoryReplay.cpp 
    src/game/processInput.cpp 
    src/game/settings.cpp
    
    dependencies/glad/src/glad.c
   ${ZLIB_TEST_SOURCES}
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()
include_directories( 
    ${OPENGL_INCLUDE_DIRS},
    ${FREETYPE_INCLUDE_DIR_ft2build}
    dependencies/
    dependencies/glad/include/
    ${CMAKE_SOURCE_DIR}/src/
) 

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

target_link_libraries(tetris 
    glfw
    OpenGL::GL
    ${FREETYPE_LIBRARY} 
    ${ZLIB_LIBRARIES}
)

add_custom_command(TARGET tetris POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:tetris>/res/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
