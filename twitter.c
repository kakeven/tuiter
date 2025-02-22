#include <stdlib.h>
#include <stdio.h>


typedef struct{
    char login[15];
    char senha[15];
    char nome[15];
}cadastro;

typedef struct{
    cadastro login[15];
    char frase[100];
}post;



void limpar_buffer(){
    while (getchar() != '\n');
}




int main(){
   //arquivo
    FILE *arquivos_dados;
    FILE *arquivo_tamanho;
    int tamanho;
    
    arquivo_tamanho=fopen("tamanho.txt","r");
    fscanf(arquivo_tamanho,"%d",&tamanho);
    fclose(arquivo_tamanho);
    
    if(arquivo_tamanho==NULL){
        tamanho=1;
    }


    cadastro *cadastros=malloc(sizeof(cadastro)*tamanho);
    int resposta_interna,resposta_menu_cadastro,resposta_programa;
    int i=0;
    

    //programa
    while(1){
    printf("+----------------------------+\n");
    printf("| 1 - Timeline               |\n");
    printf("| 2 - Menu de cadastro       |\n");
    printf("| 3 - estatistica de hashtag |\n");
    printf("+----------------------------+\n");
    printf("Selecione uma opcao: "); 
    scanf("%d",&resposta_programa);

    switch (resposta_programa){
    case 1: resposta_programa=1;
        ;;;
        break;
    
    
        
    
    case 2: resposta_programa=2;
    //menu de cadastro
    while(1){
    //menu
    printf("+---------------------+\n");
    printf("| 1 - cadastrar       |\n");
    printf("| 2 - ver cadastrados |\n");
    printf("| 3 - sair            |\n");
    printf("+---------------------+\n");
    printf("Digite a opcao desejada: ");
    scanf("%d",&resposta_menu_cadastro);
    limpar_buffer();
    
    
    switch (resposta_menu_cadastro){
    
    case 1: resposta_menu_cadastro=1;
        
        while(1){
            arquivos_dados=fopen("Dados Usuarios.txt","a+");
            printf("Bem vindo ao XYZ\n");
            
            
            printf("Usuario: %d\nLimite: %d\n",i,tamanho);


            printf("Digite seu nome: ");
            scanf("%s",cadastros[i].nome);

            printf("Digite seu login: ");
            scanf("%s",cadastros[i].login);
            
            printf("Digite sua senha: ");
            scanf("%s",cadastros[i].senha);


            //adiciona ao arquivo
            fprintf(arquivos_dados,"%s %s %s\n",cadastros[i].nome,cadastros[i].login,cadastros[i].senha);
            fclose(arquivos_dados);
    
            //alocacao usuario
            tamanho++;
            arquivo_tamanho=fopen("tamanho.txt","w+");
            fprintf(arquivo_tamanho,"%d",tamanho);
            fclose(arquivo_tamanho);
            
            cadastro *temp = realloc(cadastros, sizeof(cadastro) * tamanho);
            cadastros=temp;
            
            
            
            //realocação
            printf("cadastrar outro usuario ? ");
            scanf("%d",&resposta_interna);
            
            if(resposta_interna==1){
            
            //aumenta o tamanho em 1
            tamanho++;
            
            //abre o arquivo para editar
            arquivo_tamanho=fopen("tamanho.txt","w+");
            fprintf(arquivo_tamanho,"%d",tamanho);
            fclose(arquivo_tamanho);
            cadastro *temp = realloc(cadastros, sizeof(cadastro) * tamanho);
            cadastros=temp;
            i++;
            continue;
            }
            else{break;}
                
        
            
        }
    break; 
        

    case 2: resposta_menu_cadastro=2;
        
        while(1){
        
            //abertura do arquivo tamanho
            arquivo_tamanho=fopen("tamanho.txt","r");
            //abertura do arquivo de dados
            arquivos_dados=fopen("Dados Usuarios.txt","r");
            
            //pega o numero que tem no arquivo de tamanho
            fscanf(arquivo_tamanho,"%d",&tamanho);
            
            
            //ler os dados do arquivo
            int i=0;
        while ( i < tamanho && fscanf(arquivos_dados, "%s %s %s\n", cadastros[i].nome, cadastros[i].login, cadastros[i].senha) == 3) {
            i++;
        }
            
        //fecha o arquivo de dados
        fclose(arquivos_dados);
        
        
        
        printf("Tamanho: %d",tamanho);
        
        printf("\n=== Usuarios Cadastrados ===\n");
        
        //mostra os dados lidos
        for (int j = 0; j < tamanho-1; j++) {
            printf("Usuario %d:\n", j );
            printf("Nome: %s\n", cadastros[j].nome);
            printf("Login: %s\n", cadastros[j].login);
            printf("Senha: %s\n\n", cadastros[j].senha);
        }
        fclose(arquivo_tamanho);
        break;
    }
    break;
    
        
        
    case 3: resposta_menu_cadastro=3;
            exit(1);

        break;
    default:
        break;
        }
        
    
    
    

    }
    free(cadastros);
    break;
    
default:
        break;

    }
    }


}
    

    
    

    
    
    
    
    
    
    
    
    
    
    
    
   








