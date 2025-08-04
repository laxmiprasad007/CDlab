#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char keywords[][10] = {
    "int", "float", "char", "return", "if", "else", "while", "for", "void"
};

int isKeyword(char *word) {
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

void analyze(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: cannot open file.\n");
        return;
    }
    char ch, buffer[100];
    int i = 0;
    printf("Tokens:\n");
    while ((ch = fgetc(fp)) != EOF) {
        // Identifiers or Keywords
        if (isalpha(ch) || ch == '_') {
            buffer[i++] = ch;
            while (isalnum(ch = fgetc(fp)) || ch == '_')
                buffer[i++] = ch;
            buffer[i] = '\0';
            i = 0;
            ungetc(ch, fp); // put back non-word char

            if (isKeyword(buffer))
                printf("[Keyword] %s\n", buffer);
            else
                printf("[Identifier] %s\n", buffer);
        }
        // Numbers
        else if (isdigit(ch)) {
            buffer[i++] = ch;
            while (isdigit(ch = fgetc(fp)))
                buffer[i++] = ch;
            buffer[i] = '\0';
            i = 0;
            ungetc(ch, fp);
            printf("[Number] %s\n", buffer);
        }
        // Operators / Symbols
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || 
                ch == '(' || ch == ')' || ch == '{' || ch == '}') { 
            printf("[Symbol] %c\n", ch); 
        } 
        // Skip spaces, newlines, tabs 
        else if (ch == ' ' || ch == '\n' || ch == '\t') { 
            continue; 
        } 
        // Skip comments (// style) 
        else if (ch == '/') { 
            char next = fgetc(fp); 
            if (next == '/') { 
                while ((ch = fgetc(fp)) != '\n' && ch != EOF); // skip to end of line 
            } else { 
                ungetc(next, fp); 
                printf("[Symbol] /\n"); 
            } 
        } 
    } 
    fclose(fp); 
} 

int main() { 
    char filename[100]; 
    printf("Enter filename to analyze: "); 
    scanf("%s", filename); 
    analyze(filename); 
    return 0; 
}
