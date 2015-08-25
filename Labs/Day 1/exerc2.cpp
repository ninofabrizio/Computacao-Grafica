#include <cstdio>
#include <iostream>
using namespace std;

int media (int *vet, int n) {

    int som = 0;

    for(int i = 0; i < n; i++)
        som += vet[i];

    return som/n;
}

int main () {

    int vet[10] = {0}, j = -1, med;
    bool achou = false;

    vet[0] = 1;

    for(int i = 1; i < 10; i++)
        vet[i] = vet[i - 1] + 1;

    med = media(vet, 10);

    for(int i = 0; i < 10; i++) {
        
        if(vet[i] == med) {
            
            j = i;
            achou = true;
            break;
        }
    }

    if(achou == false)
        cout<<"Nao ha valor no vetor que seja igual a media!"<<endl;
    else
        cout<<"Valor no vetor que equivale a media: "<<vet[j]<<endl;

    return 0;
}
