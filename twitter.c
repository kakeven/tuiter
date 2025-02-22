#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Structs
typedef struct {
    char login[15];
    char senha[15];
    char nome[15];
} Cadastro;
  
typedef struct {
    char autor[15];
    char conteudo[100];
} Post;

//Funcoes
void limpar_buffer();
int carregar_tamanho();
void salvar_tamanho(int tamanho);
void cadastrar_usuario(Cadastro *cadastros, int *tamanho, int *indice);
void listar_usuarios(Cadastro *cadastros, int tamanho);
int fazer_login(Cadastro *cadastros, int tamanho, char *usuario_logado);
void fazer_post(char *usuario_logado);
void ver_timeline();
void menu_cadastro(Cadastro *cadastros, int *tamanho, int *indice);
void menu_principal();

// main
int main() {
    int tamanho = carregar_tamanho();
    Cadastro *cadastros = malloc(sizeof(Cadastro) * tamanho);

    if (cadastros == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    int indice = 0; // numero de usuarios cadastrados
    menu_principal(cadastros, &tamanho, &indice); // inicia o menu

    free(cadastros);
    return 0;
}

// Função para limpar o buffer de entrada
void limpar_buffer() {
    while (getchar() != '\n');
}

//função para carregar o tamanho do arquivo
int carregar_tamanho() {
    FILE *arquivo_tamanho = fopen("tamanho.txt", "r");
    int tamanho = 1;

    if (arquivo_tamanho != NULL) {
        fscanf(arquivo_tamanho, "%d", &tamanho);
        fclose(arquivo_tamanho);
    }

    return tamanho;
}

// Funçao para salvar o tamanho no arquivo
void salvar_tamanho(int tamanho) {
    FILE *arquivo_tamanho = fopen("tamanho.txt", "w");

    if (arquivo_tamanho == NULL) {
        printf("Erro ao salvar o tamanho!\n");
        return;
    }

    fprintf(arquivo_tamanho, "%d", tamanho);
    fclose(arquivo_tamanho);
}

//Cadastrar um novo usuário
void cadastrar_usuario(Cadastro *cadastros, int *tamanho, int *indice) {
    FILE *arquivos_dados = fopen("Dados Usuarios.txt", "a+");

    if (arquivos_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados!\n");
        return;
    }

    printf("\n=== Cadastro de Usuário ===\n");
    printf("Usuário: %d\nLimite: %d\n", *indice, *tamanho);

    printf("Digite seu nome: ");
    scanf("%s", cadastros[*indice].nome);

    printf("Digite seu login: ");
    scanf("%s", cadastros[*indice].login);

    printf("Digite sua senha: ");
    scanf("%s", cadastros[*indice].senha);

    fprintf(arquivos_dados, "%s %s %s\n", cadastros[*indice].nome, cadastros[*indice].login, cadastros[*indice].senha);
    fclose(arquivos_dados);

    (*tamanho)++;
    salvar_tamanho(*tamanho);

    Cadastro *temp = realloc(cadastros, sizeof(Cadastro) * (*tamanho));
    if (temp == NULL) {
        printf("Erro ao realocar memória!\n");
        free(cadastros);
        exit(1);
    }
    cadastros = temp;

    (*indice)++;
    printf("Usuário cadastrado com sucesso!\n");
}

//listar todos os usuários cadastrados
void listar_usuarios(Cadastro *cadastros, int tamanho) {
    FILE *arquivos_dados = fopen("Dados Usuarios.txt", "r");
    if (arquivos_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados!\n");
        return;
    }

    printf("\n=== Usuários Cadastrados ===\n");
    for (int i = 0; i < tamanho - 1; i++) {
        if (fscanf(arquivos_dados, "%s %s %s", cadastros[i].nome, cadastros[i].login, cadastros[i].senha) == 3) {
            printf("Usuário %d:\n", i + 1);
            printf("Nome: %s\n", cadastros[i].nome);
            printf("Login: %s\n", cadastros[i].login);
            printf("Senha: %s\n\n", cadastros[i].senha);
        }
    }
    fclose(arquivos_dados);
}

// Função para fazer login
int fazer_login(Cadastro *cadastros, int tamanho, char *usuario_logado) {
    char login[15], senha[15];

    printf("\n=== Login ===\n");
    printf("Digite seu login: ");
    scanf("%s", login);

    printf("Digite sua senha: ");
    scanf("%s", senha);

    FILE *arquivos_dados = fopen("Dados Usuarios.txt", "r");
    if (arquivos_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados!\n");
        return 0;
    }

    Cadastro temp;
    while (fscanf(arquivos_dados, "%s %s %s", temp.nome, temp.login, temp.senha) == 3) {
        if (strcmp(temp.login, login) == 0 && strcmp(temp.senha, senha) == 0) {
            strcpy(usuario_logado, temp.login); //salvar o nome do usuário logado
            fclose(arquivos_dados);
            printf("Login bem-sucedido! Bem-vindo, %s.\n", temp.nome);
            return 1;
        }
    }

    fclose(arquivos_dados);
    printf("Login ou senha incorretos.\n");
    return 0;
}

// Função para fazer um post
void fazer_post(char *usuario_logado) {
    FILE *arquivo_posts = fopen("Posts.txt", "a+");

    if (arquivo_posts == NULL) {
        printf("Erro ao abrir o arquivo de posts!\n");
        return;
    }

    Post post;
    strcpy(post.autor, usuario_logado);

    printf("\n=== Novo Post ===\n");
    printf("Você escolheu fazer um post? se sim aperte Enter e escreva o que você está pensando!: ");
    limpar_buffer();
    fgets(post.conteudo, 100, stdin);
    post.conteudo[strcspn(post.conteudo, "\n")] = '\0';

    fprintf(arquivo_posts, "%s: %s\n", post.autor, post.conteudo);
    fclose(arquivo_posts);

    printf("Post publicado com sucesso!\n");
}

// Função para ver a timeline
void ver_timeline() {
    FILE *arquivo_posts = fopen("Posts.txt", "r");

    if (arquivo_posts == NULL) {
        printf("Erro ao abrir o arquivo de posts!\n");
        return;
    }

    printf("\n=== Timeline ===\n");
    char linha[120];
    while (fgets(linha, sizeof(linha), arquivo_posts) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        printf("%s\n", linha);
    }

    fclose(arquivo_posts);
}

//menu de csdastro
void menu_cadastro(Cadastro *cadastros, int *tamanho, int *indice) {
    int resposta_menu_cadastro;

    while (1) {
        printf("\n+---------------------+\n");
        printf("| 1 - Cadastrar       |\n");
        printf("| 2 - Ver cadastrados |\n");
        printf("| 3 - Voltar          |\n");
        printf("+---------------------+\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &resposta_menu_cadastro);
        limpar_buffer();

        switch (resposta_menu_cadastro) {
            case 1:
                cadastrar_usuario(cadastros, tamanho, indice);
                break;

            case 2:
                listar_usuarios(cadastros, *tamanho);
                break;

            case 3:
                return;

            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    }
}

//menu principal
void menu_principal(Cadastro *cadastros, int *tamanho, int *indice) {
    int resposta_programa;
    char usuario_logado[15] = "";
    while (1) {
        printf("\n+----------------------------+\n");
        printf("| 1 - Login                  |\n");
        printf("| 2 - Menu de cadastro       |\n");
        printf("| 3 - Timeline               |\n");
        printf("| 4 - Sair                   |\n");
        printf("+----------------------------+\n");
        printf("Selecione uma opção: ");
        scanf("%d", &resposta_programa);
        limpar_buffer();

        switch (resposta_programa) {
            case 1:
                if (fazer_login(cadastros, *tamanho, usuario_logado)) {
                    int opcao_usuario;
                    while (1) {
                        printf("\n+---------------------+\n");
                        printf("| 1 - Fazer post      |\n");
                        printf("| 2 - Ver timeline    |\n");
                        printf("| 3 - Logout          |\n");
                        printf("+---------------------+\n");
                        printf("Digite a opção desejada: ");
                        scanf("%d", &opcao_usuario);
                        limpar_buffer();

                        switch (opcao_usuario) {
                            case 1:
                                fazer_post(usuario_logado);
                                break;

                            case 2:
                                ver_timeline();
                                break;

                            case 3:
                                printf("Logout realizado.\n");
                                strcpy(usuario_logado, "");
                                break;

                            default:
                                printf("Opção inválida! Tente novamente.\n");
                                break;
                        }

                        if (opcao_usuario == 3) break;
                    }
                }
                break;

            case 2:
                menu_cadastro(cadastros, tamanho, indice);
                break;

            case 3:
                ver_timeline();
                break;

            case 4:
                printf("\nSaindo...\n");
                return;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }
    }
}