cmake_minimum_required(VERSION 3.16)
project(BallSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置Qt6路径
set(QT6_STATIC_PATH "A:/build/qt-6.9.0-win64-mingw64-static")
set(CMAKE_PREFIX_PATH ${QT6_STATIC_PATH})

# 查找Qt6组件
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/inc)

# 源文件和头文件
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/canvas.cpp
    src/ball.cpp
    inc/ball.h
    inc/canvas.h
    inc/mainwindow.h
)

# 生成可执行文件 - 添加 WIN32 参数去除控制台窗口
add_executable(BallSimulation WIN32 ${SOURCES})

# 启用Qt元对象系统
set_target_properties(BallSimulation PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

# 链接Qt库
target_link_libraries(BallSimulation
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# 设置静态链接标准库
if(MINGW)
    # 静态链接所有标准库
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    
    # 可选：单独指定静态链接库
    # target_link_libraries(BallSimulation 
    #     -static-libgcc 
    #     -static-libstdc++
    # )
endif()

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)