#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <errno.h>

int execvp(const char *file, char *const argv[]);
int setreuid(uid_t ruid, uid_t euid);

int recup (const char* name) {
    struct passwd pwd;
    struct passwd *result;
    char *buf;
    size_t bufsize;
    int s;

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)          /* Value was indeterminate */
        bufsize = 16384;        /* Should be more than enough */

    buf = malloc(bufsize);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    s = getpwnam_r(name, &pwd, buf, bufsize, &result);
    if (result == NULL) {
        if (s == 0)
            printf("L'utilisateur n'existe pas\n");
        else {
            errno = s;
            perror("getpwnam_r");
        }
        exit(EXIT_FAILURE);
    }
    return pwd.pw_uid;
}

int main(int argc, char** argv) {
    int i = 1;
    int posi = 0;
    char *newargv[] = { NULL, NULL };
    if (argc == 1) {
        printf("Baby shark est seul en mer\n");
        return 0;
    }
    else if (strcmp(argv[1], "--user") == 0 && argc == 2) {
        printf("Il manque l'user ainsi que la commande\n");
        return 0;
    }
    if (strcmp(argv[1], "--user") == 0) {
        i = 3;
        int bouh = recup(argv[2]);
        setreuid( 0, recup(argv[2]));
    }
    while (i < argc) {
        newargv[posi] = argv[i];
        posi = posi + 1;
        i = i + 1;   
    }
    if (strcmp(argv[1], "--user") == 0 && argc < 7 && argc > 3) {
        newargv[posi] = NULL;
        execvp(argv[3], newargv);
    } else if (strcmp(argv[1], "--user") == 0 && argc == 3) {
        printf("Il manque la commande\n", argv[2]);
        return 0;
    } else if (argc < 5 && argc > 1) {
        newargv[posi] = NULL;
        execvp(argv[1], newargv);
    } else {
        printf("Il y a trop d'arguments\n");
        return 0;
    }
    perror("execve");   /* execve() ne retourne qu'en cas d'erreur */
    exit(EXIT_FAILURE); 
    return 0;
}

