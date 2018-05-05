#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main (){
    int i, j;
    srand(time(NULL));
    
    for (i = 0; i < 5400; i++) {
        for (j = 0; j < 5400; j++) {
           if (j == 0) printf("%d", (rand()%444)*54000);
            else printf(" %d", (rand()%444)*54000);
        }
        printf("\n");
    }
    return 0;
}
