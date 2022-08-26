aux_source_directory(. PIGZ_SRC)
aux_source_directory(zopfli/src/zopfli ZOPFLI_SRC)

add_library(pigz-lib SHARED ${PIGZ_SRC} ${ZOPFLI_SRC})
target_link_libraries(pigz-lib pthread z m)
