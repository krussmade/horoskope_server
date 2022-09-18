cmake_minimum_required(VERSION 3.20)
project(horoscope_server)

set(CMAKE_CXX_STANDARD 23)

add_executable(horoscope_server
        main.cpp
        libs/html_parser.hpp
        libs/httplib.h
        libs/json.hpp
        src/config.h
        src/repository.cpp
        src/repository.h
        src/utils.h
        src/models.h
        libs/sqlite/shell.c
        libs/sqlite/sqlite3.c
        libs/sqlite/sqlite3.h
        libs/sqlite/sqlite3ext.h
        libs/sqlitepp.h)
