// client_ui.c
#include <gtk/gtk.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 8888
#define BUFFER_SIZE 2048

int sock;
GtkWidget *text_view;
GtkWidget *entry;
char username[50];

void xor_encrypt_decrypt(char *msg) {
    char key = 0xAA;
    for (int i = 0; msg[i] != '\0'; i++)
        msg[i] ^= key;
}

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = recv(sock, buffer, BUFFER_SIZE, 0);
        if (n <= 0) break;
        xor_encrypt_decrypt(buffer);

        GtkTextBuffer *buffer_widget = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(buffer_widget, &end);
        gtk_text_buffer_insert(buffer_widget, &end, buffer, -1);
        gtk_text_buffer_insert(buffer_widget, &end, "\n", -1);
    }
    close(sock);
    exit(0);
}

void send_message(GtkWidget *widget, gpointer data) {
    const char *msg = gtk_entry_get_text(GTK_ENTRY(entry));
    if (strlen(msg) == 0) return;

    if (strcmp(msg, "/exit") == 0) {
        close(sock);
        gtk_main_quit();
        return;
    }

    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%s: %s", username, msg);
    xor_encrypt_decrypt(buffer);
    send(sock, buffer, strlen(buffer), 0);
    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;

    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    send(sock, username, strlen(username), 0);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), username);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    entry = gtk_entry_new();
    GtkWidget *send_btn = gtk_button_new_with_label("Send");

    g_signal_connect(send_btn, "clicked", G_CALLBACK(send_message), NULL);
    g_signal_connect(entry, "activate", G_CALLBACK(send_message), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), send_btn, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
