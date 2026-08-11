cmake_minimum_required(VERSION 4.2)
project(fast_compeptative)

set(CMAKE_CXX_STANDARD 20)

add_executable(fast_compeptative main.cpp
        utils.cpp
        utils.h
        exemple.cpp
        exemple.h
        A.cpp
        A.h
        B.cpp
        B.h
        C.cpp
        C.h
        D.cpp
        D.h
        E.cpp
        E.h
        F.cpp
        F.h
        G.cpp
        G.h
        H.cpp
        H.h)

add_executable(make_ready make_ready.cpp
        utils.cpp
        utils.h
        exemple.cpp
        exemple.h)
