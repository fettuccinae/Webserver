#include <stdio.h>
#include <stdlib.h>

int main() {
    // Open the input file
    FILE *file = fopen("head.html", "rb");

    fseek(file, 0, SEEK_END);
    long fSize = ftell(file);
    rewind(file);
    char *data = malloc(fSize + 1); // +1 for null-terminator
    size_t bytesRead = fread(data, 1, fSize, file);
    if (bytesRead != fSize) {
        perror("Error reading file");
        free(data);
        fclose(file);
        return 1;
    }

    data[bytesRead] = '\0'; // Null-terminate the string
    fclose(file);

    // Open the output file
    FILE *f2 = fopen("new.txt", "wb");
    if (f2 == NULL) {
        perror("Failed to open output file");
        free(data);
        return 1;
    }

    // Write to the output file
    fwrite(data, sizeof(char), bytesRead, f2);

    // Clean up
    fclose(f2);
    free(data);

    printf("File copied successfully.\n");
    return 0;
}
