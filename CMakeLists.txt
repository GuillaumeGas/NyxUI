cmake_minimum_required(VERSION 3.16)

project(NyxUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories("C:/Users/Guillaume/Documents/prog/c++/NyxQt")

link_directories("C:/Users/Guillaume/Documents/prog/c++/NyxQt/build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug")

set(PROJECT_SOURCES
        main.cpp


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NyxUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        images/ai.png images/empty.png images/wall.png
        resources.qrc



        map
        test.nx
        game/gameinfo.cpp game/gameinfo.h game/grid.cpp game/grid.h nyx/nyxsyscallsinterface.cpp nyx/nyxsyscallsinterface.h ui/codeeditor.cpp ui/codeeditor.h ui/highlighter.cpp ui/highlighter.h ui/linenumberwidget.cpp ui/linenumberwidget.h ui/mainwindow.cpp ui/mainwindow.h ui/renderer.cpp ui/renderer.h





    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET NyxUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(NyxUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NyxUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(NyxUI PRIVATE libNyxQt Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NyxUI)
endif()
set_target_properties(NyxUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS NyxUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET NyxUI POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "C:/Users/Guillaume/Documents/prog/c++/NyxQt/build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug/libNyxQt.dll"
  $<TARGET_FILE_DIR:NyxUI>)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NyxUI)
endif()
