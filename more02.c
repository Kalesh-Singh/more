//
// Created by Kaleshwar Singh on 2019-05-01.
//

/*
 * more02.c - version 0.2 of more
 *      read and print 24 lines then pause for a few special commands.
 *      feature of version 0.2: reads from /dev/tty for commands.
 */

#include <stdio.h>
#include <stdlib.h>
#define PAGE_LEN 24
#define LINE_LEN 512
#define CONTROLLING_TERMINAL "/dev/tty"

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
    FILE *fp_tty;

    fp_tty = fopen(CONTROLLING_TERMINAL, "r");  // NEW: cmd stream
    if (fp_tty == NULL) {                       // If open fails
        exit(EXIT_FAILURE);                     //      no use in running
    }
    while (fgets(line, LINE_LEN, fp)) {         // More input
        if (num_of_lines == PAGE_LEN) {         // Full screen ?
            reply = see_more(fp_tty);           // NEW: Pass command stream
            if (reply == 0) {                   // n: done
                break;
            }
            num_of_lines -= reply;              // Reset count
        }
        if (fputs(line, stdout) == EOF) {       // Show line
            exit(EXIT_FAILURE);                 // or die
        }
        num_of_lines++;                         // Count it
    }
}

/*
 * Prints message, waits for response.
 * Expects command stream as argument.
 * Returns the number of lines to advance.
 * q                    - means no
 * space                - means yes (next screen)
 * carriage return      - means yes (one line)
 */
int see_more(FILE* cmd) {                       // NEW: Takes arg
    int c;
    printf("\033[7m more? \033[m");             // Reverse on a VT100
    while ((c = getc(cmd)) != EOF) {            // NEW: Reads from /dev/tty
        switch (c) {
            case 'q':                           // 'q'  -> no
                return 0;
            case ' ':                           // ' '  -> next page
                return PAGE_LEN;
            case '\n':                          // '\n' -> next line
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}