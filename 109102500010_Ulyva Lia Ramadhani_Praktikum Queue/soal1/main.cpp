#include "queue.h"
#include <iostream>
#include <sstream>
#include <cctype>
using namespace std;

// ubah string ke upper case
string toUpper(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

// cek apakah string itu integer (boleh negatif)
bool isInteger(string s) {
    if (s.empty()) return false;
    int mulai = 0;
    if (s[0] == '-' || s[0] == '+') mulai = 1;
    if (mulai >= (int)s.length()) return false;
    for (int i = mulai; i < (int)s.length(); i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

// cek kode valid: 1-10 karakter
bool kodeValid(string kode) {
    return kode.length() >= 1 && kode.length() <= 10;
}

// cek berat valid: 1-5000
bool beratValid(int berat) {
    return berat >= 1 && berat <= 5000;
}

// posisi elemen dalam queue (1-based), 0 kalau ga ada
int posisiElemen(const Queue &Q, address target) {
    int pos = 1;
    address p = Q.head;
    while (p != nullptr) {
        if (p == target) return pos;
        pos++;
        p = p->next;
    }
    return 0;
}

void cetakRekap(const Queue &A, const Queue &B, const Statistik &S) {
    cout << "  A        : " << nbList(A) << " paket, total " << totalBerat(A) << " gram" << endl;
    cout << "  B        : " << nbList(B) << " paket, total " << totalBerat(B) << " gram" << endl;
    cout << "  TERKIRIM : " << S.jumlah << " paket, total " << S.totalBerat << " gram" << endl;
}

int main() {
    Queue A, B;
    createQueue(A);
    createQueue(B);

    Statistik kirim;
    kirim.jumlah = 0;
    kirim.totalBerat = 0;

    cout << "=== SISTEM SORTIR & PENGIRIMAN PAKET ===" << endl;
    cout << "Perintah: MASUK | PINDAH | KIRIM | CARI | LIHAT | REKAP | BERSIHKAN | SELESAI" << endl;

    string baris;
    while (true) {
        cout << "> ";
        if (!getline(cin, baris)) break;

        istringstream iss(baris);
        string cmd;
        if (!(iss >> cmd)) continue;
        cmd = toUpper(cmd);

        if (cmd == "SELESAI") {
            delAll(A);
            delAll(B);
            cout << "Program selesai. Statistik akhir:" << endl;
            cetakRekap(A, B, kirim);
            break;
        }
        else if (cmd == "MASUK") {
            string kode, sBerat;
            if (!(iss >> kode >> sBerat)) {
                cout << "DITOLAK: format salah, pakai MASUK kode berat" << endl;
                continue;
            }
            // cek ga ada argumen sisa
            string sisa;
            if (iss >> sisa) {
                cout << "DITOLAK: argumen berlebih" << endl;
                continue;
            }
            if (!kodeValid(kode)) {
                cout << "DITOLAK: kode harus 1-10 karakter" << endl;
                continue;
            }
            if (!isInteger(sBerat)) {
                cout << "DITOLAK: berat harus bilangan bulat 1-5000" << endl;
                continue;
            }
            int berat = stoi(sBerat);
            if (!beratValid(berat)) {
                cout << "DITOLAK: berat harus 1-5000 gram" << endl;
                continue;
            }
            // cek duplikat
            if (fFindElm(A, kode) || fFindElm(B, kode)) {
                cout << "DITOLAK: kode '" << kode << "' masih aktif" << endl;
                continue;
            }
            // bikin node baru
            infotype data;
            data.kode = kode;
            data.berat = berat;
            address p = alokasi(data);
            if (p == nullptr) {
                cout << "DITOLAK: alokasi gagal" << endl;
                continue;
            }
            enqueue(A, p);
            cout << "OK: " << kode << " (" << berat << " gram) masuk ke A, posisi " << nbList(A) << endl;
        }
        else if (cmd == "PINDAH") {
            string sn;
            if (!(iss >> sn)) {
                cout << "DITOLAK: format salah, pakai PINDAH n" << endl;
                continue;
            }
            string sisa;
            if (iss >> sisa) {
                cout << "DITOLAK: argumen berlebih" << endl;
                continue;
            }
            if (!isInteger(sn)) {
                cout << "DITOLAK: n harus bilangan bulat" << endl;
                continue;
            }
            int n = stoi(sn);
            if (n <= 0) {
                cout << "DITOLAK: n harus positif" << endl;
                continue;
            }
            if (n > nbList(A)) {
                cout << "DITOLAK: A hanya berisi " << nbList(A) << " paket" << endl;
                continue;
            }
            // pindahin n node dari A ke B
            for (int i = 0; i < n; i++) {
                address p = dequeue(A);
                enqueue(B, p);
            }
            cout << "OK: " << n << " paket dipindah A -> B" << endl;
        }
        else if (cmd == "KIRIM") {
            string sn;
            if (!(iss >> sn)) {
                cout << "DITOLAK: format salah, pakai KIRIM n" << endl;
                continue;
            }
            string sisa;
            if (iss >> sisa) {
                cout << "DITOLAK: argumen berlebih" << endl;
                continue;
            }
            if (!isInteger(sn)) {
                cout << "DITOLAK: n harus bilangan bulat" << endl;
                continue;
            }
            int n = stoi(sn);
            if (n <= 0) {
                cout << "DITOLAK: n harus positif" << endl;
                continue;
            }
            if (n > nbList(B)) {
                cout << "DITOLAK: B hanya berisi " << nbList(B) << " paket" << endl;
                continue;
            }
            for (int i = 0; i < n; i++) {
                address p = dequeue(B);
                cout << "  TERKIRIM " << (i+1) << ": " << p->info.kode
                     << " " << p->info.berat << " gram" << endl;
                kirim.jumlah++;
                kirim.totalBerat += p->info.berat;
                dealokasi(p);
            }
            cout << "OK: " << n << " paket dikirim" << endl;
        }
        else if (cmd == "CARI") {
            string kode;
            if (!(iss >> kode)) {
                cout << "DITOLAK: format salah, pakai CARI kode" << endl;
                continue;
            }
            string sisa;
            if (iss >> sisa) {
                cout << "DITOLAK: argumen berlebih" << endl;
                continue;
            }
            address p = findElm(A, kode);
            if (p != nullptr) {
                cout << "DITEMUKAN: A, posisi " << posisiElemen(A, p)
                     << ", berat " << p->info.berat << " gram" << endl;
            } else {
                p = findElm(B, kode);
                if (p != nullptr) {
                    cout << "DITEMUKAN: B, posisi " << posisiElemen(B, p)
                         << ", berat " << p->info.berat << " gram" << endl;
                } else {
                    cout << "TIDAK DITEMUKAN" << endl;
                }
            }
        }
        else if (cmd == "LIHAT") {
            printInfo(A, "A");
            printInfo(B, "B");
        }
        else if (cmd == "REKAP") {
            cetakRekap(A, B, kirim);
        }
        else if (cmd == "BERSIHKAN") {
            delAll(A);
            delAll(B);
            cout << "OK: A dan B dikosongkan, statistik terkirim tetap "
                 << kirim.jumlah << " paket / " << kirim.totalBerat << " gram" << endl;
        }
        else {
            cout << "Perintah tidak dikenal: " << cmd << endl;
        }
    }
    return 0;
}