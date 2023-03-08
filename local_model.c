#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10000

int remove_duplicates(int length, int matrix[length]){
    int i, j, k;
    i = 0;
    for ( i = 0; i < length; i ++) 
        {  
        for ( j = i + 1; j < length; j++)  
        {  
            // use if statement to check duplicate element  
            if (matrix[i] == matrix[j])  
            {  
                // delete the current position of the duplicate element  
                for ( k = j; k < length - 1; k++)  
                {  
                    //length -= 1;
                    matrix[k] = matrix[k + 1];  
                }  
                // decrease the size of array after removing duplicate element  
                length--;  
                                
            // if the position of the elements is changes, don't increase the index j  
                j--;      
            }  
        }  
    }
    return length;
}

void construct_defender(FILE *ISPL_file, int amount_countermeasures, int defender_matrix[amount_countermeasures]){
    int i;
    fputs("Agent Defender\n", ISPL_file);

    //write Variables
    fputs( "    Vars:\n", ISPL_file);
    fputs("         state : {none};\n", ISPL_file);
    //the state are the vulnerabilities, use label in attack_graph
    fputs("     end Vars\n", ISPL_file);
    fputs("     Actions = {", ISPL_file);
    //define as numbers of vulnerabilities,
    if (amount_countermeasures == 0 ){
        fputs("none};", ISPL_file);
    }
    else{
    for (i = 0; i< amount_countermeasures-1; i ++){
        if (defender_matrix[i] != 0){
         fprintf(ISPL_file, "act%d,", defender_matrix[i]);
        }
    }
    fprintf(ISPL_file, "act%d};", defender_matrix[i]);
    }
    fputs("\n", ISPL_file);

    fputs("     Protocol:\n", ISPL_file);
    //if the vulnerability is true we can use countermeasure   
    i = 0; 
    if (amount_countermeasures == 0 ){
        fputs("     state = none :{none};\n", ISPL_file);
    }
    else{
        for (i = 0; i< amount_countermeasures; i ++){
            if (defender_matrix[i] != 0){
            fprintf(ISPL_file, "       Environment.var%d = true : {act%d}; \n", defender_matrix[i], defender_matrix[i]);
        }
    }
    }
    fputs("     end Protocol\n", ISPL_file);
    fputs("     Evolution:\n        (state = none) if (", ISPL_file);
    if (amount_countermeasures == 0 ){
        fputs("Action = none);\n", ISPL_file);
    }
    else{
    
    for (i = 0; i< amount_countermeasures-1; i ++){
            if (defender_matrix[i] != 0){
         fprintf(ISPL_file, "(Action = act%d) or ", defender_matrix[i]);}
    }
    fprintf(ISPL_file, "(Action = act%d));\n", defender_matrix[i]);
    }
    //if the vulnerability is true we can use countermeasure to set is at False  
    fputs("     end Evolution\n", ISPL_file);
    fputs("end Agent\n\n", ISPL_file);
}

void construct_attacker(int MATRIX_WIDTH, FILE *ISPL_file, FILE *states_file, int amount_transitions, int matrix[amount_transitions][MATRIX_WIDTH], int amount_countermeasures, int defender_matrix[amount_countermeasures], int total, int split_list[total]){
    int i1;
    char string[BUFFER_SIZE];
    fputs("Agent Attacker\n", ISPL_file);

    // Write Variables 
    fputs( "    Vars:\n", ISPL_file);
    fputs( "        state: {none};", ISPL_file);
    int i;
    int k = 0;
    fputs("    end Vars\n", ISPL_file);

    //write Actions
    fputs("    Actions = {", ISPL_file);
    int rule_list[amount_transitions];
    int j;
    for (i=0;i<amount_transitions; i++){
        rule_list[i] = matrix[i][MATRIX_WIDTH-1];
    }
    int length = remove_duplicates(amount_transitions, rule_list);  

    for (i=0;i<length; i++){
        if (i == length-1){
            fprintf(ISPL_file, "act%d};\n", rule_list[i]);
        }
        else {
            fprintf(ISPL_file, "act%d, ", rule_list[i]);
        }
    }

    //write Protocol
    int precond;
    k = 0;

    fputs("    Protocol:\n", ISPL_file); 
    for (i=0; i < amount_transitions; i++){
        fputs("        (", ISPL_file);
        precond = matrix[i][0];
        while (precond != 0 ){
            fprintf(ISPL_file, "Environment.var%d = true", precond);
            k++;
            precond = matrix[i][k];
            if (precond !=  0){
                fputs(" and \n        ", ISPL_file);
                }
            }
            fprintf(ISPL_file, ") : {act%d};\n", matrix[i][MATRIX_WIDTH-1]);
            k = 0;
    }
    fputs("    end Protocol\n", ISPL_file);

    //write Evolution   
    fputs("    Evolution:\n         (state = none) if (", ISPL_file);
    for (i=0;i<length; i++){
        if (i == length-1){
            fprintf(ISPL_file, "(Action = act%d));\n", rule_list[i]);
        }
        else {
            fprintf(ISPL_file, "(Action = act%d) or \n           ", rule_list[i]);
        }
    }
  
    fputs("    end Evolution\n", ISPL_file);
    

    fputs("end Agent\n\n", ISPL_file);

}

