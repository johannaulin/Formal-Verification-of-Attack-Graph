#include "local_model.c"
#include "defender_matrix.c"
#include "matrix_converter.c"
#include "finding_goals.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10000

int main( int argc, char *paths[] )
{   
    //printf("%s", paths[1]);
    //printf("%s", paths[2]);
    //printf("%s", paths[3]);
    FILE *attack_pointer;

    int MATRIX_WIDTH = countOccurrences_PATH(paths[1], "box") + countOccurrences_PATH(paths[1], "diamond") + 2;
    printf("%d", MATRIX_WIDTH);

    //split file in two before certain character 
    FILE *transition_file;
    FILE *states_file;
    FILE *rule_file;
    
    //needed for defender
    int amount_countermeasures; 
    char *countermeasure;


    //Split file into rules, states and transitions
    int total;
    int amount_transition;
    total = countOccurrences_PATH(paths[1], "box") + countOccurrences_PATH(paths[1], "diamond") + countOccurrences_PATH(paths[1], "ellipse");
    //total = count_total_states_and_rules(paths[1]);
    int split_list[total];
    amount_transition = split_file(transition_file, states_file, rule_file, paths[1], total, split_list);
    printf("Attack Graph File Splitted \n");


    //Construct matrix for attacker 
    //construct empty attack matrix, call on first half 
    int matrix[amount_transition][MATRIX_WIDTH]; //first Matrix_width-2 are possible preconditions, Matrix_width-1 is postcondition and Matrix_width is RULE activated
    int i, j, m, n;
    m = amount_transition;
    n = MATRIX_WIDTH;
    //we fill the matrix
    fill_matrix(MATRIX_WIDTH, amount_transition, total, matrix, split_list);
    printf("Attacker Matrix Constructed\n");

    //count amount of countermeasures
    amount_countermeasures = countOccurrences_PATH(paths[2], "True");

    //Construct the matrix for the defender
    int defender_matrix[amount_countermeasures];
    countermeasure_matrix(paths[2], paths[1], amount_countermeasures, defender_matrix);
    printf("Defenders Matrix Constructed \n");

    //we construct structure for goals
    int amount_state = total;
    int state_list[amount_state];
    
    //We find the goals
    find_goals(paths[1], paths[3], amount_state, state_list);
    printf("Goals are found \n");

    //Now we write the local model
    FILE *ISPL_file = fopen("ISPL_file.ispl", "w+"); // write only

    // Environment 
    construct_environment(MATRIX_WIDTH, ISPL_file, total, split_list, defender_matrix, amount_transition, matrix, amount_countermeasures);
    printf("Environment Constructed \n");

    // Attacker
    states_file = fopen("states_nodes", "r");
    construct_attacker(MATRIX_WIDTH, ISPL_file, states_file, amount_transition, matrix, amount_countermeasures, defender_matrix, total, split_list);
    printf("Attacker Constructed \n");

    // Construction of the agent Defender
    construct_defender(ISPL_file, amount_countermeasures, defender_matrix);
    printf("Defender Constructed \n");

    //construct the rest: Evaluation, Groups, Formulae
    construct_rest(ISPL_file, total, split_list, state_list);
    printf("Evaluation, Groups, Formulae Constructed \n");
    
    return 1;
}
