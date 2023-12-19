#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.c"

void mountWebDAV(const char* mountPoint, const char* username, const char* password, const char* webdavURL) {
    char command[256];
    makeConfigFile("/etc/davfs2/davfs2.conf");
    makeSecrets("/etc/davfs2/secrets", webdavURL, username, password);
    snprintf(command, sizeof(command), "mount -t davfs -o noexec,uid=%s,gid=%s %s %s",
             username, username, webdavURL, mountPoint);

    system(command);
    //printf("Will Run %s\n", command);
}

void unmountWebDAV(const char* mountPoint) {
    char command[256];
    makeConfigFile("/etc/davfs2/davfs2.conf");
    snprintf(command, sizeof(command), "umount %s", mountPoint);

    system(command);
    //printf("Will Run %s\n", command);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [mount|unmount] <mount_point> [username:password@webdav_url:port]\n", argv[0]);
        return 1;
    }

    const char* operation = argv[1];
    const char* mountPoint = argv[2];

    if (strcmp(operation, "mount") == 0) {
        if(argc < 4){
            fprintf(stderr, "Need to specify address\n");
            return 1;
        }
        const char* credentials = argv[3];
        char username[256], password[256], webdavURL[256];
        int port;

        if (sscanf(credentials, "%255[^:]:%255[^@]@%255[^:]:%d", username, password, webdavURL, &port) > 2) {
            // Mount WebDAV
            printf("Mounting WebDAV...\n");
            mountWebDAV(mountPoint, username, password, webdavURL);
            printf("WebDAV mounted at %s\n", mountPoint);
        } else {
            fprintf(stderr, "Invalid credentials format. Could not parse address.\n");
            return 1;
        }
    } else if (strcmp(operation, "unmount") == 0) {
        // Unmount WebDAV
        printf("Unmounting WebDAV...\n");
        unmountWebDAV(mountPoint);
        printf("WebDAV unmounted.\n");
    } else {
        fprintf(stderr, "Invalid command.\n");
        fprintf(stderr, "Usage: %s [mount|unmount] <mount_point> [username:password@webdav_url:port]\n", argv[0]);
        return 1;
    }

    return 0;
}
