#include <stdio.h>
#include <stdlib.h> //EXIT_SUCCESS

#define BUF_SIZE (1024 * 4) //stdin buf

static void out_char(char ch);
static void fputs_iterator(const char *str, size_t count);

int main(int argc, char **argv) {
    
    char buffer[BUF_SIZE];
    const char spaces[] = "  ";
    unsigned int indent = 0;
    char is_str = 0;
    char esc = 0;

    // read stdin until EOF
    while (!feof(stdin)) {
        
        size_t n = fread(&buffer, sizeof(char), BUF_SIZE, stdin);

        if (n != BUF_SIZE && ferror(stdin)) {
            exit(EXIT_FAILURE);
        }

        for (size_t k = 0; k < n; k++) {
            char ch = buffer[k];

            if (is_str) { // inside quoted string
                out_char(ch);
                if (! esc) {
                    if (ch == '"') { // string just ended
                        is_str = 0;
                    } else if (ch == '\\') {
                        esc = 1;
                    }
                } else {
                    esc = 0;
                }
                continue;
            }
    
            switch (ch) {
    
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break; // ignore orig formatting
        
            case '{':
            case '[':
                out_char(ch);
                out_char('\n');
                indent++;
                fputs_iterator(spaces, indent);
                break;
    
            case '}':
            case ']':
                out_char('\n');
                indent--;
                fputs_iterator(spaces, indent);
                out_char(ch);
                if (indent == 0) out_char('\n');
                break;
                
            case ',':
                out_char(',');
                out_char('\n');
                fputs_iterator(spaces, indent);
                break;

            case ':':
                out_char(':');
                out_char(' ');
                break;

            case '"': // string/property key start, ref if statement at the top
                out_char('"');
                is_str = 1;
                break;
    
            default:
                out_char(ch); // nums, null, true, false
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

static void out_char(char ch) {
    if (EOF == putchar(ch)) {
        exit(EXIT_FAILURE);
    }
}

static void fputs_iterator(const char *str, size_t cnt) {
    while (cnt-- > 0) {
        int val = fputs(str, stdout);
        if (val == EOF) {
            exit(EXIT_FAILURE);
        }
    }
}
