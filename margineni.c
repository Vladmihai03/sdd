
//Bilet #4 / (120 min) 06.06.2020 

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct {
	int pret;
	char* nume;
	char* tip;	
	float price;
	int spatiu;
}platforma;

typedef struct {
	platforma inf;
	struct nodarb* right, * left;
}nodarb;

typedef struct {
	platforma* vect;
	int nrElem;
}heap;


void filtrare(heap h, int index) {
	
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;

	if (indexSt < h.nrElem && h.vect[indexSt].pret < h.vect[indexRad].pret) {
		indexRad = indexSt;
	}
	if (indexDr < h.nrElem && h.vect[indexDr].pret < h.vect[indexRad].pret) {
		indexRad = indexDr;
	}
	if (indexRad != index) {
		platforma temp = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = temp;

		filtrare(h, indexRad);
	}
}



void traversare(heap h) {
	for (int i = 0; i < h.nrElem; i++) {
		printf("\nPret: %d, Nume: %s, Tip: %s, Price: %5.2f, Spatiu: %d", h.vect[i].pret, h.vect[i].nume, h.vect[i].tip, h.vect[i].price, h.vect[i].spatiu);
	}
}

heap inserare(heap h, platforma pf) {
	
	platforma* vectNou = (platforma*)malloc((h.nrElem + 1) * sizeof(platforma));
	for (int i = 0; i < h.nrElem; i++) {
		vectNou[i] = h.vect[i];
	}
	h.nrElem++;
	vectNou[h.nrElem - 1] = pf;
	free(h.vect);
	h.vect = vectNou;
	for (int i = (h.nrElem-1)/2; i>=0; i--) {
		filtrare(h, i);
	}
	return h;
}

heap extragere(heap h, platforma* pf) {
	platforma* vectNou = (platforma*)malloc((h.nrElem - 1) * sizeof(platforma));
	platforma temp = h.vect[0];
	h.vect[0] = h.vect[h.nrElem - 1];
	h.vect[h.nrElem - 1] = temp;

	*pf = h.vect[h.nrElem - 1];
	h.nrElem--;
	for (int i = 0; i < h.nrElem; i++) {
		vectNou[i] = h.vect[i];
	}
	free(h.vect);
	h.vect = vectNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}
	return h;
}

nodarb* creare(platforma pf, nodarb* st, nodarb* dr) {
	nodarb* nou = (nodarb*)malloc(sizeof(nodarb));
	nou->inf.pret = pf.pret;
	nou->inf.nume = (char*)malloc((strlen(pf.nume) + 1) * sizeof(char));
	strcpy(nou->inf.nume, pf.nume);
	nou->inf.tip = (char*)malloc((strlen(pf.tip) + 1) * sizeof(char));
	strcpy(nou->inf.tip, pf.tip);
	nou->inf.price = pf.price;
	nou->inf.spatiu = pf.spatiu;
	nou->right = dr;
	nou->left = st;
	return nou;
}

nodarb* inserareArb(nodarb* root, platforma pf) {
	if (root == NULL) {
		return creare(pf, NULL, NULL);
	}
	if (pf.spatiu < root->inf.spatiu) {
		root->left = inserareArb(root->left, pf);
	}
	else if(pf.spatiu > root->inf.spatiu) {
		root->right = inserareArb(root->right, pf);
	}
	else {
		printf("\n\nAvem aceasta valoare deja: %d", pf.spatiu);
	}
	return root;
}

void heap2Arb(heap h, nodarb** root, int n) {
	if (n > h.nrElem) {
		return -1;
	}
	else {
		for (int i = 0; i < n; i++) {
			*root = inserareArb(*root, h.vect[i]);
		}
	}
}

void traversareArb(nodarb* root) {
	if (root) {
		traversareArb(root->left);
		printf("\nPret: %d, Nume: %s, Tip: %s, Price: %5.2f, Spatiu: %d", root->inf.pret, root->inf.nume, root->inf.tip, root->inf.price, root->inf.spatiu);
		traversareArb(root->right);
	}
}

void eliberareArb(nodarb* root) {
	if (root) {
		eliberareArb(root->left);
		eliberareArb(root->right);
		free(root->inf.nume);
		free(root->inf.tip);
		free(root);
	}
}

void afisareNoduriUnSingurDescendent(nodarb* root) {
	if (root) {
		if ((root->left == NULL && root->right != NULL) || (root->left != NULL && root->right == NULL)) {
			printf("\nNod cu un singur descendent - Pret: %d, Nume: %s, Tip: %s, Price: %5.2f, Spatiu: %d", root->inf.pret, root->inf.nume, root->inf.tip, root->inf.price, root->inf.spatiu);
		}
		afisareNoduriUnSingurDescendent(root->left);
		afisareNoduriUnSingurDescendent(root->right);
	}
}


int main() {
	heap h;
	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &h.nrElem);
	h.vect = (platforma*)malloc(h.nrElem * sizeof(platforma));
	platforma p;
	char buffer1[50], buffer2[50];
	for (int i = 0; i < h.nrElem; i++) {
		fscanf(f, "%d", &h.vect[i].pret);
		fscanf(f, "%s", buffer1);
		h.vect[i].nume = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
		strcpy(h.vect[i].nume, buffer1);
		fscanf(f, "%s", buffer2);
		h.vect[i].tip = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
		strcpy(h.vect[i].tip, buffer2);
		fscanf(f, "%f", &h.vect[i].price);
		fscanf(f, "%d", &h.vect[i].spatiu);
	}

	for (int i = (h.nrElem - 1) / 2; i >= 0; i--) {
		filtrare(h, i);
	}
	traversare(h);
	printf("\n");
	platforma pf;
	pf.pret = 109;
	pf.nume = (char*)malloc((strlen("xbox monster") + 1) * sizeof(char));
	strcpy(pf.nume, "xbox monster");
	pf.tip = (char*)malloc((strlen("ultimate") + 1) * sizeof(char));
	strcpy(pf.tip, "utlimate");
	pf.price = 500;
	pf.spatiu = 200;
	h = inserare(h, pf);
	traversare(h);
	printf("\n");
	h = extragere(h, &pf);
	pf.pret = 960;
	printf("\n");
	h = inserare(h, pf);
	traversare(h);

	nodarb* root = NULL;
	heap2Arb(h, &root, 5); // Copiază primele 5 elemente din heap în arbore
	traversareArb(root);
	printf("\n");

	printf("\nPlatformele aflate în nodurile care au un singur nod descendent:\n");
	afisareNoduriUnSingurDescendent(root);
	printf("\n");

	// Eliberare memorie heap
	for (int i = 0; i < h.nrElem; i++) {
		free(h.vect[i].nume);
		free(h.vect[i].tip);
	}
	free(h.vect);

	// Eliberare memorie arbore
	eliberareArb(root);

	return 0;
}