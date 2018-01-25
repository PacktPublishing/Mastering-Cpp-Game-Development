cmake_minimum_required(VERSION 3.0)

project(BookEngine)
file(GLOB SOURCES ./*.cpp ./*/*.cpp ./*/*/*.cpp ./*/*/*/*.cpp ./*.h ./*/*.h ./*/*/*.h ./*/*/*/*.h)



include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/Windows/x86)

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(BookEngine SHARED ${SOURCES})

#target_link_libraries(BookEngine debug BookEngine)
target_link_libraries(BookEngine debug glew32 SDL2 SDL2main SDL2_ttf SDL2_mixer OpenGL32 BulletDynamics_debug BulletCollision_debug LinearMath_debug lua53.lib)
target_link_libraries(BookEngine optimized glew32 SDL2 SDL2main SDL2_ttf SDL2_mixer OpenGL32 BulletDynamics BulletCollision LinearMath lua53.lib)



