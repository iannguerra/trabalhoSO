#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include"inserir_registro.c"
#include"selecionar_todos_registros.c"
#include"selecionar_registro_especifico.c"
#include"alterar_registro.c"
#include"deletar_registro.c"
#include"struct.h"


int main()
{
    int opcao = 0;
    int cont = 0; 
    FILE *arquivo;

    while(opcao != 6) //Enquanto for diferente da opção de encerrar, será necessário digitar um valor válido
    {
        printf("\n\n=======SIMPLEDB=======\n");
        printf("\n\n1 - INSERIR REGISTRO\n2 - SELECIONAR TODOS REGISTRO\n3 - SELECIONAR REGISTRO ESPECIFICO\n4 - ALTERAR REGISTRO\n5 - DELETAR REGISTRO\n6 - ENCERRAR SIMPLEDB\n\n");
        scanf("%d", &opcao);
        
        switch (opcao)
        {
            case 1:
                cont ++;
                inserir(arquivo, cont);
                break;
            
            case 2:
                selecionar(arquivo);
                break;

            case 3:
                selecionar_especifico(arquivo);
                break;
            
            case 4:
                alterar(arquivo);
                break;

            case 5:
                deletar(arquivo);
                break;

            //default: printf("Digite uma opção válida");
        }
    }
    return 0;
}






