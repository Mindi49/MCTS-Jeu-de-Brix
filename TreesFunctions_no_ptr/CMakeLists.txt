cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

project(TreesFunctions_noptr)
add_executable(${PROJECT_NAME} "main.cc" "node.cc" "treeutil.cc" "tree.cc" "mcts.cc")
