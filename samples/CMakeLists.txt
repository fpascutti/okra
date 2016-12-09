
# Copyright Franck Pascutti 2016.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(okra-tests CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

function(okra_sample name)
    add_executable(${name} ${ARGN})
    target_sources(${name}
        PRIVATE gtest-main.cpp
    )
    target_link_libraries(${name}
        PRIVATE okra
        PRIVATE ${CONAN_LIBS_GTEST} gmock gmock_main
    )
endfunction()

okra_sample(inline-scenario
    inline-scenario/inline-scenario.cpp
)
