include(CheckCXXCompilerFlag)

set(FUZZTEST_SOURCES
    simdtest.cpp)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
		endif()
endif(CCACHE_FOUND)

set_property(DIRECTORY PROPERTY COMPILE_OPTIONS ${EXTRA_CXX_FLAGS})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # If the user is running a newer version of Clang that includes the
    # -Wdouble-promotion, we will ignore that warning.
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.7)
        CHECK_CXX_COMPILER_FLAG("-Wno-double-promotion" HAS_NO_DOUBLE_PROMOTION)
        if (HAS_NO_DOUBLE_PROMOTION)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-double-promotion")
        endif()
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Force to always compile with /W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # Force to always compile with /WX
    if(CMAKE_CXX_FLAGS MATCHES "/WX-")
        string(REGEX REPLACE "/WX-" "/WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,fuzzer -DRAPIDJSON_HAS_STDSTRING=1")

add_executable(fuzztest ${FUZZTEST_SOURCES})
target_link_libraries(fuzztest fuzz_test)

add_dependencies(tests fuzztest)

add_test(NAME fuzztest
    COMMAND fuzz_test -o /tmp/fuzz_results -i ${CMAKE_SOURCE_DIR}/test/fuzztest -t 300 "${CMAKE_SOURCE_DIR}/build/bin/fuzztest"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
