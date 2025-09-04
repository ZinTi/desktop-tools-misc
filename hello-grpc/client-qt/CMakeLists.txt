cmake_minimum_required(VERSION 3.20)
project(client-qt VERSION 0.1 LANGUAGES CXX)

# 设置 Qt 安装路径
set(CMAKE_PREFIX_PATH
        "A:/build/qt-6.9.0-win64-mingw64-static"
        "A:/build/grpc-1.71.0-amd64-win-mingw64"
        "A:/build/openssl-win64-mingw"
)

# 设置 gRPC 和 Protobuf 的根目录
set(GRPC_INSTALL_DIR "A:/build/grpc-1.71.0-amd64-win-mingw64")
set(Protobuf_ROOT ${GRPC_INSTALL_DIR})
set(gRPC_ROOT ${GRPC_INSTALL_DIR})

# 设置 OpenSSL 库路径（mingw64静态库）
set(OPENSSL_ROOT_DIR "A:/build/openssl-win64-mingw")

# 设置 Protobuf 的 CMake 配置路径
set(Protobuf_DIR "${GRPC_INSTALL_DIR}/lib/cmake/protobuf")

# 添加自定义 CMake 模块路径（包含 grpc 的 Config.cmake）
list(APPEND CMAKE_MODULE_PATH "${GRPC_INSTALL_DIR}/cmake")

# 启用自动处理moc/uic/rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenSSL REQUIRED)

# 如果找不到 OpenSSL，手动指定
if (NOT OpenSSL_FOUND)
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libcrypto.a")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libssl.a")
    set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
endif()

# 查找 Protobuf 和 gRPC
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 设置源文件的变量
set(PROJECT_SOURCES
        # 主函数
        src/main.cpp

        # protoc generated
        src/gen/hello.pb.cc
        src/gen/hello.grpc.pb.cc

        # service 层
        src/svc/greetSvc.h
        src/svc/greetSvc.cpp

        # UI 层
        src/ui/MainWindow.cpp
        src/ui/MainWindow.h
        src/ui/MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(client-qt
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(client-qt SHARED
                ${PROJECT_SOURCES}
        )
    else()
        add_executable(client-qt
                ${PROJECT_SOURCES}
        )
    endif()
endif()

# 目标包含目录
target_include_directories(client-qt PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gen
        ${CMAKE_CURRENT_SOURCE_DIR}/src/svc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# 链接依赖库
target_link_libraries(client-qt PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets

        # gRPC相关库
        gRPC::grpc++
        gRPC::grpc
        #gRPC::grpc_cronet
        gRPC::gpr
        protobuf::libprotobuf

        # Abseil库（通过gRPC自动引入）

        # OpenSSL库
        "A:/build/grpc-1.71.0-amd64-win-mingw64/lib/libssl.a"
        "A:/build/grpc-1.71.0-amd64-win-mingw64/lib/libcrypto.a"

        # Windows系统库
        ws2_32
        crypt32
        gdi32
        userenv
)

# 优化二进制体积、静态编译配置
set_target_properties(client-qt PROPERTIES
        WIN32_EXECUTABLE TRUE
		RUNTIME_OUTPUT_DIRECTORY "A:/myproj/hello-grpc/bin" # 设置可执行文件的输出路径
        LINK_FLAGS "-static -Wl,-s"
)

# 静态编译专用配置
target_compile_definitions(client-qt PRIVATE
        QT_STATICPLUGIN
)

# Qt6静态插件最终化处理
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(client-qt)
endif()

# 构建命令示例
# mkdir build && cd build
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_MAKE_PROGRAM=mingw32-make ..
# mingw32-make -j16