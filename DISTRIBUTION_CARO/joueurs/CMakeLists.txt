cmake_minimum_required(VERSION 3.0)

project(joueurs)

find_package(Threads)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
add_compile_options(-Wall -Wpedantic)


set(SRCS
    joueur.cc
    joueur_random.cc
    joueur_manuel.cc
    joueur_montecarlo_ABBEL.cc
    treeutil_ABBEL.cc
    )
    
set(HEADERS
    joueur.hh
    joueur_random.hh
    joueur_manuel.hh
    joueur_montecarlo_ABBEL.hh
    value_ABBEL.hh
    node_ABBEL.hh
    tree_ABBEL.hh
    treeutil_ABBEL.hh
    systemutil_ABBEL.hh
    )

add_library(joueurs ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
