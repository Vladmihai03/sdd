
//Bilet #5 / (150 min) 11.06.2021


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    unsigned int cod_departament;
    char* nume;
    char* pozitie;
    int varsta;
    float salariu;
    int ani_experienta;
} angajat;

typedef struct nodLS {
    angajat inf;
    struct nodLS* next;
} nodLS;

typedef struct nodLP {
    nodLS* inf;
    struct nodLP* next;
} nodLP;

void inserareLS(nodLS** cap, angajat a) {
    nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
    nou->inf.cod_departament = a.cod_departament;
    nou->inf.nume = (char*)malloc((strlen(a.nume) + 1) * sizeof(char));
    strcpy(nou->inf.nume, a.nume);
    nou->inf.pozitie = (char*)malloc((strlen(a.pozitie) + 1) * sizeof(char));
    strcpy(nou->inf.pozitie, a.pozitie);
    nou->inf.varsta = a.varsta;
    nou->inf.salariu = a.salariu;
    nou->inf.ani_experienta = a.ani_experienta;
    nou->next = NULL;
    if (*cap == NULL) {
        *cap = nou;
    }
    else {
        nodLS* temp = *cap;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nou;
    }
}
void inserareLP(nodLP** capLP, angajat a) {
    nodLP* temp = *capLP;
    nodLP* prev = NULL;
    
    while (temp != NULL) {
        if (temp->inf != NULL && temp->inf->inf.cod_departament == a.cod_departament) {
            inserareLS(&(temp->inf), a);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
    nou->inf = NULL;
    nou->next = NULL;
    inserareLS(&(nou->inf), a);

    if (prev == NULL) {
        *capLP = nou;
    }
    else {
        prev->next = nou;
    }
}



void traversareLS(nodLS* capLS) {
    nodLS* temp = capLS;
    while (temp != NULL) {
        printf("Cod Departament: %u, Nume: %s, Pozitie: %s, Varsta: %d, Salariu: %.2f, Ani Experienta: %d\n",
            temp->inf.cod_departament, temp->inf.nume, temp->inf.pozitie,
            temp->inf.varsta, temp->inf.salariu, temp->inf.ani_experienta);
        temp = temp->next;
    }
}

void traversareLP(nodLP* capLP) {
    nodLP* temp = capLP;
    while (temp != NULL) {
        if (temp->inf != NULL) {
            printf("\nDepartament: %d\n", temp->inf->inf.cod_departament);
            traversareLS(temp->inf);
        }
        temp = temp->next;
    }
}

void dezalocareLS(nodLS* capLS) {
    nodLS* temp = capLS;
    while (temp != NULL) {
        nodLS* aux = temp->next;
        free(temp->inf.nume);
        free(temp->inf.pozitie);
        free(temp);
        temp = aux;
    }
}

void dezalocareLP(nodLP* capLP) {
    nodLP* temp = capLP;
    while (temp != NULL) {
        nodLP* aux = temp->next;
        dezalocareLS(temp->inf);
        free(temp);
        temp = aux;
        
    }
}

void nod2Vector(nodLS* cap, angajat* vect, int* nrElem, float salariu) {
    nodLS* temp = cap;
    while (temp != NULL) {
        if (temp->inf.salariu > salariu) {
            vect[*nrElem].cod_departament = temp->inf.cod_departament;
            vect[*nrElem].nume = (char*)malloc((strlen(temp->inf.nume) + 1) * sizeof(char));
            strcpy(vect[*nrElem].nume, temp->inf.nume);
            vect[*nrElem].pozitie = (char*)malloc((strlen(temp->inf.pozitie) + 1) * sizeof(char));
            strcpy(vect[*nrElem].pozitie, temp->inf.pozitie);
            vect[*nrElem].salariu = temp->inf.salariu;
            vect[*nrElem].varsta = temp->inf.varsta;
            vect[*nrElem].ani_experienta = temp->inf.ani_experienta;
            (*nrElem)++;
        }
        temp = temp->next;
    }
}

void traversare2Vector(nodLP* cap, angajat* vect, int* nrElem, float salariu) {
    nodLP* temp = cap;
    while (temp != NULL) {
        if (temp->inf != NULL) {
            nod2Vector(temp->inf, vect, nrElem, salariu);
        }
        temp = temp->next;
    }
}

void put(nodLS* capLS, nodLS** prim, nodLS** ultim) {
    nodLS* temp = capLS;
    while (temp != NULL) {

        nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
        nou->inf.cod_departament = temp->inf.cod_departament;
        nou->inf.nume = (char*)malloc((strlen(temp->inf.nume) + 1) * sizeof(char));
        strcpy(nou->inf.nume, temp->inf.nume);
        nou->inf.pozitie = (char*)malloc((strlen(temp->inf.pozitie) + 1) * sizeof(char));
        strcpy(nou->inf.pozitie, temp->inf.pozitie);
        nou->inf.salariu = temp->inf.salariu;
        nou->inf.varsta = temp->inf.varsta;
        nou->inf.ani_experienta = temp->inf.ani_experienta;
        nou->next = NULL;
        if (*prim == NULL && *ultim == NULL) {
            *prim = nou;
            *ultim = nou;
        }
        else {
            (*ultim)->next = nou;
            *ultim = nou;
        }


        temp = temp->next;
    }
}

void traversare2Stiva(nodLP* cap, nodLS** prim, nodLS** ultim, char domeniu[50]) {
    nodLP* temp = cap;
    while (temp != NULL) {
        if (temp->inf != NULL && temp->inf->inf.cod_departament == domeniu) {
            put(temp->inf, prim, ultim);
        }
        temp = temp->next;
    }
}








int main() {
    FILE* f = fopen("fisier.txt", "r");
    if (f == NULL) {
        printf("Nu s-a putut deschide fisierul.\n");
        return -1;
    }

    int n;
    fscanf(f, "%d", &n);
    angajat a;
    char buffer[50];
    nodLP* capLP = NULL;

    for (int i = 0; i < n; i++) {
        fscanf(f, "%u", &a.cod_departament);
        fscanf(f, "%s", buffer);
        a.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(a.nume, buffer);
        fscanf(f, "%s", buffer);
        a.pozitie = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(a.pozitie, buffer);
        fscanf(f, "%d", &a.varsta);
        fscanf(f, "%f", &a.salariu);
        fscanf(f, "%d", &a.ani_experienta);

        inserareLP(&capLP, a);

        free(a.nume);
        free(a.pozitie);
    }
    fclose(f);

    traversareLP(capLP);
    printf("\n\n");
    angajat* vect = (angajat*)malloc(n * sizeof(angajat));
    int nrElem = 0;
    traversare2Vector(capLP, vect, &nrElem, 5000);
    printf("Prin vector peste: 5000\n");
    for (int i = 0; i < nrElem; i++) {
        printf("Cod Departament: %u, Nume: %s, Pozitie: %s, Varsta: %d, Salariu: %.2f, Ani Experienta: %d\n",
            vect[i].cod_departament, vect[i].nume, vect[i].pozitie,
            vect[i].varsta, vect[i].salariu, vect[i].ani_experienta);
        free(vect[i].nume);
        free(vect[i].pozitie);
    }
    free(vect);

    printf("\nSI acum in Stiva: \n");
    nodLS* prim = NULL;
    nodLS* ultim = NULL;
    traversare2Stiva(capLP, &prim, &ultim, 2);
    traversareLS(prim);
    dezalocareLP(capLP);
    dezalocareLS(prim);

    return 0;
}
