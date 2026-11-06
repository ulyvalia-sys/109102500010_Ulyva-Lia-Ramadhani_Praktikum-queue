#ifndef BUFFER_H
#define BUFFER_H

// VERSI diset waktu compile: -DVERSI=1 / 2 / 3
#ifndef VERSI
#define VERSI 1
#endif

const int KAPASITAS = 6;
const int NILAI_MIN = -1000;
const int NILAI_MAX = 1000;

struct Rekaman {
    int nomor;
    int nilai;
};

struct Buffer {
    Rekaman data[KAPASITAS];
    int head;
#if VERSI == 3
    int count;   // versi 3 pake count, ga pake tail
#else
    int tail;    // versi 1 & 2 pake tail
#endif
    int nomorBerikutnya;
    long long totalGeser;
};

// ===== fungsi ADT =====
void createBuffer(Buffer &B);
bool isEmptyBuffer(const Buffer &B);
bool isFullBuffer(const Buffer &B);
int  nbBuffer(const Buffer &B);
int  ruangKosong(const Buffer &B);
int  indeksFisik(const Buffer &B, int i);

bool enqueue(Buffer &B, int nilai);
bool dequeue(Buffer &B, Rekaman &keluar);

void kosongkan(Buffer &B);
void printInfo(const Buffer &B);
void ringkas(const Buffer &B);

// helper
const char* namaVersi();
int  nomorBerikut(const Buffer &B);
long long totalGeser(const Buffer &B);

#endif