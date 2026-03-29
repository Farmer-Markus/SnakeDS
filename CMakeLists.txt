cmake_minimum_required(VERSION 4.0)

if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "Environment variable DEVKITPRO not set! Unable to find nds cmake toolchain")
endif()
set(DEVKITPRO "$ENV{DEVKITPRO}")
set(CMAKE_TOOLCHAIN_FILE "${DEVKITPRO}/cmake/NDS.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(snake-nds)

# Build textures
set(IMG_SRC
    "data/palette.png"
    "data/apple.png"
    "data/background.png"
    "data/snake_head.png"
    "data/snake_body.png"
    "data/snake_tail.png"
    "data/snake_turn.png"
)

set(IMG_BIN_DIR ${CMAKE_BINARY_DIR}/include/data)
set(IMG_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
set(GRIT_BIN "${DEVKITPRO}/tools/bin/grit")

file(MAKE_DIRECTORY ${IMG_BIN_DIR})

foreach(IMG ${IMG_SRC})
    get_filename_component(IMG_NAME ${IMG} NAME_WLE)
    set(IMG_GRIT ${PROJECT_SOURCE_DIR}/data/${IMG_NAME}.grit)

    if(NOT EXISTS ${IMG_GRIT})
        set(IMG_GRIT ${PROJECT_SOURCE_DIR}/data/common.grit)
    endif()

    set(IMG_OUT_S ${IMG_BIN_DIR}/${IMG_NAME}.s)
    set(IMG_OUT_H ${IMG_BIN_DIR}/${IMG_NAME}.h)

    add_custom_command(
        OUTPUT ${IMG_OUT_S} ${IMG_OUT_H}
        COMMAND ${GRIT_BIN} ${PROJECT_SOURCE_DIR}/${IMG} -fts -o ${IMG_OUT_S} -ff ${IMG_GRIT}
        DEPENDS ${PROJECT_SOURCE_DIR}/${IMG} ${IMG_GRIT}
        COMMENT "Convert texture with grit"
    )

    list(APPEND IMG_GENERATED ${IMG_OUT_S})
endforeach()

# Build executable
enable_language(ASM)
add_executable(snake src/main.cpp src/Snake.cpp src/Grid.cpp ${IMG_GENERATED})

target_include_directories(snake PUBLIC ${IMG_INCLUDE_DIR} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(snake nds9)

nds_create_rom(snake)
