// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_CLIENTS 100
#define BUFFER_SIZE 2048

int clients[MAX_CLIENTS];
char usernames[MAX_CLIENTS][50];
int client_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void xor_encrypt_decrypt(char *msg) {
    char key = 0xAA;
    for (int i = 0; msg[i] != '\0'; i++) {
        msg[i] ^= key;
    }
}

void broadcast_message(char *message, int sender_sock) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_sock) {
            char *encrypted = malloc(BUFFER_SIZE);
            sprintf(encrypted, "%s", message);
            xor_encrypt_decrypt(encrypted);
            send(clients[i], encrypted, strlen(encrypted), 0);
            free(encrypted);
        }
    }
    pthread_mutex_unlock(&lock);
}

void *client_handler(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    char username[50];

    memset(username, 0, sizeof(username));
    int bytes_received = recv(client_sock, username, sizeof(username), 0);
    if (bytes_received <= 0) {
        close(client_sock);
        pthread_exit(NULL);
    }

    pthread_mutex_lock(&lock);
    clients[client_count] = client_sock;
    strncpy(usernames[client_count], username, sizeof(usernames[client_count]));
    client_count++;
    pthread_mutex_unlock(&lock);

    printf("[Server] %s joined the chat.\n", username);
    char join_msg[BUFFER_SIZE];
    sprintf(join_msg, "[%s] joined the chat.", username);
    broadcast_message(join_msg, -1);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            printf("[Server] %s disconnected.\n", username);
            char exit_msg[BUFFER_SIZE];
            sprintf(exit_msg, "[%s] has left the chat.", username);
            broadcast_message(exit_msg, -1);
            break;
        }

        xor_encrypt_decrypt(buffer);

        if (strcmp(buffer, "/exit") == 0) {
            printf("[Server] %s typed /exit. Disconnecting them.\n", username);
            char exit_msg[BUFFER_SIZE];
            sprintf(exit_msg, "[%s] has left the chat.", username);
            broadcast_message(exit_msg, -1);
            break;
        }

        char tagged_msg[BUFFER_SIZE];
        sprintf(tagged_msg, "[%s]: %s", username, buffer);
        printf("%s\n", tagged_msg);
        broadcast_message(tagged_msg, client_sock);
    }

    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == client_sock) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
                strcpy(usernames[j], usernames[j + 1]);
            }
            break;
        }
    }
    client_count--;
    pthread_mutex_unlock(&lock);

    close(client_sock);
    pthread_exit(NULL);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, MAX_CLIENTS);

    printf("[Server] Chatroom started on port %d...\n", PORT);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_sock;
        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, new_sock);
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}
