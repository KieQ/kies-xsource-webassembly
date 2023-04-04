cmake_minimum_required(VERSION 3.20)

set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE /root/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
set(CMAKE_TOOLCHAIN_FILE /root/kies-xsource-webassembly/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(kies)

set(CMAKE_CXX_STANDARD 20)

find_package(cryptopp REQUIRED)

add_executable(kie_crypto main.cpp)
set(CMAKE_EXECUTABLE_SUFFIX ".js")
target_link_options(kie_crypto PUBLIC --bind -sEXPORT_ES6=1)
target_link_libraries(kie_crypto PRIVATE cryptopp::cryptopp)