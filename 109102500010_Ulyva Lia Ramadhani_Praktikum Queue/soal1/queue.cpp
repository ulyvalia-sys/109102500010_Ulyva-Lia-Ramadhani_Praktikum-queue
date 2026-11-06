#include "queue.h"
#include <iostream>
using namespace std;

void createQueue(Queue &Q) {
    Q.head = nullptr;
    Q.tail = nullptr;
}

bool isEmptyQueue(const Queue &Q) {
    return Q.head == nullptr;
}

int nbList(const Queue &Q) {
    int n = 0;
    address p = Q.head;
    while (p != nullptr) {
        n++;
        p = p->next;
    }
    return n;
}

long long totalBerat(const Queue &Q) {
    long long total = 0;
    address p = Q.head;
    while (p != nullptr) {
        total += p->info.berat;
        p = p->next;
    }
    return total;
}

address alokasi(infotype P) {
    address pBaru = new Node;
    if (pBaru == nullptr) return nullptr;
    pBaru->info = P;
    pBaru->next = nullptr;
    return pBaru;
}

void dealokasi(address &P) {
    delete P;
    P = nullptr;
}

void enqueue(Queue &Q, address P) {
    if (isEmptyQueue(Q)) {
        Q.head = P;
        Q.tail = P;
    } else {
        Q.tail->next = P;
        Q.tail = P;
    }
}

address dequeue(Queue &Q) {
    if (isEmptyQueue(Q)) return nullptr;

    address p = Q.head;
    Q.head = Q.head->next;

    if (Q.head == nullptr) {
        Q.tail = nullptr;
    }
    p->next = nullptr;
    return p;
}

address findElm(const Queue &Q, string kode) {
    address p = Q.head;
    while (p != nullptr) {
        if (p->info.kode == kode) return p;
        p = p->next;
    }
    return nullptr;
}

bool fFindElm(const Queue &Q, string kode) {
    return findElm(Q, kode) != nullptr;
}

void printInfo(const Queue &Q, string nama) {
    cout << "  " << nama << " (depan->belakang): ";
    if (isEmptyQueue(Q)) {
        cout << "KOSONG" << endl;
        return;
    }
    address p = Q.head;
    while (p != nullptr) {
        cout << p->info.kode << "(" << p->info.berat << "g)";
        if (p->next != nullptr) cout << " -> ";
        p = p->next;
    }
    cout << endl;
}

void delAll(Queue &Q) {
    address p;
    while (!isEmptyQueue(Q)) {
        p = dequeue(Q);
        dealokasi(p);
    }
}