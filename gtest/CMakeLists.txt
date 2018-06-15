# CMake 最低版本号要求
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pthread")

#CMake运行时，打印路径
MESSAGE(--${PROJECT_SOURCE_DIR}/lib)
MESSAGE(--${PROJECT_SOURCE_DIR}/bin)

#添加子目录
###

#添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

#添加库文件搜索路径
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/sdk)

#用于将当前目录下的所有源文件的名字保存在变量 APP_SRC 中
AUX_SOURCE_DIRECTORY(. APP_SRC)

#编译出一个可执行文件且命名为Main
ADD_EXECUTABLE(gtest_wallet_front ${APP_SRC})

TARGET_LINK_LIBRARIES(gtest_wallet_front libgtest.a  libgtest_main.a librequest_manager.a libzmq.so libprotobuf.so libprotoc.so libprotobuf-lite.so)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
