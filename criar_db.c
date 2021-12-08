#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include<ctype.h>

void insert(FILE *arquivo);
void select(FILE *arquivo);
/*void delete(FILE *arquivo);
void update(FILE *arquivo);*/

int key(FILE *arquivo, int cod);

struct st_registro
{
    int id;
    char type[50];
    char atributos[300]; 
};
typedef struct st_registro registro;

int main()
{
    int k, opcao;
    FILE *arquivo;
    
    for (k = 0; opcao != 5;)
    {
        printf("\n\n=======SIMPLEDB=======:\n");
        printf("\n\n1 - INSERIR REGISTRO\n2 - SELECIONAR REGISTRO\n3 - ALTERAR REGISTRO\n4 - DELETAR REGISTRO\n\n");
        scanf("%d", &opcao);
        
        switch (opcao)
        {
        case 1:
            if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
            {
                if((arquivo = fopen("simpledb.dat", "w+b"))==NULL)
                {
                printf("Erro ao abrir o arquivo!");
                }
            }
            insert(arquivo);
            fclose(arquivo);
            break;
        
        case 2:
            if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
            {
                printf("Erro ao abrir o arquivo!");
            }
            select(arquivo);
            fclose(arquivo);
            break;
        }
    }
    getchar();
    return 0;
}


int cod_reg(FILE *arquivo)
{ 
    registro reg;
    int cod = 1;
    fseek(arquivo,0,SEEK_SET);
    fread(&reg, sizeof(reg),1, arquivo);
    while (!feof(arquivo))
    { 
        if (reg.id <= cod)
        { 
            cod ++;
        }
    }
    return cod;
}


void inserir(FILE *arquivo)
{
    registro reg;
    int posicao;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        if((arquivo = fopen("simpledb.dat", "w+b"))==NULL)
        {
            printf("Erro ao abrir o arquivo!");
        }
    }

    reg.id = cod_reg(arquivo);
    
    printf("\n sort-key ");
    fflush(stdin);
    fgets(reg.type, 50, stdin);

    printf("\n value ");
    fflush(stdin);
    fgets(reg.atributos, 50, stdin);

    fseek(arquivo,0,SEEK_END);
    fwrite(&reg, sizeof(reg),1, arquivo);
    fflush(arquivo);

    getchar(); getchar();
    system("clear");
}


void select(FILE *arquivo)
{
    char posicao;

    registro reg;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        printf("Erro ao abrir o arquivo!");
        return main();
    }
    
    printf("\n sort key: \n");
    fgets(posicao, 50, stdin);
    fseek(arquivo, sizeof(reg)*(posicao-1), SEEK_SET);
    fread(&reg,sizeof(registro),1,arquivo);
    printf("\n\nDados:");
    printf("\n\nid: %d\nsort-key: %s\natributos: %s",reg.id,reg.type,reg.atributos);

    fclose(arquivo);
    getchar(); getchar();
    system("clear");        
}
