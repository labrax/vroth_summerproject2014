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

#define TRANS_THRESHOLD 1 //threshold for minimum amount of itens in a transaction to consider it - USE 1 to avoid prunning

typedef struct _value { //store relation between item and index
    unsigned int index;
    unsigned int value;
} value;

void prepare_values(FILE * fp, value * data); //read values from attr file and prepare struct
void print_values(value * data); //print struct, debug function

void create_arff(FILE * input, FILE * output, value * data); //create arff file
void process_transaction(FILE * input, FILE * output, value * data); //process a transaction
void read_line(char * line, unsigned int * trans, unsigned int * item_value); //process a line
unsigned int item_index(value * data, unsigned int value); //return the index from a item value

char * trans_id; //identifiers
char * item_id;

int main(int argc, char ** argv) {
    if(argc < 6) { //check if the parameters are in right number
        fprintf(stderr, "\
this program will get a transaction file and attr file to prepare an arff file for weka\
run as:\n\
./create_arff <transaction file> <attr file> <output file> <transaction identifier> <item identifier>\n\
");
        exit(-1);
    }

    FILE * transaction_input = fopen(argv[1], "r"); //check the parameters
    if(!transaction_input) {
        fprintf(stderr, "failed to open %s\n", argv[1]);
        exit(-1);
    }
    FILE * attr_input = fopen(argv[2], "r");
    if(!attr_input) {
        fprintf(stderr, "failed to open %s\n", argv[2]);
        exit(-1);
    }
    FILE * output = fopen(argv[3], "w");
    if(!output) {
        fprintf(stderr, "failed to open output file %s\n", argv[3]);
        exit(-1);
    }
    trans_id = argv[4];
    item_id = argv[5];

    unsigned int lines = 0; //count amount of lines for allocation
    while (EOF != (fscanf(attr_input, "%*[^\n]"), fscanf(attr_input, "%*c")))
        ++lines;

    fseek(attr_input, 0, SEEK_SET); //return to beginning of file

    value * data = malloc(sizeof(value) * lines); //allocate memory and reset values
    memset(data, 0, sizeof(value) * lines);

    prepare_values(attr_input, data); //read attr file
//    print_values(data); //debug purpouses

    create_arff(transaction_input, output, data); //<-- most important function

    free(data); //deallocate memory

    return 0;
}

void prepare_values(FILE * fp, value * data) {
    char buffer[64];
    int now = 0;
    while(!feof(fp)) {
        fscanf(fp, "%[^\n]\n", buffer);
        if(strstr(buffer, item_id)) {
            data[now].index = now;
            data[now].value = atoi(&buffer[strlen(item_id)]);
            now++;
        }
    }
    return;
}

void print_values(value * data) {
    int i;
    for(i=0; data[i].value != 0; i++) {
        printf("%d %d\n", data[i].index, data[i].value);
    }
    return;
}

void create_arff(FILE * input, FILE * output, value * data) {
    fprintf(output, "%%file generated using createarff\n\
%%create by Victor R. Cardoso - CNPq\n\
%%on summer project supervisioned by Robert Hoehndorf and Georgios Gkoutos\n\
%%at Aberystwyth University\n\n");
    fprintf(output, "@relation relation_transaction_item\n\n");

    fprintf(output, "%%attrs obtained:\n");

    unsigned int i;
    for(i=0; i<strlen(item_id); i++) //fix ':' on identifier to put on arff file
        if(item_id[i] == ':') {
            item_id[i] = '_';
            break;
        }

    for(i=0; data[i].value != 0; i++) {
        fprintf(output, "@attribute %s%010d {0, 1}\n", item_id, data[i].value);
    }

    for(i=0; i<strlen(item_id); i++) //return '_' to previous state
        if(item_id[i] == '_') {
            item_id[i] = ':';
            break;
        }

    fprintf(output, "\n%%transactions found:\n@data\n");

    while(!feof(input)) {
        process_transaction(input, output, data);
    }

    return;
}

void process_transaction(FILE * input, FILE * output, value * data) {
    fpos_t pos_before;
    char line[256];

    char output_line[256]="";

    unsigned int trans_ret=0;
    unsigned int item_value=0;

    unsigned int trans_now=0;
    unsigned int trans_v_count=0;

    while(!feof(input)) {
        fscanf(input, "%[^\n]\n", line);

        read_line(line, &trans_ret, &item_value);

        if(item_value == 0) {
            fprintf(stderr, "item with value 0 returned, ignoring\n");
            continue;
        }

        if(trans_now == 0) {
            trans_now = trans_ret;
        }

        if(trans_v_count == 0) {
            strcat(output_line, "{");
            //fprintf(output, "{");
            //fprintf(output, "%d: {", trans_now); //debug ;)
        }

        if(trans_ret != trans_now) { //is starting new transaction, stop here
            strcat(output_line, "}\n");
            if(trans_v_count >= TRANS_THRESHOLD) {
                fprintf(output, "%s", output_line);
            }
            //fprintf(output, "}\n");
            if(!feof(input))
                fsetpos(input, &pos_before);

            break;
        }
        else {
            if(trans_v_count != 0) {//if had values before
                strcat(output_line, ", ");
                //fprintf(output, ", ");
            }

            unsigned int index = item_index(data, item_value);

            char temp_out[128];
            sprintf(temp_out, "%d 1", index);

            strcat(output_line, temp_out);
            trans_v_count++;

        }
        fgetpos(input, &pos_before);
    }
    if(feof(input)) {
        strcat(output_line, "}\n");
        if(trans_v_count >= TRANS_THRESHOLD) {
            fprintf(output, "%s", output_line);
        }
        //fprintf(output, "}\n");
    }
    return;
}

void read_line(char * line, unsigned int * trans, unsigned int * item_value) {
    char firstpart[16], secondpart[16];
    if(strstr(line, trans_id) && strstr(line, item_id)) { //check if both identifiers are in the line
        sscanf(line, "%s %s", firstpart, secondpart);

        *trans = atoi(&firstpart[strlen(trans_id)]);
        *item_value = atoi(&secondpart[strlen(item_id)]);
    }
    else {
        fprintf(stderr, "tag missing for %s\n", line);
        *trans = 0;
        *item_value = 0;
    }

    return;
}

unsigned int item_index(value * data, unsigned int value) {
    unsigned int index;
    for(index = 0; data[index].value != 0; index++) {
        if(data[index].value == value)
            return index;
    }
    fprintf(stderr, "item index for %u could NOT be found, there IS something wrong, exitting\n", value);
    exit(-1);
    return 0;
}
