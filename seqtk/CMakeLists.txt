add_library(seqtk-lib SHARED seqtk.c khash.h kseq.h)
target_link_libraries(seqtk-lib z m)
