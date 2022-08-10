cmake_minimum_required(VERSION 3.0)

project(main)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/cssparser/CSSParser.cpp
    ${PROJECT_SOURCE_DIR}/cssparser/CSSProperties.cpp
    ${PROJECT_SOURCE_DIR}/cssparser/CSSUtils.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/cmdline
    ${PROJECT_SOURCE_DIR}/cssparser
)
