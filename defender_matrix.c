#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER 10000

void countermeasure_matrix(char countermeasure_file_path[BUFFER], char attack_path[BUFFER],int amount_countermeasures, int matrix[amount_countermeasures]){
    char line[BUFFER];
    char line2[BUFFER];
    int label;
    char vulnerability_string[BUFFER];
    int not_found;
    int i;

    FILE *countermeasure_file = fopen(countermeasure_file_path, "r");
    FILE *attack_graph_file;

    if (countermeasure_file == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write previleges.\n");
        exit(EXIT_FAILURE);
    }

    i = 0;
    while (!feof(countermeasure_file)){
        fgets(line, BUFFER, countermeasure_file);
        if (strstr(line, "True") != NULL){
            //printf("vuln string = %s \n", line);

            attack_graph_file = fopen(attack_path, "r");
            //find vulnerability
            sscanf(line, "Countermeasure = True : %s;", vulnerability_string);
            not_found = 0;
            //find vulnerability in file
            while (not_found == 0 && !feof(attack_graph_file)){               
                fgets(line2, BUFFER, attack_graph_file);
                 //if found find label
                 
                if (strstr(line2, vulnerability_string) != NULL){
                    sscanf(line2, "%d%*[^0123456789]", &label);
                    matrix[i] = label; //set the true countermeasure to what label it concerns 
                    not_found = 1;
                }
            }
            if (not_found == 0){
                matrix[i] = 0;
            }
            fclose(attack_graph_file);
            i++;
        }   
    }
    fclose(countermeasure_file);
}
