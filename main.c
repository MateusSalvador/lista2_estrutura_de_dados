#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a estrutura de um nó da lista duplamente encadeada
struct Node {
    char artist[100];
    char song[100];
    struct Node* prev;
    struct Node* next;
};

// Função para criar um novo nó
struct Node* createNode(char artist[], char song[]) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->artist, artist);
    strcpy(newNode->song, song);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Função para inserir um nó no início da lista
void insertAtBeginning(struct Node** head, char artist[], char song[]) {
    struct Node* newNode = createNode(artist, song);
    if (*head == NULL) {
        *head = newNode;
        (*head)->next = newNode;
        (*head)->prev = newNode;
    } else {
        newNode->next = *head;
        newNode->prev = (*head)->prev;
        (*head)->prev->next = newNode;
        (*head)->prev = newNode;
        *head = newNode;
    }
}

// Função para exibir a playlist pela ordem de cadastro
void displayPlaylistByOrder(struct Node* head) {
    if (head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    struct Node* current = head;
    printf("Playlist pela ordem de cadastro:\n");
    do {
        printf("%s;%s\n", current->artist, current->song);
        current = current->next;
    } while (current != head);
}

// Função para exibir a playlist ordenada pelo nome das músicas
void displayPlaylistBySongName(struct Node* head) {
    if (head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    struct Node* current = head;
    char songs[100][100];
    int count = 0;

    do {
        strcpy(songs[count], current->song);
        current = current->next;
        count++;
    } while (current != head);

    printf("Playlist ordenada pelo nome das musicas:\n");

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(songs[i], songs[j]) > 0) {
                char temp[100];
                strcpy(temp, songs[i]);
                strcpy(songs[i], songs[j]);
                strcpy(songs[j], temp);
            }
        }
    }

    for (int i = 0; i < count; i++) {
        current = head;
        do {
            if (strcmp(current->song, songs[i]) == 0) {
                printf("%s;%s\n", current->artist, current->song);
                break;
            }
            current = current->next;
        } while (current != head);
    }
}

// Função para avançar para a próxima música
struct Node* nextSong(struct Node* current) {
    return current->next;
}

// Função para retornar à música anterior
struct Node* previousSong(struct Node* current) {
    return current->prev;
}

// Função para buscar uma música
struct Node* searchSong(struct Node* head, char song[]) {
    if (head == NULL) {
        return NULL;
    }

    struct Node* current = head;
    do {
        if (strcmp(current->song, song) == 0) {
            return current;
        }
        current = current->next;
    } while (current != head);

    return NULL;
}

// Função para remover uma música
void removeSong(struct Node** head, char song[]) {
    struct Node* songToRemove = searchSong(*head, song);
    if (songToRemove == NULL) {
        printf("A musica não foi encontrada na playlist.\n");
        return;
    }

    if (*head == songToRemove) {
        if (songToRemove->next == songToRemove) {
            free(songToRemove);
            *head = NULL;
        } else {
            (*head)->next = songToRemove->next;
            (*head)->prev = songToRemove->prev;
            songToRemove->next->prev = *head;
            songToRemove->prev->next = *head;
            *head = (*head)->next;
            free(songToRemove);
        }
    } else {
        songToRemove->prev->next = songToRemove->next;
        songToRemove->next->prev = songToRemove->prev;
        free(songToRemove);
    }
}

// Função para salvar a lista em um arquivo
void savePlaylistToFile(struct Node* head) {
    if (head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    FILE* file = fopen("C:\\Users\\User\\Desktop\\lista02_ed\\musicas.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    struct Node* current = head;
    do {
        fprintf(file, "%s;%s\n", current->artist, current->song);
        current = current->next;
    } while (current != head);

    fclose(file);
}

int main() {
    char songToSearch[100];
    char newArtist[100];
    char newSong[100];
    char songToRemove[100];
    struct Node* head = NULL;
    FILE* file = fopen("C:\\Users\\User\\Desktop\\lista02_ed\\musicas.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de musicas.\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char artist[100];
        char song[100];
        sscanf(line, "%99[^;];%99[^\n]", artist, song);
        insertAtBeginning(&head, artist, song);
    }

    fclose(file);

    int choice;
    struct Node* current = head;

    do {
        printf("\n-----------------MENU-----------------\n");
        printf("1. Exibir playlist pela ordem de cadastro\n");
        printf("2. Exibir playlist ordenada pelo nome das musicas\n");
        printf("3. Inserir nova musica\n");
        printf("4. Remover musica\n");
        printf("5. Buscar musica\n");
        printf("6. Avancar para a proxima musica\n");
        printf("7. Retornar para a musica anterior\n");
        printf("8. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);
        getchar(); // Limpa o buffer de entrada
        printf("\n--------------RESULTADO--------------\n");

        switch (choice) {
            case 1:
                displayPlaylistByOrder(head);
                break;
            case 2:
                displayPlaylistBySongName(head);
                break;
            case 3:
                printf("Informe o nome do artista: ");
                fgets(newArtist, sizeof(newArtist), stdin);
                newArtist[strcspn(newArtist, "\n")] = 0; // Remove a quebra de linha
                printf("Informe o nome da musica: ");
                fgets(newSong, sizeof(newSong), stdin);
                newSong[strcspn(newSong, "\n")] = 0; // Remove a quebra de linha
                insertAtBeginning(&head, newArtist, newSong);
                savePlaylistToFile(head);
                break;
            case 4:
                printf("Informe o nome da musica a ser removida: ");
                fgets(songToRemove, sizeof(songToRemove), stdin);
                songToRemove[strcspn(songToRemove, "\n")] = 0; // Remove a quebra de linha
                removeSong(&head, songToRemove);
                savePlaylistToFile(head);
                break;
            case 5:
                printf("Informe o nome da musica a ser buscada: ");
                fgets(songToSearch, sizeof(songToSearch), stdin);
                songToSearch[strcspn(songToSearch, "\n")] = 0; // Remove a quebra de linha
                struct Node* foundSong = searchSong(head, songToSearch);
                if (foundSong) {
                    printf("musica encontrada: %s;%s\n", foundSong->artist, foundSong->song);
                } else {
                    printf("musica não encontrada na playlist.\n");
                }
                break;
            case 6:
                current = nextSong(current);
                printf("musica atual: %s;%s\n", current->artist, current->song);
                break;
            case 7:
                current = previousSong(current);
                printf("musica atual: %s;%s\n", current->artist, current->song);
                break;
            case 8:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 8);

    return 0;
}