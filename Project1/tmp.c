#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int tmp() {
    FILE* in = fopen("in.txt", "r");
    if (in == NULL)
    {
        printf("No open file");
        return 1;
    }
    int n;
    fscanf(in, "%d\n", &n);
    int s_n = 2 * n;
    char* s = (char*)malloc(sizeof(char) * s_n);
    if (s == NULL) exit(1);
    int a = ftell(in);
    //fscanf(in, "%s", s);
    fgets(s, s_n, in);
    printf("%s", s);
    fclose(in);
    free(s);

    return 0;
}