#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define SOCK "/tmp/mpvsocket"
#define MAX_APPS 50

const char *default_apps[] = {
    "firefox", "chrome", "chromium", "spotify", "brave",
    "org.mozilla.firefox", "com.brave.Browser", "org.videolan.VLC", "com.spotify.Client"
};
const int num_default = 9;

int already_in_list(const char *app, const char *apps[], int num_apps) {
    for(int i=0;i<num_apps;i++) if(strcmp(apps[i], app)==0) return 1;
    return 0;
}

int load_exclusions(const char *file, const char *apps[], int *num_apps) {
    FILE *f = fopen(file, "r");
    if(!f) return 0;
    char line[128];
    while(fgets(line,sizeof(line),f)) {
        line[strcspn(line,"\n")] = 0;
        if(*line && !already_in_list(line, apps, *num_apps)) apps[(*num_apps)++] = strdup(line);
    }
    fclose(f);
    return 1;
}

void save_exclusions(const char *file, const char *apps[], int num_apps) {
    FILE *f = fopen(file,"w");
    if(!f) return;
    for(int i=0;i<num_apps;i++) fprintf(f,"%s\n", apps[i]);
    fclose(f);
}

int remove_from_list(const char *app, const char *apps[], int *num_apps) {
    for(int i=0;i<*num_apps;i++) {
        if(strcmp(apps[i], app)==0) {
            for(int j=i;j<*num_apps-1;j++) apps[j]=apps[j+1];
            (*num_apps)--;
            return 1;
        }
    }
    return 0;
}

void print_help() {
    printf("Usage: cdremel [options]\n");
    printf("Options:\n");
    printf("  --add-exclusion <app>      Add app to exclusion list\n");
    printf("  --remove-exclusion <app>   Remove app from exclusion list\n");
    printf("  -i                          Ignore all apps and play continuously\n");
    printf("  --help                      Show this help message\n");
    printf("  init                        Install dependencies and create music directory\n");
}

void print_man() {
    printf(".TH dem 1 \"2026-02-15\" \"Deck Music Controller\"\n");
    printf(".SH NAME\n");
    printf("cdremel - Desktop background music manager with automatic pause/resume\n");
    printf(".SH SYNOPSIS\n");
    printf("dem [options]\n");
    printf(".SH DESCRIPTION\n");
    printf("Plays background music and pauses when configured apps are running.\n");
    printf(".SH OPTIONS\n");
    printf("--add-exclusion <app>        Add app to exclusion list\n");
    printf("--remove-exclusion <app>     Remove app from exclusion list\n");
    printf("-i                            Ignore apps and play continuously\n");
    printf("--help                        Show help\n");
    printf("init                          Install dependencies and setup directory\n");
}

int check_brew() {
    if(system("brew --version >/dev/null 2>&1") != 0) {
        printf("Homebrew not found. Installing...\n");
        system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"");
    } else {
        printf("Homebrew is already installed.\n");
    }
    return 0;
}

int main(int argc, char *argv[]) {
    const char *apps[MAX_APPS];
    int num_apps = 0;
    int flags_only = 0;
    int ignore_apps = 0;

    char music_dir[512];
    char exclusions_file[512];
    snprintf(music_dir,sizeof(music_dir),"%s/Music/background",getenv("HOME"));
    snprintf(exclusions_file,sizeof(exclusions_file),"%s/.deck_exclusions.txt",getenv("HOME"));

    if(argc>1 && strcmp(argv[1],"init")==0) {
        printf("Initializing...\n");
        check_brew();
        system("brew install mpv");
        mkdir(music_dir,0755);
        printf("Initialization done. Music directory created at %s\n", music_dir);
        return 0;
    }

    if(argc>1 && strcmp(argv[1],"--help")==0) {
        print_help();
        return 0;
    }

    for(int i=0;i<num_default;i++) apps[num_apps++] = default_apps[i];
    load_exclusions(exclusions_file, apps, &num_apps);

    for(int i=1;i<argc;i++) {
        if(strcmp(argv[i], "-i")==0) ignore_apps = 1;
        if(strncmp(argv[i], "--",2)==0) flags_only = 1;

        if(strcmp(argv[i],"--add-exclusion")==0){
            if(i+1>=argc){fprintf(stderr,"Error: --add-exclusion requires a value\n");return 1;}
            const char *app = argv[++i];
            if(already_in_list(app, apps, num_apps)) printf("App \"%s\" is already configured\n", app);
            else { apps[num_apps++] = strdup(app); printf("App \"%s\" added successfully\n", app);}
        } else if(strcmp(argv[i],"--remove-exclusion")==0){
            if(i+1>=argc){fprintf(stderr,"Error: --remove-exclusion requires a value\n");return 1;}
            const char *app = argv[++i];
            if(remove_from_list(app, apps, &num_apps)) { printf("App \"%s\" successfully removed\n", app); }
            else printf("App \"%s\" was not found in the list\n", app);
        }
    }

    save_exclusions(exclusions_file, apps, num_apps);
    if(flags_only) return 0;

    pid_t pid = fork();
    if(pid==0){
        execlp("mpv","mpv","--no-video","--loop-playlist=inf",music_dir,"--input-ipc-server="SOCK,NULL);
        perror("Failed to start mpv");
        exit(1);
    }

    int state=1;
    while(1){
        int other_running=0;
        if(!ignore_apps){
            for(int i=0;i<num_apps;i++){
                char cmd[256];
                snprintf(cmd,sizeof(cmd),"pgrep -x '%s' >/dev/null 2>&1", apps[i]);
                if(system(cmd)==0){other_running=1;break;}
            }
        }

        if(other_running && state){
            FILE *sock = popen("socat - "SOCK,"w");
            if(sock){fprintf(sock,"{ \"command\": [\"set_property\", \"pause\", true] }\n");pclose(sock);}
            state=0;
            printf("Music paused (other app running)\n");
        } else if(!other_running && !state){
            FILE *sock = popen("socat - "SOCK,"w");
            if(sock){fprintf(sock,"{ \"command\": [\"set_property\", \"pause\", false] }\n");pclose(sock);}
            state=1;
            printf("Music resumed (no other app)\n");
        }
        sleep(1);
    }

    return 0;
}

