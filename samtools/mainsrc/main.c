#ifdef AS_LIBRARY

int samtools_main(int argc, char **argv);

int main() {
    const char *argv[] = {"samtools", "view", "-f", "0x4", "2.bam"};
    samtools_main(sizeof argv / sizeof *argv, argv);
}

#endif
