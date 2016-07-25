set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/opt/opencv3/lib -I/usr/local/opt/opencv3/include -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core")

include_directories(../include)

set(SOURCE_FILES saturation.cpp sharpness.cpp brightness.cpp contrast.cpp vignette.cpp)
add_library(filatti ${SOURCE_FILES})
