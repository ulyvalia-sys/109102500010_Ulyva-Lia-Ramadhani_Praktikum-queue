#ifndef QUEUE_H
#define QUEUE_H

#include <string>

// Tipe data untuk paket
struct Paket {
    std::string kode;
    int berat;
};

typedef Paket infotype;

// Node untuk linked list
struct Node {
    infotype info;
    Node* next;
};

typedef Node* address;

// Queue dengan head dan tail
struct Queue {
    address head;
    address tail;
};

// Statistik untuk paket terkirim
struct Statistik {
    long long jumlah;
    long long totalBerat;
};

// Prototipe fungsi ADT
void createQueue(Queue &Q);
bool isEmptyQueue(const Queue &Q);
int nbList(const Queue &Q);
long long totalBerat(const Queue &Q);

address alokasi(infotype P);
void dealokasi(address &P);

void enqueue(Queue &Q, address P);
address dequeue(Queue &Q);

address findElm(const Queue &Q, std::string kode);
bool fFindElm(const Queue &Q, std::string kode);

void printInfo(const Queue &Q, std::string nama);
void delAll(Queue &Q);

#endif