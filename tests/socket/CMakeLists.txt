# CMake 最低版本号要求
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

#用于将当前目录下的所有源文件的名字保存在变量 LIB1_SRC 中
AUX_SOURCE_DIRECTORY(. LIB_SRC)

#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/mylog)
#输出静态库文(Archive)件，第一个参数指的是库的名字
ADD_LIBRARY(socket ${LIB_SRC})


#所以编译出来的库文件会输出到这个文件夹下的lib子目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)