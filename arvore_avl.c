#include<stdio.h>
#include<stdlib.h>

typedef struct No
{
    int chave;
    struct No *esq;
    struct No *dir;
    int h;
}No;

typedef struct
{
    No* raiz;
}Arv;

int max(int a, int b);

int h(No *N)
{
    if (N == NULL)
        return 0;
    return N->h;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

No* Novo(int chave)
{
    No* no = (No*) malloc(sizeof(No));
    no->chave = chave;
    no->esq = NULL;
    no->dir = NULL;
    no->h = 1;
    return no;
}

No* rotacaoLL(No *pA)
{
    No *pB;

    pB = pA->esq;
    pA->esq = pB->dir;
    pB->dir = pA;

    pA->h = max(h(pA->esq), h(pA->dir))+1;
    pB->h = max(h(pB->esq), h(pB->dir))+1;

    return pB;
}

No* rotacaoRR(No *pA)
{
    No *pB;

    pB = pA->dir;
    pA->dir = pB->esq;
    pB->esq = pA;

    pA->h = max(h(pA->esq), h(pA->dir))+1;
    pB->h = max(h(pB->esq), h(pB->dir))+1;

    return pB;
}

No* rotacaoLR(No *pA)
{
    No *pB, *pC;

    pB = pA->esq;
    pC = pB->dir;
    pB->dir = pC->esq;
    pC->esq = pB;
    pA->esq = pC->dir;
    pC->dir = pA;
    
    pA->h = max(h(pA->esq), h(pA->dir))+1;
    pB->h = max(h(pB->esq), h(pB->dir))+1;
    pC->h = max(h(pC->esq), h(pC->dir))+1;

    return pC;
}

No* rotacaoRL(No *pA)
{
    No *pB, *pC;

    pB = pA->dir;
    pC = pB->esq;
    pB->esq = pC->dir;
    pC->dir = pB;
    pA->dir = pC->esq;
    pC->esq = pA;

    pA->h = max(h(pA->esq), h(pA->dir))+1;
    pB->h = max(h(pB->esq), h(pB->dir))+1;
    pC->h = max(h(pC->esq), h(pC->dir))+1;

    return pC;
}

int fb(No *N)
{
    if (N == NULL)
        return 0;
    return h(N->esq) - h(N->dir);
}
 
No* Inserir(No* no, int chave)
{
    if (no == NULL)
        return Novo(chave);
 
    if (chave < no->chave)
        no->esq = Inserir(no->esq, chave);
    else if (chave > no->chave)
        no->dir = Inserir(no->dir, chave);
    else
        return no;

    no->h = 1 + max(h(no->esq), h(no->dir));

    if (fb(no) > 1 && fb(no->esq) > 0)
        return rotacaoLL(no);

    if (fb(no) < -1 && fb(no->dir) < 0)
        return rotacaoRR(no);

    if (fb(no) > 1 && fb(no->esq) < 0)
        return rotacaoLR(no);

    if (fb(no) < -1 && fb(no->dir) > 0)
        return rotacaoRL(no);

    return no;
}

No *MaisEsq(No* no)
{ 
    if (no->esq != NULL)
        return MaisEsq(no->esq);
    return no;
}

No* Remover(No* no, int chave)
{
    if (no == NULL)
        return no;
 
    if (chave < no->chave)
        no->esq = Remover(no->esq, chave);
 
    else if( chave > no->chave )
        no->dir = Remover(no->dir, chave);
 
    else
    {
        if(no->esq == NULL || no->dir == NULL)
        {
            No *temp = no->esq ? no->esq : no->dir;
 
            if (temp == NULL)
            {
                temp = no;
                no = NULL;
            }
            else
                *no = *temp; // no->esq = temp->esq; no->dir = temp->dir; no->chave = temp->chave;
            free(temp);
        }
        else
        {
            No* temp = MaisEsq(no->dir);
 
            no->chave = temp->chave;
 
            no->dir = Remover(no->dir, temp->chave);
        }
    }
 
    if (no == NULL)
        return no;
 
    no->h = 1 + max(h(no->esq), h(no->dir));
 
    if (fb(no) > 1 && fb(no->esq) > 0)
        return rotacaoLL(no);
 
    if (fb(no) > 1 && fb(no->esq) < 0)
        return rotacaoLR(no);
 
    if (fb(no) < -1 && fb(no->dir) < 0)
        return rotacaoRR(no);
 
    if (fb(no) < -1 && fb(no->dir) > 0)
        return rotacaoRL(no);
 
    return no;
}

void PrintTree(No *no)
{ 	
    if (no != NULL) 
    { 	
        printf("(C%d", no->chave);
        PrintTree(no->esq); 
        PrintTree(no->dir);
        printf(")"); 	
    } 	
    else printf("()"); 
}

void InicializarArv(Arv *arvore)
{
    arvore->raiz = NULL;
}

No* Busca(No *no, int chave)
{
    if (no != NULL)
    {
        if (no->chave == chave) return no;
        else if (no->chave > chave) return Busca(no->dir, chave);
        else return Busca(no->esq, chave);
    }
    return no;
}

int main()
{
    Arv arvore;
    InicializarArv(&arvore);

    int i;
    int n, a;
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        scanf("%d", &a);
        arvore.raiz = Inserir(arvore.raiz, a);
    }
    
    scanf("%d", &a);

    if (Busca(arvore.raiz, a) == NULL)
        arvore.raiz = Inserir(arvore.raiz, a);
    else
        arvore.raiz = Remover(arvore.raiz, a);
 
    PrintTree(arvore.raiz);
 
    return 0;
}