#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10000

//This function is used to count occurences of a certain string in a file
void order_matrix(int MATRIX_WIDTH, int amount_transition, int matrix[amount_transition][MATRIX_WIDTH], int new_matrix[amount_transition][MATRIX_WIDTH]){
    int i, j, k;
    int t = 0;
    for (k = MATRIX_WIDTH-2; k> 0; k--){
    for (j= 0 ; j < amount_transition; j++){
        
        if (matrix[j][k-1] != 0 && matrix[j][k] == 0){
            for (i = 0; i < k; i++){
                new_matrix[t][i] = matrix[j][i];

            }
            new_matrix[t][MATRIX_WIDTH-2] = matrix[j][MATRIX_WIDTH-2];
            new_matrix[t][MATRIX_WIDTH-1] = matrix[j][MATRIX_WIDTH-1];
        t++;   
        }    
    }
}

}

int countOccurrences_PATH(char path[BUFFER_SIZE], const char *word)
{   
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write previleges.\n");

        exit(EXIT_FAILURE);
    }
    char str[BUFFER_SIZE];
    char *pos;
    int index, count;   
    count = 0;
    // Read line from file till end of file.
    while ((fgets(str, BUFFER_SIZE, fptr)) != NULL)
    {
        index = 0;
        // Find next occurrence of word in str
        while ((pos = strstr(str + index, word)) != NULL)
        {
            // Index of word in str is
            // Memory address of pos - memory
            // address of str.
            index = (pos - str) + 1;
            count++;
        }
    }
    fclose(fptr);
    return count;
}

//This function will find the correct rule corresponding to the rule label number given
int find_rule(int rule_number){
    int label, rule;
    char string[BUFFER_SIZE];
    FILE *rule_file;
    int not_found = 0;
    rule_file = fopen("rules_nodes", "r");
    if (rule_file == NULL){
                printf("Unable to open rules file.\n");
                printf("Please check you have read/write previleges.\n");
                exit(EXIT_FAILURE);
    }   
    //we will now look for the correct transition in attack graph file
    while ((not_found == 0) && (!feof(rule_file))){
        fgets(string, BUFFER_SIZE, rule_file);      
        //we want to find the three integers in a rule, representing thelabel number and RULE number, cannot find d = 0  
        sscanf(string,"%d%*[^0123456789]%d%*[^0123456789] %d ",&label, &label, &rule); 
        //if label is rule number we found the correct transition
        if (label == rule_number + 1){
            not_found = 1;
            return rule;
        }
    }
    return 0; //we cannot find 0 thus we return 0 if i2 not found 
}

void fill_matrix(int MATRIX_WIDTH, int amount_transition, int total, int matrix[amount_transition][MATRIX_WIDTH], int split_list[total]){
    
    int i = 0;
    int j = 0;
    char read;
    int i1, i2;
    char stringline[BUFFER_SIZE];
    int transition = 0;
    int k = 0;
    FILE *transition_file;
    int rule_number;
    for (i = 0; i < amount_transition; i++)
    {
        for (j = 0; j < MATRIX_WIDTH; j++)
        {   
            matrix[i][j] = 0;
        }
    } 
    i = 0 ;
    j = 0 ;
    for (i = 0; i < total; i++){
        if (split_list[i] == 1) {
            rule_number = find_rule(i);//find rule number not label find_rule(i)
            matrix[transition][MATRIX_WIDTH-1] = rule_number; //set last entry with rule number
            transition_file = fopen("transitions", "r");
            if (transition_file == NULL){
                printf("Unable to open transition file.\n");
                printf("Please check you have read/write previleges.\n");
                exit(EXIT_FAILURE);
            }
            while (!feof(transition_file)){
                read = fgetc(transition_file);
                if (read == ';'){
                    stringline[j] = '\n';
                    sscanf(stringline,"%*[^0123456789]%d%*[^0123456789]%d",&i1, &i2);
                    if (i2 == i+1){
                        matrix[transition][k] = i1;
                        k++;
                    }
                    else if(i1 == i+1) {
                        matrix[transition][MATRIX_WIDTH-2] = i2;
                    }
                    j = 0;
                    i1 = 0;
                    i2 = 0;
                }   
                else{
                    stringline[j] = read;
                    j++;
                }
            }
        fclose(transition_file); 
        k = 0;
        transition ++;
        }       
    }
    int new_matrix[amount_transition][MATRIX_WIDTH];
    i =  0;
    j = 0 ;
    for (i = 0; i < amount_transition; i++)
    {
    for (j = 0; j < MATRIX_WIDTH; j++)
        {   
        new_matrix[i][j] = 0;
        }
    } 


    order_matrix(MATRIX_WIDTH, amount_transition, matrix, new_matrix);
    i =0;
    j = 0;
    for (i = 0; i < amount_transition; i++)
    {
        for (j = 0; j < MATRIX_WIDTH; j++)
        {
            matrix[i][j] = new_matrix[i][j];
        }
    }
    matrix = new_matrix;
}

int split_file(FILE *transition_file, FILE *states_file, FILE *rule_file, char path[BUFFER_SIZE], int total, int state_list[total]){ 
    int label = 0;
    int i = 0;
    int j;
    char read;
    FILE *attack_graph_file;
    transition_file = fopen("transitions", "w+");
    if (transition_file == NULL){
                printf("Unable to open created transition file.\n");
                printf("Please check you have read/write previleges.\n");
                exit(EXIT_FAILURE);
            }
    rule_file = fopen("rules_nodes", "w+");
    if (rule_file == NULL){
                printf("Unable to open created rules file.\n");
                printf("Please check you have read/write previleges.\n");
                exit(EXIT_FAILURE);
            }
    states_file = fopen("states_nodes", "w+");
    if (states_file == NULL){
                printf("Unable to open created states file.\n");
                printf("Please check you have read/write previleges.\n");
                exit(EXIT_FAILURE);
            }
    attack_graph_file = fopen(path, "r");
    if (attack_graph_file == NULL){
                printf("Unable to open attack graph file.\n");
                printf("Please check you have read/write previleges.\n");
                exit(EXIT_FAILURE);
            }
    int amount_rules = 0;

    while(! feof(attack_graph_file))
    {   char stringline[BUFFER_SIZE];
        char read = fgetc(attack_graph_file);
            if(read == ';'){ // ; marks end of line
                
                stringline[i] = ';';
                stringline[i+1] = '\n'; 
                j = 0;

                if ((strstr(stringline, "->") == NULL) && (strstr(stringline, "RULE") == NULL)){ 
                    state_list[label] = 0; //0 is pre or postcondition
                    label += 1;
                    while (j < i+1)
                    {   
                        fputc(stringline[j], states_file);
                        j ++;
                    }                             
                }

                else if ((strstr(stringline, "->") == NULL) && (strstr(stringline, "RULE") != NULL)){ 
                    state_list[label] = 1; //1 is rule, transition                     
                    label += 1 ;
                    amount_rules ++;
                    while (j < i+1)
                    {   
                        fputc(stringline[j], rule_file);
                        j ++;
                    }                             
                }
                else {
                    while (j < i+1){
                        fputc(stringline[j], transition_file);
                        j ++;
                    }  
            }
            i = 0;                   
            }
            else{
                stringline[i] = read;
                i++;
            }        
    }
    fclose(transition_file);
    fclose(states_file);
    fclose(rule_file);
    fclose(attack_graph_file);
    return amount_rules;
}
