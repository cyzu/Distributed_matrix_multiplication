#include <stdlib.h>
#include <stdio.h>

struct Matrice{
 int ligne, colonne;
 long long int **matrice;
};


 /************************************************/
 /************************************************/
 /*************** FONCTIONS UTILES ***************/
 /************************************************/
 /************************************************/

 void allocation_matrice(struct Matrice * m){
 	int i;
 	m->matrice = malloc(sizeof(long long int*) * m->ligne);
 	for(i = 0; i < m->ligne; i++){
 		m->matrice[i] = malloc(sizeof(long long int) * m->colonne);
 	}
 }

 void afficher_matrice(struct Matrice *m){
     int i, j;
     for(i = 0; i < m->ligne; i++){
         for(j = 0; j < m->colonne; j++){
             if (j == 0){
                 printf("%lld", m->matrice[i][j]);
             }else {
                 printf(" %lld", m->matrice[i][j]);
             }
         }
         printf("\n");
     }
 }

 void produit_matriciel(struct Matrice * c, struct Matrice * a, struct Matrice * b){
     int i, j, k;
     for(i = 0; i < a->colonne; i++){
         for(j = 0; j < b->ligne; j++){
             for(k = 0; k < b->ligne; k++){
                 c->matrice[i][j] = c->matrice[i][j] + a->matrice[i][k] * b->matrice[k][j];
             }
         }
     }
 }


 /************************************************/
 /************************************************/
 /************ GENERATEURS DE MATRICE ************/
 /************************************************/
 /************************************************/

 void generation_matrice_zero(struct Matrice * m, int size){
 	int i, j;

  m->ligne = size;
 	m->colonne = size;

 	allocation_matrice(m);

 	for(i = 0; i < m->ligne; i++){
 		for(j = 0; j < m->colonne; j++){
 			m->matrice[i][j] = 0; //(i+1) * (j+1);
 		}
 	}
 }

 void generation_matrice(struct Matrice * m, int size){
 	int i, j;
  long long int valeur = 1;

  m->ligne = size;
  m->colonne = size;
 	allocation_matrice(m);

 	for(i = 0; i < m->ligne; i++){
 		for(j = 0; j < m->colonne; j++){
             m->matrice[i][j] = valeur;//i+j; //(i+1) * (j+1);
             valeur++;
 		}
 	}
 }

 void generation_matrice_incr(struct Matrice * m, int size){
 	int i, j;
  long long int valeur = 1;

 	m->ligne = size;
 	m->colonne = size;
  allocation_matrice(m);

 	for(i = 0; i < m->ligne; i++){
    for(j = 0; j < m->colonne; j++){
        m->matrice[i][j] = valeur * (valeur+1);
        valeur++;
    }
 	}
 }

 void generation_matrice_fixe(struct Matrice * m, int l, int c){
     int i, j;

     m->ligne = l;
     m->colonne = c;
     allocation_matrice(m);

     for(i = 0; i < m->ligne; i++){
         for(j = 0; j < m->colonne; j++){
             m->matrice[i][j] = 0;
         }
     }
 }

void generation_matrice_from_file(struct Matrice * m, const char * filename, const int size){
    FILE *file = fopen(filename, "r");
    int i, j;
    long long int valeur;
    
    if (file == NULL) {
        fprintf(stderr, "Erreur : l'ouverture du fichier '%s' a échoué !\n", filename);
        exit(1);
    }
    /* Remettre le curseur au début du fichier */
    rewind(file);
    
    m->ligne = size;
    m->colonne = size;
    
    allocation_matrice(m);
    
    /* Insersion des valeurs */
    for(i = 0; i < m->ligne; i++){
        for (j = 0; j < m->colonne; j++) {
            fscanf(file, "%lld", &valeur);
            m->matrice[i][j] = valeur;
        }
    }
    fclose(file);
}

int get_matrice_size(char * filename){
    FILE *file = fopen(filename, "r");
    int size = 0;
    long long int valeur = 0;
    
    if (file == NULL) {
        fprintf(stderr, "Erreur : l'ouverture du fichier '%s' a échoué !\n", filename);
        exit(1);
    }
    /* Compteur du nombre d'entier pour connaitre la taille de la matrice à allouer */
    while(valeur != '\n'){
        if (valeur == ' '){
            size++;
        }
        valeur = fgetc(file);
    }
    size++; // compte la dernière valeurs
    fclose(file);
    if (size <= 0){
        fprintf(stderr, "Erreur : il n'y a psa de valeur dans le fichier %s !\nFin du programme...", filename);
        exit(1);
    }
    return size;
}



int main (int argc, char *argv[]){
    if (argc - 1 < 2){
        fprintf(stderr, "Usage : %s <filename1> <filename2>\n ", argv[0]);
        exit(0);
    }
    int size = get_matrice_size(argv[1]);
    
    struct Matrice *A = malloc(sizeof(struct Matrice));
    struct Matrice *B = malloc(sizeof(struct Matrice));
    
    generation_matrice_from_file(A, argv[1], size);
    generation_matrice_from_file(B, argv[2], size);
    
    //afficher_matrice(A);
    //afficher_matrice(B);

    struct Matrice *C = malloc(sizeof(struct Matrice));
    generation_matrice_fixe(C, A->ligne, B->colonne);
    produit_matriciel(C, A, B);
    afficher_matrice(C);

    return 0;
}
