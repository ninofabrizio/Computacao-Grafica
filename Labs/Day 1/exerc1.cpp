#include <cstdio>

void swap (int &val1, int &val2) {

    int temp = val1;
    val1 = val2;
    val2 = temp;
}

int main () {

    int val1, val2;

    printf("Diga valor 1: ");
    scanf("%d", &val1);

    printf("Diga valor 2: ");
    scanf("%d", &val2);

    swap(val1, val2);

    printf("\nTrocas:\nValor 1 = %d\nValor 2 = %d\n\n", val1, val2);

    return 0;
}
