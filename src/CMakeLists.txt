link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )
   
add_executable(${PROJECT_NAME} # name of the executable on Windows will be example.exe 
    VectorImageConverter.cpp 
    MainWindow.cpp
    VPIConverter.cpp
    VPIConverter.h
    MPIConverter.cpp
    MPIConverter.h
    MainWindow.h )

target_link_libraries(${PROJECT_NAME} 
    ${GTKMM_LIBRARIES}  )
    
find_library(LIBRT rt) 
if(LIBRT)
	target_link_libraries(${PROJECT_NAME} ${LIBRT})
endif()
