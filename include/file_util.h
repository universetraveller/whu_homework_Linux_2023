#include <stdio.h>

#include <stdlib.h>

char* readFile(const char *filename) {
    FILE *file;

    // Open the file for reading
    file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("Error opening the file.\n");
        return NULL; // Return NULL to indicate failure
    }

    // Seek to the end of the file to determine its size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to store the file contents
    char *fileContents = (char *)malloc(fileSize + 1);

    // Read the file contents into the allocated memory
    if (fileContents) {
        fread(fileContents, 1, fileSize, file);
        fileContents[fileSize] = '\0'; // Null-terminate the string
    }

    // Close the file
    fclose(file);

    return fileContents; // Return the file contents as a string
}

int write_to_file(const char *filename, const char *data) {
    FILE *file;

    // Open the file for writing ("w" mode creates a new file or truncates an existing one)
    file = fopen(filename, "w");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1; // Return an error code
    }

    // Write data to the file using fprintf()
    fprintf(file, "%s\n", data);

    // Close the file when done
    fclose(file);

    return 0; // Return 0 to indicate success
}
