#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include<ctype.h>
#include<pthread.h>

void inserir(FILE *arquivo, int cont);
void selecionar(FILE *arquivo);
void selecionar_especifico(FILE *arquivo);
void deletar(FILE *arquivo);
void alterar(FILE *arquivo);
void *t1Func(void *arg);

int key(FILE *arquivo, int cod);

struct st_registro
{
    int id;
    char type[300];
    char atributos[300]; 
};
typedef struct st_registro registro;

int main(int argc, char **argv)
{
    pthread_t t1;
    pthread_create(&t1, NULL, t1Func, NULL);

    int opcao;
    int cont = 0;
    FILE *arquivo;

    while(opcao != 5)
    {
        printf("\n\n=======SIMPLEDB=======\n");
        printf("\n\n1 - INSERIR REGISTRO\n2 - SELECIONAR TODOS REGISTRO\n3 - SELECIONAR REGISTRO ESPECIFICO\n4 - ALTERAR REGISTRO\n\n");
        scanf("%d", &opcao);
        
        switch (opcao)
        {
            case 1:
                if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
                {
                    if((arquivo = fopen("simpledb.dat", "w+b"))==NULL)
                    {
                    pthread_join(t1, NULL);
                    }
                }
                
                cont ++;
                inserir(arquivo, cont);
                fclose(arquivo);
                break;
            
            case 2:
                if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
                {
                    pthread_join(t1, NULL);
                }
                selecionar(arquivo);
                fclose(arquivo);
                break;

            case 3:
                if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
                {
                    pthread_join(t1, NULL);
                }
                selecionar_especifico(arquivo);
                fclose(arquivo);
                break;
            
            case 4:
                if((arquivo = fopen("simpledb.dat", "rb+"))==NULL)
                {
                    pthread_join(t1, NULL);
                }
                alterar(arquivo);
                fclose(arquivo);
                break;
        }
        
    }
    return 0;
}


void inserir(FILE *arquivo, int cont)
{
    registro reg;

    reg.id = cont;

    printf("\nSORT-KEY: ");
    fgets(reg.type, 300, stdin);
    fgets(reg.type, 300, stdin);
    printf("\nVALUE: ");
    fgets(reg.atributos, 300, stdin);

    fseek(arquivo,0,SEEK_END);
    fwrite(&reg, sizeof(reg),1, arquivo);
    fflush(arquivo);
    
    system("clear");
}


void selecionar(FILE *arquivo)
{
        
    registro reg;

    while (fread(&reg,sizeof(registro),1,arquivo)==1)
    {
        printf("\n\nDados:");
        printf("\nid: %d\nsort-key: %s\natributos: %s",reg.id,reg.type,reg.atributos);
    }
    
    getchar(); getchar();
    system("clear");        
}

void selecionar_especifico(FILE *arquivo)
{
        
    registro reg;
    int encontrar = 0;

    printf("Digite o id: ");
    scanf("%d",&encontrar);

    while (fread(&reg,sizeof(registro),1,arquivo)==1)
    {
       if(reg.id == encontrar)
        {
            printf("\n\nDados:");
            printf("\n\nid: %d\nsort-key: %s\natributos: %s",reg.id,reg.type,reg.atributos);
        }
    }
    
    getchar(); getchar();
    system("clear");        
}


void alterar(FILE *arquivo)
{
    int id = 0;
    registro reg;
    
    printf("\nDigite o ID do registro que deseja alterar: ");
    scanf("%d",&id);
    
    printf("\nValue a ser reescritos: ");
    fgets(reg.atributos, 300, stdin);
    fgets(reg.atributos, 300, stdin);


    fseek(arquivo, (id - 1)*sizeof(registro), SEEK_SET);
    fwrite(&reg, sizeof(registro),1, arquivo);

}

void *t1Func(void *arg) {
    printf("Erro ao abrir o arquivo");
    return NULL;
}