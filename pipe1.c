// C Program that illustrate communication between two process using unnamed pipes#include <stdio.h>#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void server(int rfd, int wfd);
void client(int wfd, int rfd);

int main()
{
    int p1[2], p2[2];
    pid_t pid;

    pipe(p1);   // pipe1: client -> server
    pipe(p2);   // pipe2: server -> client

    pid = fork();

    if (pid == 0)
    {
        // Child process (Server)
        close(p1[1]);   // close write end of pipe1
        close(p2[0]);   // close read end of pipe2

        server(p1[0], p2[1]);
        exit(0);
    }
    else
    {
        // Parent process (Client)
        close(p1[0]);   // close read end of pipe1
        close(p2[1]);   // close write end of pipe2

        client(p1[1], p2[0]);
        wait(NULL);
    }

    return 0;
}

void server(int rfd, int wfd)
{
    char fname[100];
    char buffer[1000];
    int n, fd;

    // Read filename from pipe
    n = read(rfd, fname, sizeof(fname));
    fname[n] = '\0';

    fd = open(fname, O_RDONLY);

    if (fd < 0)
    {
        write(wfd, "File not found\n", 15);
    }
    else
    {
        n = read(fd, buffer, sizeof(buffer));
        write(wfd, buffer, n);
        close(fd);
    }
}

void client(int wfd, int rfd)
{
    char fname[100];
    char buffer[1000];
    int n;

    printf("Enter file name: ");
    scanf("%99s", fname);

    // Send filename to server
    write(wfd, fname, strlen(fname));

    // Read file contents from server
    n = read(rfd, buffer, sizeof(buffer));
    buffer[n] = '\0';

    printf("\nFile Content:\n%s\n", buffer);
}
