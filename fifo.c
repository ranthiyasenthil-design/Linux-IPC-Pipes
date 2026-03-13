#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "myfifo"

void server();
void client();

int main()
{
    pid_t pid;

    // Create FIFO
    mkfifo(FIFO_FILE, 0666);

    pid = fork();

    if(pid > 0)
    {
        // Parent → Server
        sleep(1);
        server();
    }
    else if(pid == 0)
    {
        // Child → Client
        client();
    }
    else
    {
        printf("Fork failed\n");
        exit(1);
    }

    return 0;
}

void server()
{
    int fd, file;
    char buffer[1000];
    int n;

    fd = open(FIFO_FILE, O_WRONLY);
    file = open("hello.txt", O_RDONLY);

    if(file < 0)
    {
        write(fd, "File not found\n", 15);
    }
    else
    {
        n = read(file, buffer, sizeof(buffer));
        write(fd, buffer, n);
        close(file);
    }

    close(fd);
}

void client()
{
    int fd;
    char buffer[1000];
    int n;

    fd = open(FIFO_FILE, O_RDONLY);

    n = read(fd, buffer, sizeof(buffer));
    buffer[n] = '\0';

    printf("File Content:\n%s\n", buffer);

    close(fd);
}
