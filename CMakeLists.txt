cmake_minimum_required(VERSION 3.23)
project(basic_algorithms)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.1.1
)

FetchContent_MakeAvailable(fmt)

add_executable(knn knn.cpp file_reader.cpp)
add_executable(perceptron perceptron.cpp file_reader.cpp)
add_executable(bayes bayes.cpp file_reader.cpp)
add_executable(k-means k-means.cpp file_reader.cpp)

target_link_libraries(knn fmt)
target_link_libraries(perceptron fmt)
target_link_libraries(bayes fmt)
target_link_libraries(k-means fmt)