cmake_minimum_required(VERSION 3.10)
project(httb C CXX)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS
    include/httb/httb.h
    include/httb/io_container.h
    include/httb/body.h
    include/httb/body_multipart.h
    include/httb/body_form_urlencoded.h
    include/httb/body_string.h
    include/httb/types.h
    src/async_session.h
    src/utils.h
    )

set(SOURCES
    src/client.cpp
    src/request.cpp
    src/response.cpp
    src/io_container.cpp
    src/body_string.cpp
    src/body_multipart.cpp
    src/body_form_urlencoded.cpp
    src/async_session.cpp
    )

enable_testing()

find_package(Boost 1.70 REQUIRED filesystem regex)
find_package(OpenSSL REQUIRED)
add_subdirectory(3rd-party/toolbox)
add_subdirectory(3rd-party/googletest)

add_library(httb-lib STATIC ${SOURCES})

target_compile_options(httb-lib PUBLIC -fPIC -pedantic -Wall -Wsign-compare -Wunused-parameter)

target_include_directories(httb-lib PUBLIC include)
target_include_directories(httb-lib PUBLIC ${Boost_INCLUDE_DIRS})
target_compile_definitions(httb-lib PUBLIC -DHTTB_VERSION="1.0.0")

target_link_libraries(httb-lib ${Boost_LIBRARIES})
target_link_libraries(httb-lib toolbox)
target_link_libraries(httb-lib OpenSSL::SSL OpenSSL::Crypto)

add_executable(httb-lib-test tests/HttpClientTest.cpp tests/HttpClientNoLocalTest.cpp)
target_link_libraries(httb-lib-test httb-lib gtest gtest_main)
target_include_directories(httb-lib-test PRIVATE ./3rd-party/googletest/googletest/include/)
target_compile_definitions(httb-lib-test PUBLIC -DTEST_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/tests")
add_test(NAME httb-lib-test-driver COMMAND $<TARGET_FILE:httb-lib-test>)