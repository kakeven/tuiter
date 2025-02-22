#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

typedef struct {
    char hashtag[20];
    int count;
} TrendingTopic;

// Funcoes
void limpar_buffer();
int carregar_tamanho();
void salvar_tamanho(int tamanho);
void cadastrar_usuario(Cadastro *cadastros, int *tamanho, int *indice);
void listar_usuarios(Cadastro *cadastros, int tamanho);
int fazer_login(Cadastro *cadastros, int tamanho, char *usuario_logado);
void fazer_post(char *usuario_logado, TrendingTopic *topics, int *num_topics);
void ver_timeline();
void exibir_trending_topics(TrendingTopic *topics, int num_topics);
void adicionar_hashtag(char *conteudo, TrendingTopic *topics, int *num_topics);
void menu_cadastro(Cadastro *cadastros, int *tamanho, int *indice);
void menu_principal(Cadastro *cadastros, int *tamanho, int *indice, TrendingTopic *topics, int *num_topics);
int comparar_hashtags(const void *a, const void *b);

// main
int main() {
    int tamanho = carregar_tamanho();
    Cadastro *cadastros = malloc(sizeof(Cadastro) * tamanho);

    if (cadastros == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    TrendingTopic topics[100];
    int num_topics = 0;

    int indice = 0; // número de usuários cadastrados
    menu_principal(cadastros, &tamanho, &indice, topics, &num_topics); // inicia o menu

    free(cadastros);
    return 0;
}

// Função para limpar o buffer de entrada
void limpar_buffer() {
    while (getchar() != '\n');
}

// função para carregar o tamanho do arquivo
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

// Cadastrar um novo usuário
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

// Listar todos os usuários cadastrados
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

// Função para adicionar hashtags aos Trending Topics
void adicionar_hashtag(char *conteudo, TrendingTopic *topics, int *num_topics) {
    char hashtag[20];
    int i = 0, j = 0;

    while (conteudo[i] != '\0') {
        if (conteudo[i] == '#') {
            i++;
            j = 0;

            while (isalnum(conteudo[i]) || conteudo[i] == '_') {
                hashtag[j++] = conteudo[i++];
            }
            hashtag[j] = '\0';

            int found = 0;
            for (int k = 0; k < *num_topics; k++) {
                if (strcmp(topics[k].hashtag, hashtag) == 0) {
                    topics[k].count++;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                strcpy(topics[*num_topics].hashtag, hashtag);
                topics[*num_topics].count = 1;
                (*num_topics)++;
            }
        }
        i++;
    }
}

// Função para fazer um post
void fazer_post(char *usuario_logado, TrendingTopic *topics, int *num_topics) {
    FILE *arquivo_posts = fopen("Posts.txt", "a+");

    if (arquivo_posts == NULL) {
        printf("Erro ao abrir o arquivo de posts!\n");
        return;
    }

    Post post;
    strcpy(post.autor, usuario_logado);

    printf("\n=== Novo Post ===\n");
    printf("Você escolheu fazer um post? Se sim, aperte Enter e escreva o que você está pensando!: ");
    limpar_buffer();
    fgets(post.conteudo, 100, stdin);
    post.conteudo[strcspn(post.conteudo, "\n")] = '\0';

    // Adiciona hashtags aos trending topics
    adicionar_hashtag(post.conteudo, topics, num_topics);

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

// Função para exibir os Trending Topics
void exibir_trending_topics(TrendingTopic *topics, int num_topics) {
    qsort(topics, num_topics, sizeof(TrendingTopic), comparar_hashtags);

    printf("\n=== Trending Topics ===\n");
    for (int i = 0; i < num_topics && i < 5; i++) {
        printf("%s - %d\n", topics[i].hashtag, topics[i].count);
    }
}

// Função para comparar hashtags para ordenação
int comparar_hashtags(const void *a, const void *b) {
    TrendingTopic *topicA = (TrendingTopic *)a;
    TrendingTopic *topicB = (TrendingTopic *)b;
    return topicB->count - topicA->count;
}

//menu de cadastro
void menu_cadastro(Cadastro *cadastros, int *tamanho, int *indice) {
    int resposta_cadastro;

    while (1) {
        printf("\n+---------------------------+\n");
        printf("| 1 - Cadastrar usuário     |\n");
        printf("| 2 - Listar usuários       |\n");
        printf("| 3 - Voltar                |\n");
        printf("+---------------------------+\n");
        printf("Selecione uma opção: ");
        scanf("%d", &resposta_cadastro);
        limpar_buffer();

        switch (resposta_cadastro) {
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
void menu_principal(Cadastro *cadastros, int *tamanho, int *indice, TrendingTopic *topics, int *num_topics) {
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
                                fazer_post(usuario_logado, topics, num_topics);
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
