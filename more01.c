//
// Created by Kaleshwar Singh on 2019-05-01.
//

/*
 * more01.c - version 0.1 of more
 *      read and print 24 lines then pause for a few special commands.
 */

#include <stdio.h>
#include <stdlib.h>
#define PAGE_LEN 24
#define LINE_LEN 512

void do_more(FILE *);
int see_more();

int main(int argc, char* argv[]) {
    FILE* fp;
    if (argc == 1) {
        do_more(stdin);
    } else {
        while (--argc) {
            if ((fp = fopen(*++argv, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else {
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}
/*
 * Reads PAGE_LEN lines, then calls see_more() for further instructions.
 */
void do_more(FILE* fp) {
    char line[LINE_LEN];
    int num_of_lines = 0;
    int see_more(), reply;

    while (fgets(line, LINE_LEN, fp)) {     // More input
        if (num_of_lines == PAGE_LEN) {     // Full screen ?
            reply = see_more();             //  y: ask user
            if (reply == 0) {               //  n: done
                break;
            }
            num_of_lines -= reply;          // Reset count
        }
        if (fputs(line, stdout) == EOF) {   // Show line
            exit(EXIT_FAILURE);             // or die
        }
        num_of_lines++;                     // Count it
    }
}

/*
 * Prints message, waits for response.
 * Returns the number of lines to advance.
 * q                    - means no
 * space                - means yes (next screen)
 * carriage return      - means yes (one line)
 */
int see_more() {
    int c;
    printf("\033[7m more? \033[m");         // Reverse on a VT100
    while ((c = getchar()) != EOF) {        // Get response
        switch (c) {
            case 'q':                       // 'q'  -> no
                return 0;
            case ' ':                       // ' '  -> next page
                return PAGE_LEN;
            case '\n':                      // '\n' -> next line
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}