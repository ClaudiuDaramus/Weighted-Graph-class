#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

const int INFINIT = -1;

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

};

graf_ponderat::graf_ponderat()
{
   n = 0;
   viz = new int[n];
   for(int i=0;i<n;i++)
        viz[i]=0;
   matriceadiacenta = NULL;
   matricecosturi = NULL;
   listaadiacenta = NULL;
   prim=NULL;
   ultim=NULL;
}

graf_ponderat::graf_ponderat(char *numefisier)
{
    int m, x, y, z;
    ifstream f(numefisier);
    f>>n>>m;
    viz = new int[n];
    for(int i=0;i<n;i++)
        viz[i]=0;

    grad = new int[n];
    for(int i = 0; i < n; i++)
        grad[i] = 0;
    matriceadiacenta = new int*[n];
    matricecosturi = new int*[n];
    for(int i = 0;i < n; i++)
    {
        matriceadiacenta[i] = new int[n];
        matricecosturi[i] = new int[n];
        for(int j = 0;j < n; j++)
        {
            matriceadiacenta[i][j] = 0;
            if(i == j)
                matricecosturi[i][j] = 0;
            else
                matricecosturi[i][j] = INFINIT;
        }
    }
	listaadiacenta = new int*[n];
	for(int i = 0; i < n; i++)
        listaadiacenta[i] = NULL;


    for(int i = 1; i <= m; i++)
    {
        f>>x>>y>>z;
        grad[x-1]++;
        grad[y-1]++;

        matriceadiacenta[x-1][y-1] = 1;
        matriceadiacenta[y-1][x-1] = 1;

        matricecosturi[x-1][y-1] = z;
        matricecosturi[y-1][x-1] = z;


        if(listaadiacenta[x-1]==NULL)
        {
            listaadiacenta[x-1]=new int[1];
            listaadiacenta[x-1][0]=y;
        }
        else
        {
            int* aux = new int[grad[x-1]];
            for(int i=0;i<grad[x-1]-1;i++)
                aux[i]=listaadiacenta[x-1][i];
            aux[grad[x-1]-1]=y;
            delete listaadiacenta[x-1];
            listaadiacenta[x-1]=aux;
        }

        if(listaadiacenta[y-1]==NULL)
        {
            listaadiacenta[y-1]=new int[1];
            listaadiacenta[y-1][0]=x;
        }
        else
        {
            int* aux = new int[grad[y-1]];
            for(int i=0;i<grad[y-1]-1;i++)
                aux[i]=listaadiacenta[y-1][i];
            aux[grad[y-1]-1]=x;
            delete listaadiacenta[y-1];
            listaadiacenta[y-1]=aux;
        }
    }
    prim=NULL;
    ultim=NULL;
}

graf_ponderat::graf_ponderat(const graf_ponderat &g2)
{
    n=g2.n;
    grad = new int[n];
    for(int i = 0; i < n; i++)
        grad[i]=g2.grad[i];
    matriceadiacenta = new int*[n];
    matricecosturi = new int*[n];
    for(int i = 0;i < n; i++)
    {
        matriceadiacenta[i] = new int[n];
        matricecosturi[i] = new int[n];
        for(int j = 0;j < n; j++)
        {
            matriceadiacenta[i][j]=g2.matriceadiacenta[i][j];
            matricecosturi[i][j]=g2.matricecosturi[i][j];
        }
    }
    listaadiacenta = new int*[n];
	for(int i = 0; i < n; i++)
	{
	    listaadiacenta[i] = new int[grad[i]];
	    for(int j = 0; j < grad[i]; j++)
            listaadiacenta[i][j] = g2.listaadiacenta[i][j];
	}
    prim=g2.prim;
	ultim=g2.ultim;
}

graf_ponderat::~graf_ponderat()
{
      for(int i=0; i < n;i++)
      {
           delete listaadiacenta[i];
           delete matriceadiacenta[i];
           delete matricecosturi[i];
      }

      delete listaadiacenta;
      delete matriceadiacenta;
      delete matricecosturi;
      delete prim;
      delete ultim;
      delete viz;
  }

