#include <stdio.h>
#include <stdlib.h>

struct header {
    int tam;
    struct header *prox;
};
struct header *buraco = NULL;
struct header *nxt = NULL;
struct header *atual = NULL;

void inicia(int tam_mem) {

    buraco = malloc(tam_mem);
    buraco->tam = tam_mem - sizeof (struct header);
    buraco->prox = NULL;
    nxt = buraco;
}

void* firstfit(int tam) {
    struct header *atual = buraco;
    struct header *anterior = NULL;
    int tam_total = tam + sizeof (struct header);
    while (atual != NULL) {
        if (atual->tam >= tam_total) {
            //printf("atual->tam: %d\n", atual->tam);
            //printf("tam: %d\n", tam);
            void *aux = (void *) atual + sizeof (struct header)+tam;
            anterior = atual;
            atual = aux;

            int t = anterior->tam - tam - (sizeof (struct header));
            atual->tam = t;
            atual->prox = anterior->prox;

            anterior->prox = atual;
            anterior->tam = 0 - tam;

            return (void *) anterior + sizeof (struct header);
        } else {
            if (atual->tam > tam) {
                int t = atual->tam * -1;
                atual->tam = t;
                return (void *) atual + sizeof (struct header);
            }
        }
        atual = atual->prox;
    }
    printf("Memória indisponível\n");
    return NULL;
}

void* bestfit(int tam) {
    struct header *melhor = NULL;
    struct header *anteriorMelhor = NULL;

    struct header *atual = buraco;
    struct header *anterior = NULL;

    int tam_total = tam + sizeof (struct header);
    while (atual != NULL) {
        if (melhor == NULL && atual->tam > 0) {
            anteriorMelhor = anterior;
            melhor = atual;
        } else {
            if (((atual->tam >= tam_total) || (atual->tam >= tam)) && (atual->tam < melhor->tam)) {
                anteriorMelhor = anterior;
                melhor = atual;
            }
        }
        anterior = atual;
        atual = atual->prox;
    }
    if (melhor->tam >= tam_total) {
        void *aux = (void *) melhor + sizeof (struct header)+tam;
        anteriorMelhor = melhor;
        melhor = aux;

        int t = anteriorMelhor->tam - tam - (sizeof (struct header));
        melhor->tam = t;
        melhor->prox = anteriorMelhor->prox;

        anteriorMelhor->prox = melhor;
        anteriorMelhor->tam = 0 - tam;

        return (void *) anteriorMelhor + sizeof (struct header);
    } else {
        if (melhor->tam > tam) {
            int t = melhor->tam * -1;
            melhor->tam = t;
            return (void *) melhor + sizeof (struct header);
        }
    }
    printf("Memória indisponível\n");
    return NULL;
}

void* worstfit(int tam) {
    struct header *pior = NULL;
    struct header *anteriorMelhor = NULL;

    struct header *atual = buraco;
    struct header *anterior = NULL;

    int tam_total = tam + sizeof (struct header);
    while (atual != NULL) {
        if (pior == NULL && atual->tam > 0) {
            anteriorMelhor = anterior;
            pior = atual;
        } else {
            if (((atual->tam >= tam_total) || (atual->tam >= tam)) && (atual->tam > pior->tam)) {
                anteriorMelhor = anterior;
                pior = atual;
            }
        }
        anterior = atual;
        atual = atual->prox;
    }
    if (pior->tam >= tam_total) {
        void *aux = (void *) pior + sizeof (struct header)+tam;
        anteriorMelhor = pior;
        pior = aux;

        int t = anteriorMelhor->tam - tam - (sizeof (struct header));
        pior->tam = t;
        pior->prox = anteriorMelhor->prox;

        anteriorMelhor->prox = pior;
        anteriorMelhor->tam = 0 - tam;

        return (void *) anteriorMelhor + sizeof (struct header);
    } else {
        if (pior->tam > tam) {
            int t = pior->tam * -1;
            pior->tam = t;
            return (void *) pior + sizeof (struct header);
        }
    }
    printf("Memória indisponível\n");
    return NULL;
}

