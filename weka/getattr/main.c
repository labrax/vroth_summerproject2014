/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse(FILE * input, FILE * output); //parse whole file
void parse_line(char * str, char * dest);

char * str_beg; //identifiers
char * str_end;

int main(int argc, char ** argv) {
    if(argc < 4) { //check if the parameters are in the right number
        fprintf(stderr, "\
this program will extract the attrs from a file\n\
run the program as:\n./getattr <input> <transaction identifier> <item identifier>\n\
the transaction id must be for example \"RGD:\" or \"MGI:\"\n\
the item identifier muser be for example \"MP:\"\n\
");
        exit(-1);
    }

    FILE * input = fopen(argv[1], "r"); //check the parameters
    if(!input) {
        fprintf(stderr, "problem on input file");
        exit(-1);
    }
/*    FILE * output = fopen(argv[2], "w"); //in case of using an output file -- REMOVED
    if(!output) {
        fprintf(stderr, "problem on output file");
        exit(-1);
    }*/
    str_beg = argv[2];
    str_end = argv[3];
//    printf("using identifiers: \"%s\" and \"%s\"\n", str_beg, str_end);

    parse(input, stdout);

    fclose(input);
    //fclose(output);

    return 0;
}

void parse(FILE * input, FILE * output) {
    char buffer[128];
    char dest[64];
    while(!feof(input)) {
        dest[0] = '\0';
        fscanf(input, "%[^\n]\n", buffer);
        parse_line(buffer, dest);
        if(dest[0] == '\0') {
            fprintf(stderr, "no information for: %s\n", buffer);
        }
        else {
            fprintf(output, "%s\n", dest); //
//            fprintf(output, "@ATTRIBUTE %s {0, 1}\n", dest);
        }
    }
    return;
}

void parse_line(char * str, char * dest) {
    char firstpart[16], secondpart[16];
    if(strstr(str, str_beg) && strstr(str, str_end)) { //if line has both identifiers, continue
        sscanf(str, "%s %s", firstpart, secondpart);

        /*int i;
        for(i = 0; i < strlen(secondpart); i++) {
            if(secondpart[i] == ':') {
                secondpart[i] = '_';
                break;
            }
        }*/
        strcpy(dest, secondpart);
    }
    else {
        fprintf(stderr, "tag missing for %s\n", str);
    }

    return;
}

