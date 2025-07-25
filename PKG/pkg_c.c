#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

int descargar(const char *host, const char *path, const char *output) {
    struct hostent *server = gethostbyname(host);
    if (!server) {
        perror("gethostbyname");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    char req[1024];
    snprintf(req, sizeof(req), "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", path, host);
    send(sock, req, strlen(req), 0);

    FILE *fp = fopen(output, "wb");
    if (!fp) {
        perror("fopen");
        close(sock);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int header_done = 0;

    while (1) {
        int n = recv(sock, buffer, BUFFER_SIZE, 0);
        if (n <= 0) break;

        if (!header_done) {
            char *body = strstr(buffer, "\r\n\r\n");
            if (body) {
                body += 4;
                fwrite(body, 1, n - (body - buffer), fp);
                header_done = 1;
            }
        } else {
            fwrite(buffer, 1, n, fp);
        }
    }

    fclose(fp);
    close(sock);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <paquete>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "vim") == 0) {
        printf("Descargando vim...\n");
        return descargar("example.com", "binarios/vim.bin", "vim");
    }

    if (strcmp(argv[1], "nano") == 0) {
        printf("Descargando nano...\n");
        return descargar("example.com", "binarios/nano.bin", "nano");
    }

    printf("Paquete desconocido.\n");
    return 1;
}

