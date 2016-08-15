set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/opt/opencv3/lib -I/usr/local/opt/opencv3/include -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core")

include_directories(../include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build/)

add_executable(sharpness_test sharpness_test.cpp)
target_link_libraries(sharpness_test filatti)

add_executable(hls_test hls_test.cpp)
target_link_libraries(hls_test filatti)

add_executable(contrast_test contrast_test.cpp)
target_link_libraries(contrast_test filatti)

add_executable(vignette_test vignette_test.cpp)
target_link_libraries(vignette_test filatti)

add_executable(temperature_test temperature_test.cpp)
target_link_libraries(temperature_test filatti)

add_executable(curves_test curves_test.cpp)
target_link_libraries(curves_test filatti)
