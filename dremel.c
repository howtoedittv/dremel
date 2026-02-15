#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_help() {
    printf("Usage: dremel [options]\n");
    printf("Options:\n");
    printf("  --help       Show this help message\n");
    printf("  --kill       Stop all mpv instances\n");
}

int main(int argc, char *argv[]) {
    if(argc > 1) {
        if(strcmp(argv[1], "--help") == 0) {
            print_help();
            return 0;
        }
        if(strcmp(argv[1], "--kill") == 0) {
            printf("Stopping all mpv instances...\n");
            system("pkill mpv");
            return 0;
        }
    }

    pid_t pid = fork();
    if(pid < 0) {
        perror("fork failed");
        return 1;
    }
    if(pid > 0) {
        return 0;
    }

    
    char cdremel_path[512];
    snprintf(cdremel_path, sizeof(cdremel_path), "%s/.local/bin/cdremel", getenv("HOME"));
    execl(cdremel_path, "cdremel", NULL);
    perror("Failed to start cdremel");
    return 1;
}

