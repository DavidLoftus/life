
add_executable(sfml main.cpp)

target_link_libraries(sfml life)

find_package(SFML 2 COMPONENTS graphics audio REQUIRED)
target_link_libraries(sfml sfml-graphics)