 
cmake_minimum_required(VERSION 3.4)
project(mini_mcm)
find_package(CUDA)
find_package(boost)
if (WIN32)
add_definitions(-DWIN32)
else()
add_definitions(-D__LINUX__)
endif()
add_definitions(-DBOOST_SP_USE_STD_ATOMIC)

include_directories(${CMAKE_SOURCE_DIR}/../crypto
		${CMAKE_SOURCE_DIR}/../crypto/wots
		${CMAKE_SOURCE_DIR}/../
)
set(CMAKE_CXX_FLAGS -std=c++11)


set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}
        -gencode=arch=compute_50,code=sm_50
        -gencode=arch=compute_52,code=sm_52
        -gencode=arch=compute_60,code=sm_60
        -gencode=arch=compute_61,code=sm_61
        -gencode=arch=compute_70,code=sm_70
        -gencode=arch=compute_70,code=compute_70
        -lcudart -lcudadevrt -lcuda -Xptxas -O3
        -Wno-deprecated-gpu-targets -default-stream per-thread
        )

set (mini_mcm
../trigg/cuda_trigg.cu
../mini_mcm.c
../crypto/sha256.c
../crypto/wots/wots.c
../trigg/trigg.c
)
set(DKATNETGETWORK ${CMAKE_SOURCE_DIR}/../dependencies/DKatNetGetWork/libDKatNetGetWork.a)

include_directories(${CMAKE_SOURCE_DIR}/../dependencies/DKatNetGetWork/)
cuda_add_executable(MCM_Cuda ${mini_mcm})
target_link_libraries(MCM_Cuda ${DKATNETGETWORK} crypto ssl boost_thread boost_system stdc++ cpprest cuda nvidia-ml)
