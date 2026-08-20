cmake_minimum_required(VERSION 3.16)
project(PrimeMover LANGUAGES C)
set(CMAKE_C_STANDARD 23)

add_executable(${PROJECT_NAME}
    src/main.c
    src/cog.c
    src/logger.c
    src/function_registry.c
    src/info.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
