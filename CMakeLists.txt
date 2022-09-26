# 最低cmake版本
cmake_minimum_required(VERSION 3.22)
# 以utf8编译
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# C++ 标准17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 声明C++项目
project(Suzuchan-3G LANGUAGES CXX)
# 寻找本地JNI文件
set(JAVA_AWT_LIBRARY NotNeeded)
set(JAVA_JVM_LIBRARY NotNeeded)
set(JAVA_AWT_INCLUDE_PATH NotNeeded)
find_package(JNI REQUIRED)
if (JNI_FOUND)
    message (STATUS "FOUND JNI IN ${JAVA_INCLUDE_PATH}")
    # 依赖文件路径
    set(SOURCE_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/single_include/3rd_include
            ${CMAKE_CURRENT_SOURCE_DIR}/single_include/3rd_include/utf8
            ${CMAKE_CURRENT_SOURCE_DIR}/single_include/3rd_include/utf8/utf8
            ${CMAKE_CURRENT_SOURCE_DIR}/single_include/MiraiCP
            ${JAVA_INCLUDE_PATH}/
            ${JAVA_INCLUDE_PATH2}/
            )
else()
    message(STATUS "Cannot find JNI")
endif()

find_package(Boost REQUIRED)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
endif()

find_package (Python3 3.10 EXACT COMPONENTS Interpreter Development Development.Embed REQUIRED)
if (Python3_FOUND)
    include_directories(${Python3_INCLUDE_DIRS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpython3.10 -lcrypt -ldl  -lm -lm -Xlinker -export-dynamic")

    MESSAGE( STATUS "Python3_VERSION = ${Python3_VERSION}")
endif()

# 添加动态库编译目标
add_library(suzuchan3g
        SHARED
        single_include/MiraiCP/MiraiCP.cpp # MiraiCP 源文件
        main.cpp
        Suzuchan.cpp Suzuchan.h
        exprtk/exprtk.hpp Suzuchan-math.cpp)

# 引入全部依赖文件路径
target_include_directories(suzuchan3g PUBLIC ${SOURCE_PATH})

target_link_libraries(suzuchan3g ${Python3_LIBRARIES} sqlite3)
