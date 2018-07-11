# CMake 最低版本号要求
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pthread")

#添加头文件搜索路径
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/key_center)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/socket)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/log)

#添加库文件搜索路径
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/sdk)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)

SET(LIBRARIES key_center log socket libgtest.a libgtest_main.a liblog4cplus.so libmysqlclient.so)

#用于将当前目录下的所有源文件的名字保存在变量 APP_SRC 中
AUX_SOURCE_DIRECTORY(. APP_SRC)

#编译出一个可执行文件且命名为Main
ADD_EXECUTABLE(wallet_front_gtest ${APP_SRC})

TARGET_LINK_LIBRARIES(wallet_front_gtest ${LIBRARIES} )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
