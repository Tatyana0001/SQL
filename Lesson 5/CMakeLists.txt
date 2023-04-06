cmake_minimum_required(VERSION 3.20)
project(Lesson_5)
add_executable (Lesson_5 SQL.cpp)

add_subdirectory(C:/Users/kisa7/libpqxx-7.7.4 libpqxx-build)
target_compile_features(Lesson_5 PRIVATE cxx_std_17)
target_link_libraries(Lesson_5 pqxx)
