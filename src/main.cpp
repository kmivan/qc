#include "libs.hpp"
#include "string"
#include "unistd.h"
#include "vector"
#include "fcntl.h"

using namespace std;

template <typename Func>
void run(Func function, const string& command);

void remove(const string& filename);
int system(const string& command);
string getcwd();

int main(int argc, char** argv)
{
    // 要用绝对路径，因为pigz要求将工作目录切换到文件所在目录下
    string inputFile = "/data-b/database/rawdata/metagenomics/LS454/SRR072232.fastq.gz";
    string outputFile = getcwd() + "/data/result.fq.gz";
    string adatperFile = "/data-b/software/miniconda3/share/trimmomatic-0.39-2/adapters/TruSeq2-SE.fa";
    string hg38File = "/data-b/database/UCSC/hg38";
    string chimFile = "/data-b/database/NCBI/chimpazee";

    // 切换工作目录
    chdir("/dev/shm/");

    freopen("0.fq", "w", stdout);
    run(seqtk_main, "seqtk trimfq -L 100 " + inputFile);
    fflush(stdout);
    freopen("/dev/tty", "w", stdout);
    run(pigz_main, "pigz -p 2 0.fq");

    run(fastp_main, "fastp -i 0.fq.gz -o 1.fq.gz --low_complexity_filter --trim_poly_g -q 20 -u 30 --adapter_fasta " + adatperFile + " --complexity_threshold 40");
    run(snap_aligner_main, "snap_aligner single " + hg38File + " 1.fq.gz -o 2.bam -t 2");
    remove("1.fq.gz");
    run(samtools_main, "samtools view -h -f 0x4 2.bam -o 3.bam");
    remove("2.bam");
    run(samtools_main, "samtools sort 3.bam -O BAM -o 4.bam");
    remove("3.bam");
    run(samtools_main, "samtools fastq 4.bam -@ 2 -o 5.fq");
    remove("4.bam");
    run(pigz_main, "pigz 5.fq"); // pigz 将在生成 5.fq.gz 的同时删除 5.fq，下同
    run(snap_aligner_main, "snap-aligner single " + chimFile + " 5.fq.gz -o 6.bam -t 2");
    remove("5.fq.gz");
    run(samtools_main, "samtools view -h -f 0x4 6.bam -o 7.bam");
    remove("6.bam");
    run(samtools_main, "samtools sort 7.bam -O BAM -o 8.bam");
    remove("7.bam");
    run(samtools_main, "samtools fastq 8.bam -@ 2 -o 9.fq");
    remove("8.bam");
    run(pigz_main, "pigz 9.fq");

    system("mv 9.fq.gz " + outputFile);
}

vector<string> splitOptions(const string& command)
{
    vector<string> result;
    int i = 0;
    int j = 1;
    while (j < command.size()) {
        while (j < command.size() && command[j] != ' ') {
            j++;
        }

        result.push_back(string(&command[i], &command[j]));

        while (j < command.size() && command[j] == ' ') {
            j++;
        }
        i = j;
    }

    return result;
}

template <typename Func>
void run(Func function, const string& command)
{
    vector<string>&& options = splitOptions(command);

    vector<char*> argv(options.size());
    for (int i = 0; i < options.size(); i++) {
        argv[i] = const_cast<char*>(options[i].data());
    }

    function(argv.size(), argv.data());
}

void remove(const string& filename)
{
    remove(filename.c_str());
}

int system(const string& command)
{
    return system(command.c_str());
}

string getcwd()
{
    string cwd;
    cwd.resize(256);
    getcwd((char*)cwd.c_str(), cwd.size());
    return cwd;
}