void graf_ponderat::operator=(graf_ponderat g)
{
    n=g.n;
    if(grad!=NULL)
        delete grad;
    if(matriceadiacenta!=NULL)
        delete matriceadiacenta;
    if(matricecosturi!=NULL)
        delete matricecosturi;
    if(listaadiacenta!=NULL)
        delete listaadiacenta;
    if(viz!=NULL)
        delete viz;

    grad = new int[n];
    for(int i = 0; i < n; i++)
        grad[i]=g.grad[i];
    matriceadiacenta = new int*[n];
    matricecosturi = new int*[n];
    for(int i = 0;i < n; i++)
    {
        matriceadiacenta[i] = new int[n];
        matricecosturi[i] = new int[n];
        for(int j = 0;j < n; j++)
        {
            matriceadiacenta[i][j]=g.matriceadiacenta[i][j];
            matricecosturi[i][j]=g.matricecosturi[i][j];
        }
    }
    listaadiacenta = new int*[n];
	for(int i = 0; i < n; i++)
	{
	    listaadiacenta[i] = new int[grad[i]];
	    for(int j = 0; j < grad[i]; j++)
            listaadiacenta[i][j] = g.listaadiacenta[i][j];
	}
    prim=g.prim;
	ultim=g.ultim;
}

ostream& operator<<(ostream& a, graf_ponderat &g)
{
      int alegere;
      do
      {
        cout<<"1. Matrice adiacenta\n\n";
        cout<<"2. Matrice costuri\n\n";
        cout<<"3. Lista adiacenta\n\n";
        cout<<"4. Iesire afisare";
        cout<<"______________________________________";
        cin>>alegere;

        switch(alegere)
        {
            case 1:
            {
                cout<<"1. Matrice adiacenta:\n\n";
                for(int i = 0; i < g.n; i++)
                {
                    for(int j = 0; j < g.n; j++)
                        cout<<g.matriceadiacenta[i][j]<<" ";
                    cout<<endl;
                }
                break;
            }

            case 2:
            {
                cout<<"2. Matrice costuri:\n\n";
                    for(int i = 0; i < g.n; i++)
                {
                    for(int j = 0; j < g.n; j++)
                        cout<<g.matricecosturi[i][j]<<" ";
                    cout<<endl;
                }
                break;
            }

            case 3:
            {
                cout<<"3. Lista adiacenta:\n\n";
                for(int i = 0; i < g.n; i++)
                {
                    cout<<i+1<<": ";
                    if(g.grad[i]==0)
                        cout<<"nod izolat";
                    for(int j = 0; j < g.grad[i]; j++)
                        cout<<g.listaadiacenta[i][j]<<" ";
                    cout<<endl;
                }
                break;
            }

            default:
                break;
        }
      }while(alegere!=4);
      return a;
}

graf_ponderat operator*(graf_ponderat g1, graf_ponderat g2)
{
    graf_ponderat g3;
    if(g1.n != g2.n)
    {
        cout<<"Numarul de noduri este diferit!";
        return g3;
    }
    if(g3.grad!=NULL)
        delete g3.grad;
    if(g3.matriceadiacenta!=NULL)
        delete g3.matriceadiacenta;
    if(g3.matricecosturi!=NULL)
        delete g3.matricecosturi;
    if(g3.listaadiacenta!=NULL)
        delete g3.listaadiacenta;

    g3.n = g1.n;

    g3.grad = new int[g3.n];
    for(int i = 0; i < g3.n; i++)
        g3.grad[i] = 0;
    g3.matriceadiacenta = new int*[g3.n];
    g3.matricecosturi = new int*[g3.n];
    for(int i = 0;i < g3.n; i++)
    {
        g3.matriceadiacenta[i] = new int[g3.n];
        g3.matricecosturi[i] = new int[g3.n];
        for(int j = 0;j < g3.n; j++)
        {
            g3.matriceadiacenta[i][j] = 0;
            if(i == j)
                g3.matricecosturi[i][j] = 0;
            else
                g3.matricecosturi[i][j] = INFINIT;
        }
    }
	/*g3.listaadiacenta = new int*[g3.n];
	for(int i = 0; i < g3.n; i++)
        g3.listaadiacenta[i] = NULL;

    for(int i = 0; i < g3.n; i++)
        for(int j = 0; j < g3.n; j++)
        {
            if((g1.matricecosturi[i][j] > 0) && (g2.matricecosturi[i][j] > 0))
            {
                if(g1.matricecosturi[i][j] < g2.matricecosturi[i][j])
                {
                    g3.matricecosturi[i][j] = g1.matricecosturi[i][j];
                    g3.matricecosturi[j][i] = g1.matricecosturi[i][j];
                }
                else
                {
                    g3.matricecosturi[i][j] = g2.matricecosturi[i][j];
                    g3.matricecosturi[j][i] = g2.matricecosturi[i][j];
                }

                g3.matriceadiacenta[i][j] = 1;
                g3.matriceadiacenta[j][i] = 1;

                if(g3.listaadiacenta[i-1]==NULL)
                {
                    g3.listaadiacenta[i-1]=new int[1];
                    g3.listaadiacenta[i-1][0]=j;
                }
                else
                {
                    int* aux = new int[g3.grad[i-1]];
                    for(int k=0;k<g3.grad[i-1]-1;k++)
                        aux[k]=g3.listaadiacenta[i-1][k];
                    aux[g3.grad[i-1]-1]=j;
                    delete g3.listaadiacenta[i-1];
                    g3.listaadiacenta[i-1]=aux;
                }

                if(g3.listaadiacenta[j-1]==NULL)
                {
                    g3.listaadiacenta[j-1]=new int[1];
                    g3.listaadiacenta[j-1][0]=i;
                }
                else
                {
                    int* aux = new int[g3.grad[j-1]];
                    for(int k=0;k<g3.grad[j-1]-1;k++)
                        aux[k]=g3.listaadiacenta[j-1][k];
                    aux[g3.grad[j-1]-1]=i;
                    delete g3.listaadiacenta[j-1];
                    g3.listaadiacenta[j-1]=aux;
                }
            }
        }*/
    return g3;
}

