set(SRC
    factory.cpp
    helpers.cpp
    nodes.cpp
    package.cpp
        simulation.cpp
    storage_types.cpp
    types.cpp
)

add_library(${PROJECT_NAME}_lib ${SRC})

target_include_directories(${PROJECT_NAME}_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)