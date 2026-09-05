#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

struct indice_cep
{
    char cep[8];
    long posicao;
};

int main(int argc, char **argv)
{
    FILE *file;
    Endereco endereco;
    struct indice_cep *indices;
    long tamanhoBytes;
    long tamanhoRegistros;

    if (argc != 2) {
        fprintf(stderr, "USO: %s [CEP]\n", argv[0]);
        return 1;
    }

    file = fopen("cep.dat", "rb");

    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    tamanhoBytes = ftell(file);
    tamanhoRegistros = tamanhoBytes / sizeof(Endereco);
    fseek(file, 0, SEEK_SET);
    indices = malloc(tamanhoRegistros * sizeof(struct indice_cep));

    if (indices == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        fclose(file);
        return 1;
    }

    for (long i = 0; i < tamanhoRegistros; i++) {

        fread(&endereco, sizeof(Endereco), 1, file);

        memcpy(indices[i].cep, endereco.cep, 8);

        indices[i].posicao = i;
    }

    printf("Total de registros: %ld\n", tamanhoRegistros);
    quickSort(indices, 0, tamanhoRegistros - 1);
    free(indices);
    fclose(file);

    return 0;
}
