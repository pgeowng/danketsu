
#include <stdio.h>





int main(int argc, char* argv[]) {


    FILE *file = fopen("assets/icosphere.obj", "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 0;
    }

    printf("File opened\n");




    int i = 0;
    char ch;

    while (ch != EOF) {
        char buf[16];
        int n = fscanf(file, "%s", &buf);
        if (n == EOF) {
            printf("fscanf EOF exiting....\n");
            break;
        }
        printf("n: %d\n", n);

        printf("buf: %s\n", buf);
        // (ch = fgetc(file)) != EOF;

        bool next_line = false;

        switch (buf[0]) {
            case '#':
                printf("comment\n");
                next_line = true;
                break;
            case 'o':
                printf("object\n");
                next_line = true;
                break;
            case 'v':
                if (buf[1] == '\0') {
                    float x, y, z;
                    fscanf(file, "%f %f %f", &x, &y, &z);
                    printf("vertex: %f %f %f\n", x, y ,z);
                    next_line = true;
                } else if (buf[1] == 't') {
                    float u, v;
                    fscanf(file, "%f %f", &u, &v);
                    printf("uv: %f %f\n", u, v);
                    next_line = true;
                } else if (buf[1] == 'n') {
                    float x, y, z;
                    fscanf(file, "%f %f %f", &x, &y, &z);
                    printf("normal: %f %f %f\n", x, y ,z);
                    next_line = true;
                }
                break;
            case 'f':
                unsigned int v1, v2, v3;
                unsigned int vt1, vt2, vt3;
                unsigned int vn1, vn2, vn3;
                fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

                printf("face: %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3);
                next_line = true;
                break;
            default:
                printf("default\n");
                break;
        }

        if (next_line) {
            while ((ch = fgetc(file)) != '\n') {
                if (ch == EOF) {
                    printf("EOF\n");
                    break;
                }
            }
        }

        i++;
    }

    fclose(file);


    return 0;
}