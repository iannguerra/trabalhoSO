#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

void inserir(FILE *arquivo, int cont, pthread_t t1); // Função de inserir dados no arquivo binário
void selecionar(FILE *arquivo, pthread_t t1); // Função para selecionar todas as informações presentes no arquivo
void selecionar_especifico(FILE *arquivo, pthread_t t1); // Função para selecionar informações específicas
void alterar(FILE *arquivo, pthread_t t1); // Função para alterar attributes de registro específico
void deletar(FILE *arquivo, pthread_t t1); // Função para deletar registro específico
void *t1_rotina_erro(void *arg); //Função da thread

struct st_registro
{
    int id; // Os itens id e type da struct registro formam uma chave primária
    char type[300];
    char attributes[300]; // Informações amarradas à chave primária
};
typedef struct st_registro registro;

int main()
{
    int opcao = 0;
    int cont = 0; 
    FILE *arquivo;

    pthread_t t1;
    pthread_create(&t1, NULL, t1_rotina_erro, NULL); //Criando thread t1
    while(opcao != 6) //Enquanto for diferente da opção de encerrar, será necessário digitar um valor válido
    {
        printf("\n\n=======SIMPLEDB=======\n");
        printf("\n\n1 - INSERIR REGISTRO\n2 - SELECIONAR TODOS REGISTRO\n3 - SELECIONAR REGISTRO ESPECIFICO\n4 - ALTERAR REGISTRO\n5 - DELETAR REGISTRO\n6 - ENCERRAR SIMPLEDB\n\n");
        scanf("%d", &opcao);
        
        switch (opcao)
        {
            case 1:
                cont ++;
                inserir(arquivo, cont, t1);
                break;
            
            case 2:
                selecionar(arquivo, t1);
                break;

            case 3:
                selecionar_especifico(arquivo, t1);
                break;
            
            case 4:
                alterar(arquivo, t1);
                break;

            case 5:
                deletar(arquivo, t1);
                break;

            //default: printf("Digite uma opção válida");
        }
    }
    return 0;
}

void inserir(FILE *arquivo, int cont, pthread_t t1)
{
    registro reg;
    reg.id = cont;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        if((arquivo = fopen("simpledb.dat", "w+b"))==NULL)
        {
            pthread_join(t1, NULL);
        }
    }

    printf("\nSORT-KEY: ");
    fgets(reg.type, 300, stdin); //desconsidera primeira entrada
    fgets(reg.type, 300, stdin); //desconsidera primeira entrada
    
    printf("\nVALUE: ");
    fgets(reg.attributes, 300, stdin); //grava dados da entrada stdin
    
    fseek(arquivo,0,SEEK_END); //Posicionar cursor no início do arquivo
    fwrite(&reg, sizeof(reg),1, arquivo); //Gravar dados em arquivo, consigderando o tamanho definido na struct
    fflush(arquivo);

    fclose(arquivo);

    system("clear"); //Limpar tela da aplicação
}


void selecionar(FILE *arquivo, pthread_t t1)
{
    registro reg;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        pthread_join(t1, NULL);
    }

    while (fread(&reg,sizeof(registro),1,arquivo)==1) // Lê informações enquanto não chegar ao fim do arquivo
    {
        printf("\n\nDados:");
        printf("\nid: %d\nsort-key: %s\nattributes: %s",reg.id,reg.type,reg.attributes);
    }
    
    fclose(arquivo);

    getchar(); getchar(); //Capturar um caractere (Enter) 
    system("clear"); //Limpar tela da aplicação        
}

void selecionar_especifico(FILE *arquivo, pthread_t t1)
{
    registro reg;
    int encontrar = 0;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        pthread_join(t1, NULL);
    }

    printf("Digite o id: ");
    scanf("%d",&encontrar);

    while (fread(&reg,sizeof(registro),1,arquivo)==1)
    {
       if(reg.id == encontrar)
        {
            printf("\n\nDados:");
            printf("\n\nid: %d\nsort-key: %s\nattributes: %s",reg.id,reg.type,reg.attributes);
        }
    }

    fclose(arquivo);
    
    getchar(); getchar(); //Capturar um caractere (Enter)
    system("clear"); //Limpar tela da aplicação        
}

void alterar(FILE *arquivo, pthread_t t1)
{
    int id = 0;
    registro reg;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        pthread_join(t1, NULL);
    }
    
    printf("\nDigite o ID do registro que deseja alterar: ");
    scanf("%d",&id);
    
    printf("\nValue a ser reescritos: ");
    fgets(reg.attributes, 300, stdin); //desconsidera primeira entrada
    fgets(reg.attributes, 300, stdin); //grava dados da entrada stdin

    fseek(arquivo, (id - 1)*sizeof(registro), SEEK_SET); //Posicionar cursor no início do arquivo onde o ID informado é -1
    fwrite(&reg, sizeof(registro),1, arquivo); //Gravar dados em arquivo, consigderando o tamanho definido na struct
    fflush(arquivo); // Limpar buffer

    fclose(arquivo);

    system("clear"); //Limpar tela da aplicação
}

void deletar(FILE *arquivo, pthread_t t1)
{
    FILE *arquivo_aux;
    registro reg;
    int id_excluir = 0;

    if((arquivo = fopen("simpledb.dat", "rb+"))==NULL)
    {
        pthread_join(t1, NULL);
    }

    if((arquivo_aux = fopen("arquivo_aux.dat", "w+b"))==NULL) //Reescrever arquivo auxiliar, caso já exista
    {
        pthread_join(t1, NULL);
    }    

    printf("\nDigite o ID do registro que deseja excluir: ");
    scanf("%d",&id_excluir);
    
    while (fread(&reg,sizeof(registro),1,arquivo)==1)
    {
       if(reg.id != id_excluir) //Arquivo auxiliar preenchido por registros com ID's diferentes do selecionado
        {
            fwrite(&reg, sizeof(registro),1,arquivo_aux); //dados salvos em arquivo temporário
        }
    }

    fclose(arquivo_aux); //Fechar arquivo
    fclose(arquivo); //Fechar arquivo

    remove("simpledb.dat"); //Deletar arquivo binário com id a ser excluído
    rename("arquivo_aux.dat", "simpledb.dat"); //Renomear auxiliar com os dados corretos para o nome do antigo arquivo

    system("clear"); //Limpar tela da aplicação
}

void *t1_rotina_erro(void *arg) {
    printf("Erro ao abrir o arquivo");
    return NULL;
}
