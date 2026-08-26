cmake_minimum_required(VERSION 3.16)
project(PrimeMover LANGUAGES C)
set(CMAKE_C_STANDARD 23)
find_package(RaeptorCogs CONFIG REQUIRED)
find_package(Cog CONFIG REQUIRED)

CogCreate(${PROJECT_NAME}
    src/RaeptorCogs/${PROJECT_NAME}/prime_mover.c
    src/RaeptorCogs/${PROJECT_NAME}/cog.c
    src/RaeptorCogs/${PROJECT_NAME}/logger.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
