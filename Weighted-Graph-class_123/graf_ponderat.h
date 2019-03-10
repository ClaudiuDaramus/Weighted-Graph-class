#ifndef GRAF_PONDERAT_H
#define GRAF_PONDERAT_H
#include <iostream>

using namespace std;

struct coada
{
    int val;
    coada *next;
};

class graf_ponderat{
    int n;
    int **matriceadiacenta;
    int **matricecosturi;
    int **listaadiacenta;
    int *grad;
    coada *prim, *ultim;
    int *viz;
public:
    graf_ponderat(char *numefisier);
    graf_ponderat();
    graf_ponderat(const graf_ponderat &g2);
    ~graf_ponderat();
    friend ostream& operator<<(ostream& a, graf_ponderat &g);
    friend graf_ponderat operator*(graf_ponderat g1, graf_ponderat g2);
    void operator=(graf_ponderat g);
    void BFS(int x);
    void DFS(int x);
    void matrice_drumuri_ponderi_minime();
    void drumul_pondere_minima();
    bool conex();
    void DFS1(int x);
};

#endif // GRAF_PONDERAT_H