void graf_ponderat::BFS(int x)
{
    for(int i=0;i<n;i++)
        viz[i]=0;
    coada *p= new coada;
    p->val=x;
    p->next=NULL;
    prim=p;
    ultim=p;
    viz[x-1]=1;
    while(prim!=NULL)
    {
        cout<< prim->val <<" ";
        for(int j=0;j<grad[prim->val-1];j++)
        {
            if(viz[listaadiacenta[prim->val-1][j]-1]==0)
            {
            coada *q= new coada;
            q->val=listaadiacenta[prim->val-1][j];
            q->next=NULL;
            ultim->next=q;
            ultim=q;
            viz[listaadiacenta[prim->val-1][j]-1]=1;
            }
        }
        prim=prim->next;
    }
    delete p;
    for(int i=0;i<n;i++)
        viz[i]=0;
}

void graf_ponderat::DFS(int x)
{
    cout<<x<<" ";
    viz[x-1]=1;
    for(int i=0;i<grad[x-1];i++)
        if(viz[listaadiacenta[x-1][i]-1]==0)
            this->DFS(listaadiacenta[x-1][i]);
}

bool graf_ponderat::conex()
{
    for(int i=0;i<n;i++)
        viz[i]=0;
    coada *p= new coada;
    p->val=1;
    p->next=NULL;
    prim=p;
    ultim=p;
    viz[0]=1;
    while(prim!=NULL)
    {
        for(int j=0;j<grad[prim->val-1];j++)
        {
            if(viz[listaadiacenta[prim->val-1][j]-1]==0)
            {
            coada *q= new coada;
            q->val=listaadiacenta[prim->val-1][j];
            q->next=NULL;
            ultim->next=q;
            ultim=q;
            viz[listaadiacenta[prim->val-1][j]-1]=1;
            }
        }
        prim=prim->next;
    }
    delete p;


    for(int i=0;i<n;i++)
        if(viz[i]==0)
            return 0;
    return 1;
}

int main()
{
    graf_ponderat g;
    int alegere;
      do
      {
        cout<<"1. Citire\n\n";
        cout<<"2. Afisare\n\n";
        cout<<"3. BFS\n\n";
        cout<<"4. DFS\n\n";
        cout<<"5. Conexitate\n\n";
        cout<<"6. Iesire program";
        cout<<"______________________________________";
        cin>>alegere;

        switch(alegere)
        {
            case 1:
            {
                graf_ponderat g1("graf1.in");
                g=g1;
                /*char alegere2[2];
                do
                {char fisier[30];
                strcpy(fisier, "graf");
                cout<<"1. Alegerea fisierul din care se citeste(de la 1 la 5)\n\n";
                cout<<"2. Revenire meniu principal";
                cin>>alegere2;
                strcat(fisier, alegere2);
                strcat(fisier,".in");
                graf_ponderat g1(fisier);
                g=g1;
                }while(strcmp(alegere2,"2")==0);*/
                break;
            }

            case 2:
                {
                    cout<<g;
                    break;
                }
            case 3:
            {
                cout<<"3. BFS\n\n";
                g.BFS(1);
                break;
            }

            case 4:
            {
                cout<<"4. DFS\n\n";
                g.DFS(1);
                break;
            }

            case 5:
            {
                cout<<"5. Conexitate\n\n";
                if(g.conex())
                    cout<<"da"<<endl;
                else
                    cout<<"nu"<<endl;
                break;
            }

            default:
                break;
        }
      }while(alegere!=6);
    return 0;
}