void* nextfit(int tam) {
    struct header *atual;
    struct header *anterior = NULL;
    int tam_total = tam + sizeof (struct header);
    if (nxt->prox == NULL) {
        atual = buraco;
    } else {
        atual = nxt->prox;
    }
    while (atual != NULL) {
        if (atual->tam >= tam_total) {
            void *aux = (void *) atual + sizeof (struct header)+tam;
            anterior = atual;
            atual = aux;

            int t = anterior->tam - tam - (sizeof (struct header));
            atual->tam = t;
            atual->prox = anterior->prox;

            anterior->prox = atual;
            anterior->tam = 0 - tam;

            nxt = atual;

            return (void *) anterior + sizeof (struct header);
        } else {
            if (atual->tam > tam) {
                int t = atual->tam * -1;
                atual->tam = t;
                nxt = atual;
                return (void *) atual + sizeof (struct header);
            }
        }
        atual = atual->prox;
    }
    atual = buraco;
    while (atual != nxt) {
        if (atual->tam >= tam_total) {
            void *aux = (void *) atual + sizeof (struct header)+tam;
            anterior = atual;
            atual = aux;

            int t = anterior->tam - tam - (sizeof (struct header));
            atual->tam = t;
            atual->prox = anterior->prox;

            anterior->prox = atual;
            anterior->tam = 0 - tam;

            nxt = atual;

            return (void *) anterior + sizeof (struct header);
        } else {
            if (atual->tam > tam) {
                int t = atual->tam * -1;
                atual->tam = t;
                nxt = atual;
                return (void *) atual + sizeof (struct header);
            }
        }
        atual = atual->prox;

    }
    printf("Memória indisponível\n");
    return NULL;
}

void merge(void* end) {
    struct header *anterior = NULL;
    struct header *atual = buraco;
    while (atual != NULL) {

        if (atual == end) {
            if (anterior->tam > 0) {
                int t = atual->tam + anterior->tam + sizeof (struct header);
                anterior->tam = t;
                anterior->prox = atual->prox;
                if (nxt == atual)
                    nxt = anterior;
            } else {
                anterior = anterior->prox;
            }
            atual = atual->prox;
            if (atual->tam > 0) {
                int t = anterior->tam + atual->tam + sizeof (struct header);
                anterior->tam = t;
                anterior->prox = atual->prox;
                if (nxt == atual)
                    nxt = anterior;
                return;
            }
        }
        anterior = atual;
        atual = atual->prox;
    }

}

void libera(void* end) {
    void* aux = end - sizeof (struct header);
    struct header *atual = buraco;
    while (atual != NULL && atual != aux) {
        atual = atual->prox;
    }
    if (atual == NULL || (atual ->tam > 0)) {
        printf("Endereço incorreto!");
        return;
    }
    struct header *bur = aux;
    int t = bur->tam *-1;
    bur->tam = t;
    merge(aux);
}

void mostra_mem() {
    struct header *aux = buraco;
    while (aux != NULL) {
        if (aux->tam > 0)
            printf("Espaço disponível: %d\n", aux->tam);
        else {

            int t = 0 - (aux->tam);
            printf("Espaço alocado: %d\n", t);
        }
        aux = aux->prox;
    }

}

void* meu_aloca(int tam, int opcao) {
    switch (opcao) {
        case 1:
            return firstfit(tam);
            break;
        case 2:
            return bestfit(tam);
            break;
        case 3:
            return worstfit(tam);
            break;
        case 4:
            return nextfit(tam);
            break;
        default:
            return NULL;
            break;
    }
}

int main(int argc, char** argv) {
    inicia(1024);
    void* a1 = meu_aloca(10, 2);
    void* a2 = meu_aloca(5, 2);
    void* a3 = meu_aloca(20, 2);
    void* a4 = meu_aloca(30, 2);
    void* a5 = meu_aloca(15, 2);
    void* a6 = meu_aloca(40, 2);
    void* a7 = meu_aloca(60, 2);
    void* a8 = meu_aloca(80, 2);
    void* a9 = meu_aloca(100, 2);
    mostra_mem();
    libera(a3);
    libera(a2);
    libera(a4);
    libera(a7);
    libera(a8);

    printf("\n\n\n");
    mostra_mem();
    printf("\n");
    void* a10 = meu_aloca(5, 2);
    printf("\n");
    void* a11 = meu_aloca(20, 2);7)>
      
    printf("\n\n\n");
    mostra_mem();

    return (EXIT_SUCCESS);
}

