#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 128
#define MAX_VALUE_LENGTH 128

typedef struct {
    int use_locks;
    int ask_auth;
    char servercert[MAX_VALUE_LENGTH];
    int cache_size;
    int use_proxy;
    char trust_ca_dir[MAX_VALUE_LENGTH];
    int ignore_dav_header;
} AppConfig;

// Function to skip leading whitespace in a string
void skipLeadingWhitespace(char **str) {
    while (**str != '\0' && isspace(**str)) {
        (*str)++;
    }
}


void makeSecrets(const char *filename, const char* prefix, const char* url_p, const char* userName, const char* pwd) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open the file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Open a temporary file for writing
    FILE *tempFile = fopen("_temp_WEBDEV_config.secrets", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Unable to open the temporary file for writing.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int configUpdated = 1;

    // Read each line of the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            fputs(line, tempFile);
            continue;
        }

        // Parse key-value pairs
        char prefix[32], url[MAX_KEY_LENGTH], username[MAX_VALUE_LENGTH], password[MAX_VALUE_LENGTH];
        if (sscanf(line, "%32[^:/]://%255s %127s %127[^\n]", prefix, url, username, password) == 4){
            //printf("Prefix: %s, url: %s, url_p: %s", prefix, url, url_p);
            // Compare keys and update corresponding values
            if (strcmp(url, url_p) == 0) {
            } else {
                // Copy unchanged lines
                fputs(line, tempFile);
            }
        } else {
            // Copy malformed lines
            fputs(line, tempFile);
        }
    }
    char formattedString[3*MAX_KEY_LENGTH];
    sprintf(formattedString, "%s://%s %s %s\n", prefix, url_p, userName, pwd);
    fputs(formattedString, tempFile);

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file if the configuration was updated
    if (configUpdated) {
        remove(filename);
        rename("_temp_WEBDEV_config.secrets", filename);
    } else {
        remove("_temp_WEBDEV_config.secrets");
    }
}

// Function to parse the configuration file
void parseConfigFile(const char *filename, AppConfig *config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open the file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];

    // Read each line of the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        // Parse key-value pairs
        char key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];
        if (sscanf(line, "%127s %127[^\n]", key, value) == 2) {
            // Skip leading whitespace in the value
            char *valuePtr = value;
            skipLeadingWhitespace(&valuePtr);

            // Compare keys and set corresponding values
            if (strcmp(key, "use_locks") == 0) {
                config->use_locks = atoi(valuePtr);
            } else if (strcmp(key, "ask_auth") == 0) {
                config->ask_auth = atoi(valuePtr);
            } else if (strcmp(key, "servercert") == 0) {
                strcpy(config->servercert, valuePtr);
            } else if (strcmp(key, "cache_size") == 0) {
                config->cache_size = atoi(valuePtr);
            } else if (strcmp(key, "use_proxy") == 0) {
                config->use_proxy = atoi(valuePtr);
            } else if (strcmp(key, "trust_ca_dir") == 0) {
                strcpy(config->trust_ca_dir, valuePtr);
            } else if (strcmp(key, "ignore_dav_header") == 0) {
                config->ignore_dav_header = atoi(valuePtr);
            }
        }
    }

    fclose(file);
}

void makeConfigFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open the file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Open a temporary file for writing
    FILE *tempFile = fopen("_temp_WEBDAV_config.conf", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Unable to open the temporary file for writing.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int configUpdated = 1;

    // Read each line of the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            fputs(line, tempFile);
            continue;
        }

        // Parse key-value pairs
        char key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];
        if (sscanf(line, "%127s %127[^\n]", key, value) == 2) {
            // Skip leading whitespace in the value
            char *valuePtr = value;
            skipLeadingWhitespace(&valuePtr);

            // Compare keys and update corresponding values
            if (strcmp(key, "use_locks") == 0) {
            } else if (strcmp(key, "ask_auth") == 0) {
            } else if (strcmp(key, "servercert") == 0) {
            } else if (strcmp(key, "cache_size") == 0) {
            } else if (strcmp(key, "use_proxy") == 0) {
            } else if (strcmp(key, "trust_ca_dir") == 0) {
            } else if (strcmp(key, "ignore_dav_header") == 0) {
            } else {
                // Copy unchanged lines
                fputs(line, tempFile);
            }
        } else {
            // Copy malformed lines
            fputs(line, tempFile);
        }
    }
    //fputs("use_locks       0\nask_auth        0\nservercert      /etc/davfs2/certs/server.crt\ncache_size      5000\nuse_proxy       0\ntrust_ca_cert    /etc/ssl/certs\nignore_dav_header 1\n", tempFile);
    fputs("use_locks       0\nuse_proxy       0\nignore_dav_header 1\n", tempFile);

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file if the configuration was updated
    if (configUpdated) {
        remove(filename);
        rename("_temp_WEBDAV_config.conf", filename);
    } else {
        remove("_temp_WEBDAV_config.conf");
    }
}

// Function to write the updated configuration to the file
void writeConfigFile(const char *filename, const AppConfig *config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open the file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Open a temporary file for writing
    FILE *tempFile = fopen("_temp_WEBDAV_config.conf", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Unable to open the temporary file for writing.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int configUpdated = 0;

    // Read each line of the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            fputs(line, tempFile);
            continue;
        }

        // Parse key-value pairs
        char key[MAX_KEY_LENGTH], value[MAX_VALUE_LENGTH];
        if (sscanf(line, "%127s %127[^\n]", key, value) == 2) {
            // Skip leading whitespace in the value
            char *valuePtr = value;
            skipLeadingWhitespace(&valuePtr);

            // Compare keys and update corresponding values
            if (strcmp(key, "use_locks") == 0) {
                fprintf(tempFile, "%s %d\n", key, config->use_locks);
                configUpdated = 1;
            } else if (strcmp(key, "ask_auth") == 0) {
                fprintf(tempFile, "%s %d\n", key, config->ask_auth);
                configUpdated = 1;
            } else if (strcmp(key, "servercert") == 0) {
                fprintf(tempFile, "%s %s\n", key, config->servercert);
                configUpdated = 1;
            } else if (strcmp(key, "cache_size") == 0) {
                fprintf(tempFile, "%s %d\n", key, config->cache_size);
                configUpdated = 1;
            } else if (strcmp(key, "use_proxy") == 0) {
                fprintf(tempFile, "%s %d\n", key, config->use_proxy);
                configUpdated = 1;
            } else if (strcmp(key, "trust_ca_dir") == 0) {
                fprintf(tempFile, "%s %s\n", key, config->trust_ca_dir);
                configUpdated = 1;
            } else if (strcmp(key, "ignore_dav_header") == 0) {
                fprintf(tempFile, "%s %d\n", key, config->ignore_dav_header);
                configUpdated = 1;
            } else {
                // Copy unchanged lines
                fputs(line, tempFile);
            }
        } else {
            // Copy malformed lines
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file if the configuration was updated
    if (configUpdated) {
        remove(filename);
        rename("_temp_WEBDAV_config.conf", filename);
    } else {
        remove("_temp_WEBDAV_config.conf");
    }
}

int _main() {
    makeSecrets("myconf.conf", "http", "test.com", "test@test.com", "abcdef");
    exit(0);
    AppConfig config;

    // Provide the path to your configuration file
    parseConfigFile("temp/your_config.conf", &config);

    // Print parsed configuration
    printf("use_locks: %d\n", config.use_locks);
    printf("ask_auth: %d\n", config.ask_auth);
    printf("servercert: %s\n", config.servercert);
    printf("cache_size: %d\n", config.cache_size);
    printf("use_proxy: %d\n", config.use_proxy);
    printf("trust_ca_dir: %s\n", config.trust_ca_dir);
    printf("ignore_dav_header: %d\n", config.ignore_dav_header);

    // Update the configuration in memory (replace with your logic)
    config.use_locks = 1;
    config.cache_size = 10000;

    // Write the updated configuration back to the file
    writeConfigFile("temp/your_config.conf", &config);

    return 0;
}
