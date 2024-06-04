
//Tabela de dispersie – Dosar Candidat

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

// Define structures globally
typedef struct {
    char* numeCandidat;
    char* programStudiu;
    float medieBac;
    int codDosar;
} candidat;

typedef struct nodLS {
    candidat inf;
    struct nodLS* next;
} nodLS;

typedef struct {
    nodLS** vect;
    int size;
} hashT;

typedef struct nodLP {
    nodLS* inf;
    struct nodLP* next;
} nodLP;

int hashFunction(hashT tabela, char* nume) {
    int hashValue = 0;
    for (int i = 0; i < strlen(nume); i++) {
        hashValue += nume[i];
    }
    return hashValue % tabela.size;
}

void inserare(hashT tabela, candidat c) {
    if (tabela.vect != NULL) {
        int pozitie = hashFunction(tabela, c.numeCandidat);

        nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
        nou->inf.numeCandidat = (char*)malloc((strlen(c.numeCandidat) + 1) * sizeof(char));
        strcpy(nou->inf.numeCandidat, c.numeCandidat);
        nou->inf.programStudiu = (char*)malloc((strlen(c.programStudiu) + 1) * sizeof(char));
        strcpy(nou->inf.programStudiu, c.programStudiu);
        nou->inf.medieBac = c.medieBac;
        nou->inf.codDosar = c.codDosar;
        nou->next = NULL;
        if (tabela.vect[pozitie] == NULL) {
            tabela.vect[pozitie] = nou;
        }
        else {
            nodLS* temp = tabela.vect[pozitie];
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = nou;
        }
    }
}

void traversareLS(nodLS* cap) {
    nodLS* temp = cap;
    while (temp != NULL) {
        printf("\nNumeCandidat: %s, ProgramStudiu: %s, MedieBac: %5.2f, CodDosar: %d",
            temp->inf.numeCandidat, temp->inf.programStudiu, temp->inf.medieBac, temp->inf.codDosar);
        temp = temp->next;
    }
}

void traversare(hashT tabela) {
    if (tabela.vect == NULL) {
        return;
    }
    for (int i = 0; i < tabela.size; i++) {
        if (tabela.vect[i] != NULL) {
            printf("\n\nSublista %d:", i);
            traversareLS(tabela.vect[i]);
        }
    }
}

void countProgram(nodLS* cap, char* programStudiu, int* cate) {
    nodLS* temp = cap;
    while (temp != NULL) {
        if (strcmp(temp->inf.programStudiu, programStudiu) == 0) {
            (*cate)++;
        }
        temp = temp->next;
    }
}

void cautareProgram(hashT tabela, char* programStudiu, int* cate) {
    if (tabela.vect == NULL) {
        return;
    }

    for (int i = 0; i < tabela.size; i++) {
        if (tabela.vect[i] != NULL) {
            nodLS* temp = tabela.vect[i];
            countProgram(temp, programStudiu, cate);
        }
    }
}

void inserareLS(nodLS** cap, candidat c) {
    nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
    nou->inf.numeCandidat = (char*)malloc((strlen(c.numeCandidat) + 1) * sizeof(char));
    strcpy(nou->inf.numeCandidat, c.numeCandidat);
    nou->inf.programStudiu = (char*)malloc((strlen(c.programStudiu) + 1) * sizeof(char));
    strcpy(nou->inf.programStudiu, c.programStudiu);
    nou->inf.medieBac = c.medieBac;
    nou->inf.codDosar = c.codDosar;
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

void inserareLP(nodLP** cap, candidat c) {
    nodLP* temp = *cap;
    nodLP* prev = NULL;
    while (temp != NULL) {
        if (temp->inf != NULL && strcmp(temp->inf->inf.programStudiu, c.programStudiu) == 0) {
            inserareLS(&(temp->inf), c);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
    nou->inf = NULL;
    inserareLS(&(nou->inf), c);
    nou->next = NULL;
    if (prev == NULL) {
        *cap = nou;
    }
    else {
        prev->next = nou;
    }
}

void chaining2LL(hashT tabela, nodLP** cap, int medie) {
    if (tabela.vect == NULL) {
        return;
    }
    for (int i = 0; i < tabela.size; i++) {
        if (tabela.vect[i] != NULL) {
            nodLS* temp = tabela.vect[i];
            while (temp != NULL) {
                if (temp->inf.medieBac < medie) {
                    inserareLP(cap, temp->inf);
                }
                temp = temp->next;
            }
        }
    }
}

void traversareLP(nodLP* cap) {
    nodLP* temp = cap;
    while (temp != NULL) {
        printf("\nSublista:");
        traversareLS(temp->inf);
        temp = temp->next;
    }
}

int cautare(nodLP* cap) {
    nodLP* temp = cap;
    int min = 99999;
    while (temp != NULL) {
        nodLS* temp2 = temp->inf;
        while (temp2 != NULL) {
            if (temp2->inf.codDosar < min) {
                min = temp2->inf.codDosar;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return (min != 99999) ? min : 0;
}

void stergereLS(nodLS** cap, int cod) {
    nodLS* temp = *cap;
    nodLS* prev = NULL;
    while (temp != NULL) {
        if (temp->inf.codDosar == cod) {
            if (prev == NULL) {
                *cap = temp->next;
            }
            else {
                prev->next = temp->next;
            }
            free(temp->inf.numeCandidat);
            free(temp->inf.programStudiu);
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void parcurgereCautare(nodLP** cap, int cod) {
    nodLP* temp = *cap;
    while (temp != NULL) {
        stergereLS(&(temp->inf), cod);
        temp = temp->next;
    }
}

int main() {
    FILE* f = fopen("fisier.txt", "r");
    int nrStud;
    fscanf(f, "%d", &nrStud);
    hashT tabela;
    tabela.size = 101;
    tabela.vect = (nodLS**)malloc(tabela.size * sizeof(nodLS*));
    for (int i = 0; i < tabela.size; i++) {
        tabela.vect[i] = NULL;
    }

    char buffer[50], buffer2[50];
    candidat c;

    for (int i = 0; i < nrStud; i++) {
        fscanf(f, "%s", buffer);
        c.numeCandidat = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(c.numeCandidat, buffer);
        fscanf(f, "%s", buffer2);
        c.programStudiu = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
        strcpy(c.programStudiu, buffer2);
        fscanf(f, "%f", &c.medieBac);
        fscanf(f, "%d", &c.codDosar);
        inserare(tabela, c);
    }
    fclose(f);

    traversare(tabela);

    char* programStudiu = (char*)malloc((strlen("Istorie") + 1) * sizeof(char));
    strcpy(programStudiu, "Istorie");
    int cate = 0;
    cautareProgram(tabela, programStudiu, &cate);
    printf("\n\nPentru programul de studiu %s are %d candidati\n", programStudiu, cate);

    printf("\nSi acum liste de liste:\n");
    nodLP* cap = NULL;
    chaining2LL(tabela, &cap, 9);
    traversareLP(cap);
    int min = cautare(cap);
    parcurgereCautare(&cap, min);
    printf("\n");
    traversareLP(cap);

    return 0;
}
