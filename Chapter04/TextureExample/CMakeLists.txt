cmake_minimum_required(VERSION 3.0)

project(TextureExample)

file(GLOB SOURCES ./*.cpp ./*/*.cpp ./*/*/*.cpp ./*/*/*/*.cpp ./*.h ./*/*.h ./*/*/*.h ./*/*/*/*.h)


include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib/Windows/x86)


add_executable(TextureExample ${SOURCES})

target_link_libraries(TextureExample debug BookEngine)
target_link_libraries(TextureExample optimized BookEngine)

file(COPY "${CMAKE_SOURCE_DIR}/lib/Windows/x86/Debug/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/" FILES_MATCHING PATTERN "*.dll")
file(COPY "${CMAKE_SOURCE_DIR}/BookEngine/Debug/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/" FILES_MATCHING PATTERN "*.dll")

file(COPY "${CMAKE_SOURCE_DIR}/lib/Windows/x86/Release/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/" FILES_MATCHING PATTERN "*.dll")
file(COPY "${CMAKE_SOURCE_DIR}/BookEngine/Release/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/" FILES_MATCHING PATTERN "*.dll")

file(COPY "${CMAKE_SOURCE_DIR}/Textures" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
#file(COPY "${CMAKE_SOURCE_DIR}/Textures/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/")

file(COPY "${CMAKE_SOURCE_DIR}/Shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
#file(COPY "${CMAKE_SOURCE_DIR}/Shaders/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/")
