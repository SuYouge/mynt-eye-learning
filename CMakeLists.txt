cmake_minimum_required(VERSION 3.0)

project(mynteye_demo VERSION 1.0.0 LANGUAGES C CXX)

# flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++14 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native")

# packages

if(MSVC)
  set(SDK_ROOT "$ENV{MYNTEYES_SDK_ROOT}")
  if(SDK_ROOT)
    message(STATUS "MYNTEYES_SDK_ROOT: ${SDK_ROOT}")
    list(APPEND CMAKE_PREFIX_PATH
      "${SDK_ROOT}/lib/cmake"
      "${SDK_ROOT}/3rdparty/opencv/build"
    )
  else()
    message(FATAL_ERROR "MYNTEYES_SDK_ROOT not found, please install SDK firstly")
  endif()
endif()

## mynteye

find_package(mynteye REQUIRED)
message(STATUS "Found mynteye: ${mynteye_VERSION}")

# When SDK build with OpenCV, we can add WITH_OPENCV macro to enable some
# features depending on OpenCV, such as ToMat().
if(mynteye_WITH_OPENCV)
  add_definitions(-DWITH_OPENCV)
endif()

## OpenCV

# Set where to find OpenCV
#set(OpenCV_DIR "/usr/share/OpenCV")

# When SDK build with OpenCV, we must find the same version here.
find_package(OpenCV REQUIRED)
message(STATUS "Found OpenCV: ${OpenCV_VERSION}")

find_package(PCL REQUIRED)
message(STATUS "Found PCL: ${PCL_VERSION}")

# find_package(PCL REQUIRED)
# message(STATUS "Found PCL: ${PCL_VERSION}")

# targets

include_directories(
  ${OpenCV_INCLUDE_DIRS},
  ${PCL_INCLUDE_DIRS},
  ${PCL_INCLUDE_DIRS}/visualization,
  "/usr/include/vtk-6.3"
)

## mynteye_demo
# exe
####################################################################################
add_subdirectory(util_cv)
# add_subdirectory(util_pcl)
# add_subdirectory(util_dataset)

# add_executable(mynteye_demo mynteye_demo.cc)
# target_link_libraries(mynteye_demo mynteye ${OpenCV_LIBS})

# add_executable(get_img_params get_img_params.cc)
# target_link_libraries(get_img_params mynteye ${OpenCV_LIBS})

# add_executable(get_device_info get_device_info.cc)
# target_link_libraries(get_device_info mynteye ${OpenCV_LIBS})

# add_executable(get_stereo_rectified get_stereo_rectified.cc)
# target_link_libraries(get_stereo_rectified mynteye ${OpenCV_LIBS})

# add_executable(get_imu_params get_imu_params.cc)
# target_link_libraries(get_imu_params mynteye util_cv ${OpenCV_LIBS})

# add_executable(camera_with_junior_device_api camera_with_junior_device_api.cc)
# target_link_libraries(camera_with_junior_device_api mynteye ${OpenCV_LIBS})

# add_executable(get_disparity get_disparity.cc)
# target_link_libraries(get_disparity mynteye ${OpenCV_LIBS})

# add_executable(get_depth get_depth.cc)
# target_link_libraries(get_depth mynteye ${OpenCV_LIBS})

# add_executable(get_depth_and_points get_depth_and_points.cc)
# target_link_libraries(get_depth_and_points mynteye util_pcl util_cv 
#   # link阶段出现DSO问题时 添加相应的库名称到这里即可
#   pcl_visualization vtkInteractionStyle-6.3 vtkRenderingOpenGL-6.3 vtkRenderingFreeType-6.3 
#   vtkRenderingCore-6.3  vtkCommonDataModel-6.3 vtkCommonMath-6.3 vtkCommonCore-6.3 
#   ${OpenCV_LIBS})

# add_executable(save_single_image save_single_image.cc)
# target_link_libraries(save_single_image mynteye ${OpenCV_LIBS})

# add_executable(get_imu_correspondence get_imu_correspondence.cc)
# target_link_libraries(get_imu_correspondence mynteye util_cv ${OpenCV_LIBS})

# add_executable(get_imu get_imu.cc)
# target_link_libraries(get_imu mynteye util_cv ${OpenCV_LIBS})

# add_executable(record record.cc)
# target_link_libraries(record mynteye util_dataset ${OpenCV_LIBS})

# add_executable(get_data_withoutselect get_data_withoutselect.cc)
# target_link_libraries(get_data_withoutselect mynteye ${OpenCV_LIBS})

# add_executable(get_depth_with_region get_depth_with_region.cc)
# target_link_libraries(get_depth_with_region mynteye ${OpenCV_LIBS})

add_executable(get_from_callbacks get_from_callbacks.cc)
target_link_libraries(get_from_callbacks mynteye util_cv ${OpenCV_LIBS})

####################################################################################
# 每个main单独编译
# file(GLOB_RECURSE mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
# foreach(mainfile IN LISTS mains)
#     # Get file name without directory
#     get_filename_component(mainname ${mainfile} NAME_WE)
#     add_executable(${mainname} ${mainfile})
#     target_link_libraries(${mainname} mynteye util_cv ${OpenCV_LIBS})
# endforeach()

# aux_source_directory(. ALL_SRCS)			# 添加当前目录下所有的源文件
# add_subdirectory(cv_util)						# 添加math子目录
# add_executable(mynteye_demo ${ALL_SRCS})	# 指定生成目标，注意这里要用${ALL_SRCS}!!!
# target_link_libraries(mynteye_demo mynteye cv_util ${OpenCV_LIBS} )	# 添加链接库，power是在math子目录的CMakeLists中定义的

# 不允许多次定义`main`
# add_subdirectory(util_cv)
# file(GLOB_RECURSE c_files ${PROJECT_SOURCE_DIR}/*.cc)
# add_executable(${PROJECT_NAME} ${c_files})
# target_link_libraries(${PROJECT_NAME} mynteye util_cv ${OpenCV_LIBS})

# Build
#   mkdir _build
#   cd _build
#
#   # win 
#   cmake -G "Visual Studio 15 2017 Win64" ..
#   msbuild.exe ALL_BUILD.vcxproj /property:Configuration=Release
#
#   .\Release\mynteye_demo.exe
#   
#   # unix
#   cmake ..
#   make
#
#   ./mynteye_demo
