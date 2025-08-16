#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int parseError = 0;

// Forward declarations
double parseExpression(const char **str);
double parseTerm(const char **str);
double parseFactor(const char **str);

void skipSpaces(const char **str) {
    while (**str == ' ') (*str)++;
}

double parseFactor(const char **str) {
    skipSpaces(str);

    double result = 0.0;
    if (**str == '(') {
        (*str)++;
        result = parseExpression(str);
        if (**str == ')') {
            (*str)++;
        } else {
            parseError = 1;
        }
    } else if (isdigit(**str) || **str == '.') {
        char *end;
        result = strtod(*str, &end);
        if (*str == end) parseError = 1;
        *str = end;
    } else {
        parseError = 1;
    }

    skipSpaces(str);
    return result;
}

double parseTerm(const char **str) {
    double result = parseFactor(str);
    while (**str == '*' || **str == '/') {
        char op = *(*str)++;
        double rhs = parseFactor(str);
        if (parseError) return 0;
        if (op == '*') result *= rhs;
        else if (op == '/') {
            if (rhs == 0) {
                parseError = 1;
                return 0;
            }
            result /= rhs;
        }
    }
    return result;
}

double parseExpression(const char **str) {
    double result = parseTerm(str);
    while (**str == '+' || **str == '-') {
        char op = *(*str)++;
        double rhs = parseTerm(str);
        if (parseError) return 0;
        if (op == '+') result += rhs;
        else result -= rhs;
    }
    return result;
}

int main() {
    char *query = getenv("QUERY_STRING");
    char expr[256] = {0};
    char output[256] = {0};
    int isError = 0;  // flag for CSS class

    if (query && sscanf(query, "expression=%255s", expr) == 1) {
        // Decode %2B into '+'
        for (int i = 0; expr[i]; i++) {
            if (expr[i] == '%' && expr[i+1]=='2' && (expr[i+2]=='B' || expr[i+2]=='b')) {
                expr[i] = '+';
                memmove(&expr[i+1], &expr[i+3], strlen(&expr[i+3])+1);
            }
        }

        const char *p = expr;
        double result = parseExpression(&p);
        skipSpaces(&p);

        if (parseError || *p != '\0') {
            snprintf(output, sizeof(output), "Error!");
            isError = 1;
        } else {
            // If integer, show without decimals
            if (fabs(result - (int)result) < 1e-9) {
                snprintf(output, sizeof(output), "%d", (int)result);
            } else {
                // Trim unnecessary trailing zeros
                char buf[64];
                snprintf(buf, sizeof(buf), "%.6f", result);
                char *dot = strchr(buf, '.');
                if (dot) {
                    char *end = buf + strlen(buf) - 1;
                    while (end > dot && *end == '0') *end-- = '\0';
                    if (*end == '.') *end = '\0';
                }
                strncpy(output, buf, sizeof(output));
            }
        }
    }

    // HTML Output
    printf("Content-type:text/html\n\n");
    printf("<!DOCTYPE html>");
    printf("<html lang='en'><head><meta charset='UTF-8'><title>Calculator</title>");
    printf("<link rel='stylesheet' href='/style.css'>");
    printf("<style>.error{color:red;}</style>");
    printf("</head><body>");
    printf("<div class='calculator'>");
    printf("<h1>Calculator</h1>");
    printf("<form action='/cgi-bin/calc.cgi' method='get' id='calcForm'>");

    // Display box
    printf("<input type='text' name='expression' id='display' value='%s' readonly class='%s'>",
           output, isError ? "error" : "");

    // Buttons
    printf("<div class='buttons'>");
    char *btns[] = {"7","8","9","/","4","5","6","*","1","2","3","-","0",".","C","+"};
    for (int i = 0; i < 16; i++) {
        if (strcmp(btns[i],"C")==0)
            printf("<button type='reset' onclick=\"clearDisplay()\">C</button>");
        else
            printf("<button type='button' onclick=\"append('%s')\">%s</button>", btns[i], btns[i]);
    }
    printf("<button type='submit' class='equals'>=</button>");
    printf("</div></form></div>");
    printf("<script>");
    printf("function append(v){document.getElementById('display').value+=v;}");
    printf("function clearDisplay(){document.getElementById('display').value='';}");
    printf("</script>");
    printf("</body></html>");
    return 0;
}
