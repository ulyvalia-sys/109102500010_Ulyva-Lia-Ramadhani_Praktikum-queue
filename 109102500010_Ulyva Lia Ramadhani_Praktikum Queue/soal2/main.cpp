#include "buffer.h"
#include <iostream>
#include <sstream>
#include <cctype>
using namespace std;

string toUpper(string s) {
    for (int i = 0; i < (int)s.length(); i++) s[i] = toupper(s[i]);
    return s;
}

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

int main() {
    Buffer B;
    createBuffer(B);

    cout << "=== BUFFER HASIL UKUR (kapasitas " << KAPASITAS << ") ===" << endl;
    cout << "Implementasi: " << namaVersi() << endl;
    cout << "Perintah: TULIS k v1..vk | AMBIL k | LIHAT | RINGKAS | KOSONGKAN | SELESAI" << endl;

    string baris;
    while (true) {
        cout << "> ";
        if (!getline(cin, baris)) break;

        istringstream iss(baris);
        string cmd;
        if (!(iss >> cmd)) continue;
        cmd = toUpper(cmd);

        if (cmd == "SELESAI") {
            cout << "Program selesai. Nomor terakhir=" << nomorBerikut(B) - 1
                 << ", total pergeseran=" << totalGeser(B) << endl;
            break;
        }
        else if (cmd == "TULIS") {
            string sk;
            if (!(iss >> sk)) {
                cout << "DITOLAK: format salah, pakai TULIS k v1..vk" << endl;
                continue;
            }
            if (!isInteger(sk)) {
                cout << "DITOLAK: k harus bilangan bulat" << endl;
                continue;
            }
            int k = stoi(sk);
            if (k < 1 || k > KAPASITAS) {
                cout << "DITOLAK: k harus 1-" << KAPASITAS << endl;
                continue;
            }

            int nilaiSementara[KAPASITAS];
            bool valid = true;
            for (int i = 0; i < k; i++) {
                string tok;
                if (!(iss >> tok)) {
                    cout << "DITOLAK: jumlah nilai kurang dari k=" << k << endl;
                    valid = false; break;
                }
                if (!isInteger(tok)) {
                    cout << "DITOLAK: nilai '" << tok << "' bukan integer" << endl;
                    valid = false; break;
                }
                int v = stoi(tok);
                if (v < NILAI_MIN || v > NILAI_MAX) {
                    cout << "DITOLAK: nilai '" << tok << "' di luar rentang -1000..1000" << endl;
                    valid = false; break;
                }
                nilaiSementara[i] = v;
            }

            string sisa;
            if (valid && (iss >> sisa)) {
                cout << "DITOLAK: jumlah nilai melebihi k" << endl;
                valid = false;
            }

            if (!valid) continue;

            if (ruangKosong(B) < k) {
                cout << "DITOLAK: ruang kosong hanya " << ruangKosong(B) << ", diminta " << k << endl;
                continue;
            }

            long long geserAwal = totalGeser(B);
            int nomorAwal = nomorBerikut(B);
            for (int i = 0; i < k; i++) enqueue(B, nilaiSementara[i]);
            cout << "OK: " << k << " rekaman masuk (nomor " << nomorAwal << ".."
                 << nomorBerikut(B) - 1 << "); pergeseran +"
                 << (totalGeser(B) - geserAwal)
                 << " (total " << totalGeser(B) << ")" << endl;
        }
        else if (cmd == "AMBIL") {
            string sk;
            if (!(iss >> sk)) {
                cout << "DITOLAK: format salah, pakai AMBIL k" << endl;
                continue;
            }
            if (!isInteger(sk)) {
                cout << "DITOLAK: k harus bilangan bulat" << endl;
                continue;
            }
            int k = stoi(sk);
            if (k < 1 || k > KAPASITAS) {
                cout << "DITOLAK: k harus 1-" << KAPASITAS << endl;
                continue;
            }
            if (k > nbBuffer(B)) {
                cout << "DITOLAK: data aktif " << nbBuffer(B) << ", diminta " << k << endl;
                continue;
            }

            long long geserAwal = totalGeser(B);
            for (int i = 0; i < k; i++) {
                Rekaman r;
                dequeue(B, r);
                cout << "  KELUAR " << (i+1) << ": " << r.nomor << ":" << r.nilai << endl;
            }
            cout << "OK: " << k << " rekaman diambil; pergeseran +"
                 << (totalGeser(B) - geserAwal)
                 << " (total " << totalGeser(B) << ")" << endl;
        }
        else if (cmd == "LIHAT") printInfo(B);
        else if (cmd == "RINGKAS") ringkas(B);
        else if (cmd == "KOSONGKAN") {
            kosongkan(B);
            cout << "OK: buffer dikosongkan; nomor berikutnya tetap "
                 << nomorBerikut(B) << "; total pergeseran tetap "
                 << totalGeser(B) << endl;
        }
        else cout << "Perintah tidak dikenal: " << cmd << endl;
    }
    return 0;
}