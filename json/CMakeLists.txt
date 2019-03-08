cmake_minimum_required (VERSION 2.6)

project (sample)
find_package( OpenCV REQUIRED )

add_executable( sample sample.cc )
target_link_libraries( sample jsoncpp )

add_executable( correct correct.cc )

add_executable( getbox getbox.cc )
target_link_libraries( getbox jsoncpp )

add_executable( showbox showbox.cc )
target_link_libraries( showbox ${OpenCV_LIBS} jsoncpp )

add_executable( showbox1 showbox1.cc )
target_link_libraries( showbox1 ${OpenCV_LIBS} jsoncpp )

add_executable( movbox movbox.cc )
target_link_libraries( movbox ${OpenCV_LIBS} jsoncpp )
