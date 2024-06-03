
//Bilet #13 / (120 min) 15.06.2020

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int cod;
    char* denumire;
    char* marca;
    float price;
    int cate;
} depozit;

typedef struct nodarb {
    depozit inf;
    struct nodarb* right, * left;
} nodarb;

nodarb* creare(depozit d, nodarb* st, nodarb* dr) {
    nodarb* nou = (nodarb*)malloc(sizeof(nodarb));
    nou->inf.cod = d.cod;
    nou->inf.denumire = (char*)malloc((strlen(d.denumire) + 1) * sizeof(char));
    strcpy(nou->inf.denumire, d.denumire);
    nou->inf.marca = (char*)malloc((strlen(d.marca) + 1) * sizeof(char));
    strcpy(nou->inf.marca, d.marca);
    nou->inf.price = d.price;
    nou->inf.cate = d.cate;
    nou->left = st;
    nou->right = dr;
    return nou;
}

nodarb* inserare(nodarb* root, depozit d) {
    if (root == NULL) {
        return creare(d, NULL, NULL);
    }
    if (d.cod > root->inf.cod) {
        root->right = inserare(root->right, d);
    }
    else if (d.cod < root->inf.cod) {
        root->left = inserare(root->left, d);
    }
    else {
        printf("\nAvem deja inregistrare cu acest cod\n");
    }
    return root;
}

void inordine(nodarb* root) {
    if (root != NULL) {
        inordine(root->left);
        printf("\nDenumire: %s, Marca: %s, Pret: %5.2f, NumarExemplare: %d, Cod: %d", root->inf.denumire, root->inf.marca, root->inf.price, root->inf.cate, root->inf.cod);
        inordine(root->right);
    }
}

void printGivenLevel(nodarb* root, int level) {
    if (root == NULL) return;
    if (level == 1) {
        printf("\nDenumire: %s, Marca: %s, Pret: %5.2f, NumarExemplare: %d, Cod: %d", root->inf.denumire, root->inf.marca, root->inf.price, root->inf.cate, root->inf.cod);
    }
    else {
        printGivenLevel(root->left, level - 1);
        printGivenLevel(root->right, level - 1);
    }
}

nodarb* cautare(nodarb* root, int cod) {
    if (root != NULL) {
        if (root->inf.cod == cod) {
            return root;
        }
        else if (root->inf.cod < cod) {
            return cautare(root->right, cod);
        }
        else {
            return cautare(root->left, cod);
        }
    }
    else {
        return NULL;
    }
}

void arbore2Vect(nodarb* root, depozit* vect, int* nrElem) {
    if (root == NULL) return;

    vect[*nrElem].cod = root->inf.cod;
    vect[*nrElem].denumire = (char*)malloc((strlen(root->inf.denumire) + 1) * sizeof(char));
    strcpy(vect[*nrElem].denumire, root->inf.denumire);
    vect[*nrElem].marca = (char*)malloc((strlen(root->inf.marca) + 1) * sizeof(char));
    strcpy(vect[*nrElem].marca, root->inf.marca);
    vect[*nrElem].price = root->inf.price;
    vect[*nrElem].cate = root->inf.cate;
    (*nrElem)++;

    if (root->left != NULL) {
        arbore2Vect(root->left, vect, nrElem);
    }
    else if (root->right != NULL) {
        arbore2Vect(root->right, vect, nrElem);
    }
}
int maxim(int a, int b) {
    return(a > b) ? a : b;
}

int nrNiveluri(nodarb* root) {
    if (root == NULL) {
        return 0;
    }
    int st = nrNiveluri(root->left);
    int dr = nrNiveluri(root->right);
    return 1 + maxim(st, dr);
}

int countNodesLevel(nodarb* root, int level) {
    if (root == NULL) return 0;
    if (level == 1) {
        return 1;
    }
    else {
        int st = countNodesLevel(root->left, level - 1);
        int dr = countNodesLevel(root->right, level - 1);
        return st + dr;
    }
}

void maxLevelNodes(nodarb* root, int height, int* max, int* level) {
    for (int i = 1; i <= height; i++) {
        int counts = countNodesLevel(root, i);
        if (counts > *max) {
            *max = counts;
            *level = i;
        }
    }
}


nodarb* removeLeafs(nodarb* root) {

    if (root == NULL) {
        return NULL;
    }
    
    if (root->left == NULL && root->right == NULL) {
        free(root->inf.denumire);
        free(root->inf.marca);
        free(root);
        return NULL;
    }
   
    if (root->left != NULL) {
        root->left = removeLeafs(root->left);
    }
    if (root->right != NULL) {
        root->right = removeLeafs(root->right);
    }
    return root;
}


int main() {
    FILE* f = fopen("fisier.txt", "r");


    int nrStud;
    fscanf(f, "%d", &nrStud);

    nodarb* root = NULL;
    depozit d;
    char buffer1[50], buffer2[50];

    for (int i = 0; i < nrStud; i++) {
        fscanf(f, "%d", &d.cod);
        fscanf(f, "%s", buffer1);
        d.denumire = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
        strcpy(d.denumire, buffer1);
        fscanf(f, "%s", buffer2);
        d.marca = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
        strcpy(d.marca, buffer2);
        fscanf(f, "%f", &d.price);
        fscanf(f, "%d", &d.cate);
        root = inserare(root, d);
        free(d.denumire); // Free after inserare since inserare makes a copy
        free(d.marca); // Free after inserare since inserare makes a copy
    }

    fclose(f);
    inordine(root);
    printf("\n");
    printGivenLevel(root, 3);
    printf("\n");

    nodarb* cautat = cautare(root, 8);
    if (cautat != NULL) {
        printf("\nDenumire: %s, Marca: %s, Pret: %5.2f, NumarExemplare: %d, Cod: %d", cautat->inf.denumire, cautat->inf.marca, cautat->inf.price, cautat->inf.cate, cautat->inf.cod);
        printf("\n");

        depozit* vect = (depozit*)malloc(nrStud * sizeof(depozit));
        int nrElem = 0;
        arbore2Vect(cautat, vect, &nrElem);

        printf("\nPath from node to root:\n");
        for (int i = 0; i < nrElem; i++) {
            printf("Cod: %d, Denumire: %s, Marca: %s, Pret: %.2f, NumarExemplare: %d\n", vect[i].cod, vect[i].denumire, vect[i].marca, vect[i].price, vect[i].cate);
            free(vect[i].denumire);
            free(vect[i].marca);
        }
        free(vect);
    }
    else {
        printf("\nNode with cod 3 not found.\n");
    }
    printf("\n");
    int niveluri = nrNiveluri(root);
    printf("\nNumar niveluri: %d", niveluri);
    printf("\n");
    int maxim = 0;
    int level = 0;
    maxLevelNodes(root, niveluri, &maxim, &level);
    printf("\n\nCele mai multe noduri se regasesc la nivelul %d si sutn exact %d\n", level, maxim);
    removeLeafs(root);
    inordine(root);
    printf("\n");

    return 0;
}
