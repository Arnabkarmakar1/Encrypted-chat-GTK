// client.c
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    system("gnome-terminal -- bash -c './client_ui; exec bash'");
    return 0;
}
