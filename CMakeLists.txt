cmake_minimum_required(VERSION 3.10.2)
project(make_project)

include_directories(includes)

set(SOURCES src/main.c
	    src/client.c
	    src/server.c)

add_executable(main
		${SOURCES})
