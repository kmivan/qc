cmake_minimum_required(VERSION 3.10)

project(qc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_compile_definitions(AS_LIBRARY)

add_subdirectory(fastp)
add_subdirectory(snap)
add_subdirectory(samtools)
add_subdirectory(pigz)
add_subdirectory(seqtk)

include_directories(utils/yaml-cpp/include)

aux_source_directory(src QC_SRC)
add_executable(${PROJECT_NAME} ${QC_SRC})
target_link_libraries(${PROJECT_NAME} fastp-lib snap-aligner-lib samtools-lib pigz-lib seqtk-lib)
