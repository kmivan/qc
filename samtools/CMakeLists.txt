set(CMAKE_C_STANDARD 99)

include_directories(.)
include_directories(lz4)

aux_source_directory(. SAMTOOLS_SRC)
aux_source_directory(lz4 LZ4_SRC)


add_library(samtools-lib ${SAMTOOLS_SRC} ${LZ4_SRC})
target_link_libraries(samtools-lib pthread hts z c m curses)

add_executable(samtools mainsrc/main.c)
target_link_libraries(samtools samtools-lib)
