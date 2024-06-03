
//Bilet #7 / (120 min) 14.06.2020

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
    int etaj;
    char* roomType;
    char* hotel;
    float price;
    int cod;
} camera;

typedef struct {
    camera inf;
    struct nodarb* right, * left;
} nodarb;

typedef struct {
    camera inf;
    struct nodLD* prev, * next;
} nodLD;

nodarb* creare(camera c, nodarb* dr, nodarb* st) {
    nodarb* nou = (nodarb*)malloc(sizeof(nodarb));
    nou->inf.etaj = c.etaj;
    nou->inf.roomType = (char*)malloc((strlen(c.roomType) + 1) * sizeof(char));
    strcpy(nou->inf.roomType, c.roomType);
    nou->inf.hotel = (char*)malloc((strlen(c.hotel) + 1) * sizeof(char));
    strcpy(nou->inf.hotel, c.hotel);
    nou->inf.price = c.price;
    nou->inf.cod = c.cod;
    nou->right = dr;
    nou->left = st;
    return nou;
}

nodarb* inserare(nodarb* root, camera c) {
    if (root == NULL) {
        return creare(c, NULL, NULL);
    }

    if (c.cod < root->inf.cod) {
        root->left = inserare(root->left, c);
    }
    else if (c.cod > root->inf.cod) {
        root->right = inserare(root->right, c);
    }
    else {
        printf("\n\nExista deja aceasta valoare");
    }
    return root;
}

void inordine(nodarb* root) {
    if (root != NULL) {
        inordine(root->left);
        printf("\nEtaj: %d, RoomType: %s, Hotel: %s, Price: %5.2f, Cod: %d", root->inf.etaj, root->inf.roomType, root->inf.hotel, root->inf.price, root->inf.cod);
        inordine(root->right);
    }
}

void dezalocare(nodarb* root) {
    if (root != NULL) {
        nodarb* st = root->left;
        nodarb* dr = root->right;
        free(root->inf.roomType);
        free(root->inf.hotel);
        free(root);
        dezalocare(st);
        dezalocare(dr);
    }
}

void inserareLD(nodLD** cap, nodLD** coada, camera c) {
    nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
    nou->inf.etaj = c.etaj;
    nou->inf.roomType = (char*)malloc((strlen(c.roomType) + 1) * sizeof(char));
    strcpy(nou->inf.roomType, c.roomType);
    nou->inf.hotel = (char*)malloc((strlen(c.hotel) + 1) * sizeof(char));
    strcpy(nou->inf.hotel, c.hotel);
    nou->inf.price = c.price;
    nou->inf.cod = c.cod;
    nou->next = NULL;
    nou->prev = NULL;
    if (*coada == NULL && *cap == NULL) {
        *coada = nou;
        *cap = nou;
    }
    else {
        nodLD* temp = *cap;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nou;
        nou->prev = temp;
        *coada = nou;
    }
}

void arbore2LD(nodarb* root, nodLD** cap, nodLD** coada, int cod) {
    if (root == NULL) return;

    arbore2LD(root->left, cap, coada, cod);
    if (root->inf.cod > cod) {
        inserareLD(cap, coada, root->inf);
    }
    arbore2LD(root->right, cap, coada, cod);
}

void afisareLD(nodLD* cap) {
    nodLD* temp = cap;
    while (temp != NULL) {
        printf("\nEtaj: %d, RoomType: %s, Hotel: %s, Price: %5.2f, Cod: %d", temp->inf.etaj, temp->inf.roomType, temp->inf.hotel, temp->inf.price, temp->inf.cod);
        temp = temp->next;
    }
}

void inserarePrioritate(nodLD** cap, nodLD** coada, camera c, int cod) {
    nodLD* current = *cap;
    while (current && current->inf.cod != cod) {
        current = current->next;
    }
    if (current != NULL) {
        nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
        nou->inf.etaj = c.etaj;
        nou->inf.roomType = (char*)malloc((strlen(c.roomType) + 1) * sizeof(char));
        strcpy(nou->inf.roomType, c.roomType);
        nou->inf.hotel = (char*)malloc((strlen(c.hotel) + 1) * sizeof(char));
        strcpy(nou->inf.hotel, c.hotel);
        nou->inf.price = c.price;
        nou->inf.cod = c.cod;

        if (current == *cap && current == *coada) {
            current->next = nou;
            nou->prev = current;
            nou->next = NULL;
            *coada = nou;
        }
        else if (current == *cap) {
            nodLD* urm = current->next;
            urm->prev = nou;
            nou->next = urm;
            current->next = nou;
            nou->prev = current;
        }
        else if (current == *coada) {
            nou->next = NULL;
            nou->prev = current;
            current->next = nou;
            *coada = nou;
        }
        else {
            nodLD* urm = current->next;
            current->next = nou;
            urm->prev = nou;
            nou->prev = current;
            nou->next = urm;
        }
    }
    else {
        printf("\n\nNu exista camera cu acest cod");
    }
}

void dezalocareLD(nodLD* cap) {
    nodLD* temp = cap;
    while (temp != NULL) {
        nodLD* aux = temp->next;
        free(temp->inf.roomType);
        free(temp->inf.hotel);
        free(temp);
        temp = aux;
    }
}

int main() {
    FILE* f = fopen("fisier.txt", "r");
    int nrCamere;
    fscanf(f, "%d", &nrCamere);
    nodarb* root = NULL;
    camera c;
    char buffer1[50], buffer2[50];
    nodLD* cap = NULL;
    nodLD* coada = NULL;

    for (int i = 0; i < nrCamere; i++) {
        fscanf(f, "%d", &c.etaj);
        fscanf(f, "%s", buffer1);
        c.roomType = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
        strcpy(c.roomType, buffer1);
        fscanf(f, "%s", buffer2);
        c.hotel = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
        strcpy(c.hotel, buffer2);
        fscanf(f, "%f", &c.price);
        fscanf(f, "%d", &c.cod);
        root = inserare(root, c);
        free(c.roomType);
        free(c.hotel);
    }
    fclose(f);

    inordine(root);
    printf("\n\n");
    arbore2LD(root, &cap, &coada, 1005);
    afisareLD(cap);

    camera g;
    g.etaj = 2;
    g.roomType = (char*)malloc((strlen("Single") + 1) * sizeof(char));
    strcpy(g.roomType, "Single");

    g.hotel = (char*)malloc((strlen("Marriot") + 1) * sizeof(char));
    strcpy(g.hotel, "Marriot");
    g.price = 120.23;
    g.cod = 1017;
    printf("\n\n");
    inserarePrioritate(&cap, &coada, g, 1009);
    afisareLD(cap);

    dezalocare(root);
    dezalocareLD(cap);

    return 0;
}
