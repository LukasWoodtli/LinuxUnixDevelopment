#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define BUF 1024

// use this server with client1
// but it's not working very well

int main(void) {
    int create_socket, new_socket;
    socklen_t addrlen;
    char buffer[BUF + 1];
    struct sockaddr_in address;
    long save_fd;
    const int y = 1;
    ssize_t size;

    printf("\e[2J");

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0)
        printf("Socket wurde angelegt\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(15000);

    setsockopt(create_socket, SOL_SOCKET,
               SO_REUSEADDR, &y, sizeof(int));

    if (bind(create_socket, (struct sockaddr*)&address, sizeof(address)) != 0) {
        printf("Port ist nicht frei!\n");
    }

    listen(create_socket, 5);
    addrlen = sizeof(struct sockaddr_in);

    save_fd = fcntl(create_socket, F_GETFL);
    save_fd |= O_NONBLOCK;
    fcntl(create_socket, F_SETFL, save_fd);

    while (1) {
        new_socket = accept(create_socket,
                            (struct sockaddr*)&address,
                            &addrlen);

        if (new_socket > 0) {
            printf("Ein client (%s) ist verbunden ...\n",
                   inet_ntoa(address.sin_addr));
        }
        else {
            printf("Poll: Kein Client am Socket ...\n");
            sleep(1);
            continue;
        }

        do {
            save_fd = fcntl(new_socket, F_GETFL);
            save_fd |= O_NONBLOCK;
            fcntl(new_socket, F_SETFL, save_fd);
            printf("Nachricht zum versenden: ");
            fgets(buffer, BUF, stdin);
            send(new_socket, buffer, strlen(buffer), 0);
            size = recv(new_socket, buffer, BUF-1, 0);

            if (size > 0)
                buffer[size] = '\0';

            printf("Nachricht empfangen: %s\n", buffer);
        } while (strcmp(buffer, "quit\n") != 0);

        close(new_socket);
    }

    close(create_socket);

    return EXIT_SUCCESS;
}
