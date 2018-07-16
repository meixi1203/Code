cmake_minimum_required(VERSION 2.8)
project(test)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

SET(LIBRARIES   libopencv_shape.so libopencv_stitching.so libopencv_highgui.so
                libopencv_objdetect.so libopencv_superres.so libopencv_videostab.so
                libopencv_calib3d.so libopencv_features2d.so 
                libopencv_videoio.so libopencv_imgcodecs.so libopencv_video.so 
                libopencv_ml.so libopencv_imgproc.so libopencv_flann.so 
                libopencv_core.so libopencv_photo.so)

add_executable(Opencv Opencv.cpp)  
target_link_libraries(Opencv ${LIBRARIES})  
