cmake_minimum_required(VERSION 3.5)

project ( SkodaParsePackages )

set ( CMAKE_C_STANDARD 99 )
set ( CMAKE_CXX_STANDARD 20 )
set ( CMAKE_C_FLAGS "-Wall -Wextra -Werror -g3 -o0" )
set ( CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -g3 -o0" )

include_directories (
	.
     )

add_executable(SkodaParsePackages_app
	package_generator.cpp
	data_loader.h
	common_types.h
	entity_properties.h
	package_properties.h
	)

