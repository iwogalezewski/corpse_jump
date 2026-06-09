cmake_minimum_required(VERSION 3.16)

project(corpse_jump LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


#set(SFML_DIR "C:/Users/iwoga/Documents/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system audio)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME}
    main.cpp
    Game.cpp
    Game.h
    GameObject.cpp
    GameObject.h
    Player.cpp
    Player.h
    Bullet.cpp
    Bullet.h
    Platform.cpp
    Platform.h
    Zombie.cpp
    Zombie.h

)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)