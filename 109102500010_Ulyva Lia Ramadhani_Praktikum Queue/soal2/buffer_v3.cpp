#include "buffer.h"
#include <iostream>
#include <iomanip>
using namespace std;

#if VERSI != 3
#error "File ini harus dikompilasi dengan -DVERSI=3"
#endif

const char* namaVersi() { return "3 - Circular head+count"; }

void createBuffer(Buffer &B) {
    for (int i = 0; i < KAPASITAS; i++) { B.data[i].nomor = 0; B.data[i].nilai = 0; }
    B.head = 0; B.count = 0;
    B.nomorBerikutnya = 1; B.totalGeser = 0;
}

bool isEmptyBuffer(const Buffer &B) { return B.count == 0; }
bool isFullBuffer(const Buffer &B) { return nbBuffer(B) == KAPASITAS; }
int nbBuffer(const Buffer &B) { return B.count; }
int ruangKosong(const Buffer &B) { return KAPASITAS - nbBuffer(B); }
int indeksFisik(const Buffer &B, int i) { return (B.head + i) % KAPASITAS; }
int nomorBerikut(const Buffer &B) { return B.nomorBerikutnya; }
long long totalGeser(const Buffer &B) { return B.totalGeser; }

bool enqueue(Buffer &B, int nilai) {
    if (isFullBuffer(B)) return false;
    int pos = (B.head + B.count) % KAPASITAS;
    B.data[pos].nomor = B.nomorBerikutnya++;
    B.data[pos].nilai = nilai;
    B.count++;
    return true;
}

bool dequeue(Buffer &B, Rekaman &keluar) {
    if (isEmptyBuffer(B)) return false;
    keluar = B.data[B.head];
    B.head = (B.head + 1) % KAPASITAS;
    B.count--;
    if (B.count == 0) B.head = 0;
    return true;
}

void kosongkan(Buffer &B) {
    for (int i = 0; i < KAPASITAS; i++) { B.data[i].nomor = 0; B.data[i].nilai = 0; }
    B.head = 0; B.count = 0;
}

void printInfo(const Buffer &B) {
    int n = nbBuffer(B);
    cout << "  Versi: " << namaVersi() << endl;
    cout << "  FIFO (depan->belakang): ";
    if (n == 0) cout << "KOSONG" << endl;
    else {
        for (int i = 0; i < n; i++) {
            const Rekaman &r = B.data[indeksFisik(B, i)];
            cout << r.nomor << ":" << r.nilai;
            if (i + 1 < n) cout << ", ";
        }
        cout << endl;
    }
    cout << "  Indeks fisik:" << endl;
    for (int idx = 0; idx < KAPASITAS; idx++) {
        cout << "    [" << idx << "] ";
        bool aktif = false;
        for (int i = 0; i < n; i++) if (indeksFisik(B, i) == idx) { aktif = true; break; }
        if (aktif) cout << B.data[idx].nomor << ":" << B.data[idx].nilai << "  (AKTIF)";
        else if (B.data[idx].nomor != 0) cout << B.data[idx].nomor << ":" << B.data[idx].nilai << "  (SISA)";
        else cout << "-  (KOSONG)";
        cout << endl;
    }
    cout << "  Penanda: head=" << B.head << "; count=" << B.count
         << " | jumlah=" << n << " | ruang kosong=" << ruangKosong(B)
         << " | nomor berikutnya=" << nomorBerikut(B)
         << " | total pergeseran=" << totalGeser(B) << endl;
}

void ringkas(const Buffer &B) {
    int n = nbBuffer(B);
    if (n == 0) {
        cout << "  Jumlah 0; total 0; minimum TIDAK ADA; maksimum TIDAK ADA; rata-rata TIDAK ADA" << endl;
        return;
    }
    long long total = 0;
    int minV = B.data[indeksFisik(B, 0)].nilai;
    int maxV = minV;
    for (int i = 0; i < n; i++) {
        int v = B.data[indeksFisik(B, i)].nilai;
        total += v;
        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
    }
    cout << "  Jumlah " << n << "; total " << total
         << "; minimum " << minV << "; maksimum " << maxV
         << "; rata-rata " << fixed << setprecision(2) << (double)total / n << endl;
}