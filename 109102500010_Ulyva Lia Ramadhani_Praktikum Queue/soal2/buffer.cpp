#include "buffer.h"
#include <iostream>
#include <iomanip>
using namespace std;

// fungsi-fungsi yang sama buat semua versi

int nomorBerikut(const Buffer &B) {
    return B.nomorBerikutnya;
}

long long totalGeser(const Buffer &B) {
    return B.totalGeser;
}

bool isFullBuffer(const Buffer &B) {
    return nbBuffer(B) == KAPASITAS;
}

int ruangKosong(const Buffer &B) {
    return KAPASITAS - nbBuffer(B);
}

// cek apakah indeks fisik idx itu slot aktif
bool slotAktif(const Buffer &B, int idx) {
    if (idx < 0 || idx >= KAPASITAS) return false;
    int n = nbBuffer(B);
    for (int i = 0; i < n; i++) {
        if (indeksFisik(B, i) == idx) return true;
    }
    return false;
}

void printInfo(const Buffer &B) {
    int n = nbBuffer(B);
    cout << "  Versi: " << namaVersi() << endl;
    cout << "  FIFO (depan->belakang): ";
    if (n == 0) {
        cout << "KOSONG" << endl;
    } else {
        for (int i = 0; i < n; i++) {
            const Rekaman &r = B.data[indeksFisik(B, i)];
            cout << r.nomor << ":" << r.values;
            if (i + 1 < n) cout << ", ";
        }
        cout << endl;
    }
    
    cout << "  Indeks fisik:" << endl;
    for (int idx = 0; idx < KAPASITAS; idx++) {
        cout << "    [" << idx << "] ";
        if (slotAktif(B, idx)) {
            cout << B.data[idx].nomor << ":" << B.data[idx].nilai << "  (AKTIF)";
        } else if (B.data[idx].nomor != 0) {
            cout << B.data[idx].nomor << ":" << B.data[idx].nilai << "  (SISA)";
        } else {
            cout << "-  (KOSONG)";
        }
        cout << endl;
    }
    
    cout << "  Penanda: ";
#if VERSI == 3
    cout << "head=" << B.head << "; count=" << B.count;
#else
    cout << "head=" << B.head << "; tail=" << B.tail;
#endif
    cout << " | jumlah=" << n 
         << " | ruang kosong=" << ruangKosong(B)
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
    double rata = (double)total / n;
    cout << "  Jumlah " << n 
         << "; total " << total 
         << "; minimum " << minV 
         << "; maksimum " << maxV 
         << "; rata-rata " << fixed << setprecision(2) << rata << endl;
}