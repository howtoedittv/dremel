#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid < 0) exit(1);
    if (pid > 0) exit(0);

    
    if (setsid() < 0) exit(1);

    
    pid = fork();
    if (pid < 0) exit(1);
    if (pid > 0) exit(0);

    
    chdir("/");

    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    
    open("/dev/null", O_RDONLY); 
    open("/dev/null", O_WRONLY); 
    open("/dev/null", O_WRONLY);

    
    execl("/home/barry.local/bin/cdremel", "cdremel", NULL);

    
    exit(1);
}

