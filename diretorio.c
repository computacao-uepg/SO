//
// DIRETORIOS.C
//
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static int pesquisa(const char *arg, const char *raiz, const char *path) {

    DIR *dirpont;
    struct dirent *dp;
    int achou = 0;
    if ((dirpont = opendir(path)) == NULL) {
        perror("Nao pode abrir\n\n");
        return achou;
    }
    do {
        errno = 0;
        if ((dp = readdir(dirpont)) != NULL) {
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                continue;
            if (strcmp(dp->d_name, arg) == 0) {
                achou = 1;
                struct stat informacao;
                char caminho[1000];
                memset(caminho, 0, sizeof (caminho));
                strcat(caminho, path);
                strcat(caminho, "/");
                strcat(caminho, dp->d_name);
                stat(caminho, &informacao);
                (void) printf("Achou o arquivo: %s\nTipo: %d\nTamanho: %ld bytes\nTamanho fisico: %ld bytes\nNumero de blocos: %ld\nTamanho do bloco: %ld\n", caminho, dp->d_type, informacao.st_size, informacao.st_blocks * 512, informacao.st_blocks, informacao.st_blksize);
                //Tamanho
                //Tamanho do bloco
                //Quantidade do bloco
                (void) closedir(dirpont);
                return achou;
            }
            if (dp->d_type == DT_DIR) {
                char caminho[1000];
                memset(caminho, 0, sizeof (caminho));

                strcat(caminho, path);
                strcat(caminho, "/");
                strcat(caminho, dp->d_name);
                (void) printf("%s\n", caminho);
                if (pesquisa(arg, raiz, caminho) == 1) {
                    return 1;
                }
            }
            if (strcmp(dp->d_name, arg) != 0)
                continue;
        }
        if (achou == 0) {
            (void) closedir(dirpont);
            return achou;
        }
    } while (dp != NULL);

    if (errno != 0)
        perror("Erro lendo o diretorio\n\n");
    else
        (void) printf("Falha ao procurar: %s\n", arg);
    (void) closedir(dirpont);
    return achou;
}

int main(int argc, char *argv[]) {
    int i;

    if (argc < 2) {
        printf("USO: %s nome_arquivo ...\n\n", argv[ 0 ]);
        return ( -1);
    }
    for (i = 1; i < argc; i++)
        if (!pesquisa(argv[i], ".", "."))
            printf("Nao encontrou nada\n");
    return (0);
}