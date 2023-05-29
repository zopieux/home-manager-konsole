cmake_minimum_required(VERSION 3.16)

project(home-manager-konsole VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (QT_MIN_VERSION "5.15.0")
set (KF_MIN_VERSION "5.71.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)

find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS Core)
find_package(KF${QT_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED Config I18n XmlGui)

add_executable(konsole-update update.cpp)
add_executable(konsole-gen-state gen.cpp)

set(LIBS KF${QT_MAJOR_VERSION}::XmlGui KF${QT_MAJOR_VERSION}::I18n Qt${QT_MAJOR_VERSION}::Core)
target_link_libraries(konsole-update PRIVATE ${LIBS})
target_link_libraries(konsole-gen-state PRIVATE ${LIBS})

install(TARGETS konsole-update RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) 
install(TARGETS konsole-gen-state RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}) 
