#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_input(int ch);
void format_input(int ch);
void print_ch(int ch);
void indent_ln(void);

typedef enum {TRUE = 1, FALSE = 0} bool;

int lv = 0, indent = 4;
bool esc = FALSE, internal = FALSE, nl = FALSE;

int main(int argc, char *argv[]) {
    
    if (argc == 2 && ((strcmp(argv[1], "-h") == 0) || strcmp(argv[1], "--help") == 0)) {
        printf("usage: cat file.json | jp\n\twget -O - url | jp | less\n\tjpretty < file.json\n");
        printf("options: jp -t [number]\n\t jp --indent [number]\n");
        exit(0);
    }

    int ch;
    if (argc > 2 && ((strcmp(argv[1], "-t") == 0) || strcmp(argv[1], "--indent") == 0)) {
        ch = atoi(argv[2]);
        if (ch > 0) 
            indent = ch;
    }

    while ((ch = fgetc(stdin)) != EOF) {
        if (ch != '\r' && ch != '\n') 
            parse_input(ch);
    }
    printf("\n");
    return 0;
}

void parse_input(int ch) {
    if (ch == '"' && esc == FALSE) {
        format_input(ch);
        internal = (internal == TRUE) ? FALSE : TRUE;
    } 
    else if (internal == FALSE) {
        if (ch != ' ' && ch != '\t') 
            format_input(ch);    
    }    
    else 
        printf("%c", ch);

    if (ch == '\\') 
        esc = (esc == TRUE) ? FALSE : TRUE;
    else 
        esc = FALSE;
}

void format_input(int ch) {
    if (ch == '{' || ch == '[') {
        print_ch(ch);
        lv++;
        nl = TRUE;
    } 
    else if (ch == '}' || ch == ']') {
        if (lv > 0) 
            lv--;

        indent_ln();
        printf("%c", ch);
        nl = TRUE;
    } 
    else if (ch == ',') {
        printf("%c", ch);
        nl = TRUE;
    } 
    else if (ch == ':') 
        printf(" %c ", ch);

    else 
        print_ch(ch);
}

void print_ch(int ch) {
    if (nl) {
        indent_ln();
        nl = FALSE;
    }
    printf("%c", ch);
}

void indent_ln() {
    printf("\n");
    for (int i = 0; i < lv; i++) {
        for (int j = 0; j < indent; j++) 
            printf(" ");
    }
}