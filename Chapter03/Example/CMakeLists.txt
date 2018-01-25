cmake_minimum_required(VERSION 3.0)

project(Example)

file(GLOB SOURCES ./*.cpp ./*/*.cpp ./*/*/*.cpp ./*/*/*/*.cpp ./*.c ./*/*.c ./*/*/*.c ./*/*/*/*.c)


include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib/Windows/x86)


add_executable(Example ${SOURCES})

target_link_libraries(Example debug BookEngine)
target_link_libraries(Example optimized BookEngine)

file(COPY "${CMAKE_SOURCE_DIR}/lib/Windows/x86/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/" FILES_MATCHING PATTERN "*.dll")
file(COPY "${CMAKE_SOURCE_DIR}/BookEngine/Debug/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/" FILES_MATCHING PATTERN "*.dll")

file(COPY "${CMAKE_SOURCE_DIR}/lib/Windows/x86/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/" FILES_MATCHING PATTERN "*.dll")
file(COPY "${CMAKE_SOURCE_DIR}/BookEngine/Release/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/" FILES_MATCHING PATTERN "*.dll")



