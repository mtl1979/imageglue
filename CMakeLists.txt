cmake_minimum_required(VERSION 3.16)

project(ImageGlue VERSION 1.0.0 LANGUAGES CXX)

find_package(ZLIB REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

add_subdirectory(src)

include(CPack)