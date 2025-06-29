// launcher.c
#include <gtk/gtk.h>
#include <stdlib.h>

void launch_server(GtkWidget *widget, gpointer data) {
    system("gnome-terminal -- bash -c './server; exec bash'");
}

void launch_client(GtkWidget *widget, gpointer data) {
    system("gnome-terminal -- bash -c './client; exec bash'");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Launcher");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *server_btn = gtk_button_new_with_label("Launch Server");
    GtkWidget *client_btn = gtk_button_new_with_label("Add User");

    g_signal_connect(server_btn, "clicked", G_CALLBACK(launch_server), NULL);
    g_signal_connect(client_btn, "clicked", G_CALLBACK(launch_client), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), server_btn, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), client_btn, TRUE, TRUE, 10);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