void construct_environment(int MATRIX_WIDTH, FILE *ISPL_file, int total, int split_list[total], int defender_matrix[total], int amount_transition, int matrix[amount_transition][MATRIX_WIDTH], int amount_countermeasures){
    int j, precond, i;
    
    fputs("Agent Environment\n", ISPL_file);
    fputs("     Obsvars:\n", ISPL_file);
    
    for (i = 1; i <= total; i++){
        if (split_list[i-1] == 0) {
        fprintf(ISPL_file, "        var%d: boolean;\n", i);   
        }   
    }
    fputs("     end Obsvars\n", ISPL_file);
    fputs("     Vars:\n", ISPL_file);
    fputs("     end Vars\n", ISPL_file);   
    fputs("     Actions = {none};\n", ISPL_file);
    fputs("     Protocol:\n", ISPL_file);
    fputs("         Other: {none};\n", ISPL_file);
    fputs("     end Protocol\n", ISPL_file);
        fputs("    Evolution:\n", ISPL_file);

    for (i = 0; i< amount_countermeasures; i ++){
        if (defender_matrix[i] != 0){
         fprintf(ISPL_file, "       (var%d = false) if ((var%d = true) and (Defender.Action = act%d)) ;\n", defender_matrix[i], defender_matrix[i], defender_matrix[i]);
        }
    }
    
    for (i=0; i < amount_transition; i++){
        //new precondition true
        fprintf(ISPL_file, "        ((var%d = true) and ", matrix[i][MATRIX_WIDTH-2]);

        
        //keep old preconditions true 
        precond = matrix[i][0];
        int k = 0;
        while (precond != 0 ){
        fprintf(ISPL_file, "(var%d = true)", precond);
        k++;
        precond = matrix[i][k];

        if (precond !=  0){
            fputs(" and\n        ", ISPL_file);
        }
        }
        
        fprintf(ISPL_file, ") if ");
        fprintf(ISPL_file, "(");

        //precondditions we need to be true
        precond = matrix[i][0];
        k = 0;
        int y;
        while (precond != 0 ){
            fprintf(ISPL_file, "(var%d = true)", precond);
            k++;
            for (j = 0 ; j< amount_countermeasures ; j++){                  
                if (defender_matrix[j] == precond){
                    fprintf(ISPL_file,"and !(Defender.Action = act%d)", defender_matrix[j]);
                    y = 0;   
                }
            }
            precond = matrix[i][k];
            if (precond !=  0){
                fputs(" and\n        ", ISPL_file);
            }
        }
       
        //action that needs to be taken
        fprintf(ISPL_file, " and\n        (Attacker.Action = act%d));\n", matrix[i][MATRIX_WIDTH-1]);
    }
    fputs("    end Evolution\n", ISPL_file);
    
    fputs("end Agent\n\n", ISPL_file);
}

void construct_rest(FILE *ISPL_file, int total, int split_list[total], int state_list[total]){
    fputs("Evaluation\n", ISPL_file);
    // Define atomic propostion GOAL 
    fputs("    goal_reached if  ", ISPL_file); //not finished 
    int goal;
    int k = 0 ;   
    int i;
    for (i= 0; i < total; i++){
        if (state_list[i] == 1 && k ==0){
            fprintf(ISPL_file, "(Environment.var%d = true) ", i+1);
            k = 1;
        }
        else if ((state_list[i] == 1) && (k ==1)){
            fprintf(ISPL_file, "or\n         (Environment.var%d = true) ", i+1);

        }
        // we need to define the goals where we get them how we get them etc.
        //here we set the goal for the attacker.
    }
    fprintf(ISPL_file, ";\n");
    
    fputs("end Evaluation\n\n", ISPL_file);

    fputs("InitStates\n", ISPL_file);
    //define True and False for Boxes and Diamonds for the variables in the environment 
    FILE *state_file;
    char line[BUFFER_SIZE];
    state_file = fopen("states_nodes", "r");
    if (state_file == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write previleges.\n");

        exit(EXIT_FAILURE);
    }
    fgets(line, BUFFER_SIZE, state_file);
    while (!feof(state_file)){
       
            fgets(line, BUFFER_SIZE, state_file);
            //printf("%s", line);

            sscanf(line,"%d%*[^0123456789]",&i);
            if (!feof(state_file)){
            if (strstr(line, "box") == NULL){
                fprintf(ISPL_file, "        Environment.var%d = false and\n", i);
            }
            else{
                fprintf(ISPL_file, "        Environment.var%d = true and \n", i);                
            }
            }
            else{
            if (strstr(line, "box") == NULL){
                fprintf(ISPL_file, "        Environment.var%d = false;\n", i);
            }
            else{
                fprintf(ISPL_file, "        Environment.var%d = true;\n", i);                
            }
            }
        
        } 
    fclose(state_file);  
    fputs("end InitStates\n\n", ISPL_file);

    // We can now define the groups and formulae.

    //Group
    fputs("Groups\n", ISPL_file);
    fputs("     g1 = {Attacker};\n", ISPL_file);
    fputs("end Groups\n\n", ISPL_file);
    // Formulae
    fputs("Formulae\n", ISPL_file);
    fputs("    <g1>F goal_reached;\n", ISPL_file);
    fputs("end Formulae\n\n", ISPL_file);
}
