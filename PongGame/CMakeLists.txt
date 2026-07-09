cmake_minimum_required(VERSION 4.2)
project(PongGame)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

if(APPLE)
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif()

add_executable(PongGame
        audio_player.cpp
        audio_player.h
        ball.cpp
        ball.h
        player.cpp
        player.h
        PongGame.cpp)
        
find_package(raylib REQUIRED)

target_link_libraries(PongGame PRIVATE raylib)

add_custom_command(TARGET PongGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:PongGame>/shaders"
        COMMENT "Copying shaders to build directory..."
)

add_custom_command(TARGET PongGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/sounds"
        "$<TARGET_FILE_DIR:PongGame>/sounds"
        COMMENT "Copying sounds to build directory..."
)
