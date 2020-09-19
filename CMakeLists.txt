cmake_minimum_required(VERSION 3.5)

project(dlibapp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /home/victor/.local/lib)
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()
find_package(Qt5 COMPONENTS Test Widgets Multimedia MultimediaWidgets Network REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia MultimediaWidgets REQUIRED)
find_package(dlib REQUIRED)

add_subdirectory(libs/qhttp)
#add_subdirectory(./libs/qhttp)
enable_testing(true)
include_directories(libs/qhttp/include)

add_library(facedetector facedetector.h facedetector.cpp)
add_library(config SHARED config.cpp config.h)
target_link_libraries(config PRIVATE Qt5::Core)
target_link_libraries(facedetector PRIVATE dlib::dlib Qt5::Widgets config )

add_library(net SHARED server.cpp
    server.h
    clienthandler.cpp
    clienthandler.h
    pathmatcher.cpp
    pathmatcher.h)

target_link_libraries(net PRIVATE qhttp config Qt5::Network)

if(ANDROID)
  add_library(dlibapp SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(dlibapp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Singleton.h
#    facedetector.cpp
#    facedetector.h
    imageviewerform.cpp
    imageviewerform.h
    imageviewerform.ui
    res.qrc
#    config.cpp
#    config.h
  )
endif()

target_link_libraries(dlibapp
    dlib::dlib
    Qt5::Widgets 
    Qt5::Multimedia 
    Qt5::MultimediaWidgets
    facedetector
    config
    qhttp
    net)

add_executable(pathmatchertest 
    pathmatchertest.cpp 
    pathmatchertest.h )
add_test(NAME pathmatchertest COMMAND pathmatchertest)
target_link_libraries(pathmatchertest Qt5::Test net)