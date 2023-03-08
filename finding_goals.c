#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 10000

void find_goals(char attack_path[BUFFER_SIZE], char graph_path[BUFFER_SIZE], int amount_state, int state_list[amount_state]){
    char line[BUFFER_SIZE];
    char attack_goal[BUFFER_SIZE];
    char string[BUFFER_SIZE];
    FILE * graphmodel;
    FILE * attack_graph;
    int i1, i;

    for (i= 0; i< amount_state; i++){
        state_list[i] = 0;
    }

    graphmodel = fopen(graph_path, "r");
    if (graphmodel == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write previleges.\n");

        exit(EXIT_FAILURE);
    }
    
    attack_graph = fopen(attack_path, "r");
    if (attack_graph == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please check you have read/write previleges.\n");

        exit(EXIT_FAILURE);
    }

    while (!feof(graphmodel)){

        fgets(line, BUFFER_SIZE, graphmodel);
        if (strstr(line, "attackGoal") != NULL){
            char a,b;
            char stringy[BUFFER_SIZE];
            sscanf(line, "attackGoal(%s%[^).]).", attack_goal, stringy);
            int len = strlen(attack_goal);
            attack_goal[len-2] = '\0';
            
            char *attack_goal1;
            char *attack_goal_rest;
            char attack_line[BUFFER_SIZE];
            
            while (!feof(attack_graph)){
                fgets(attack_line, BUFFER_SIZE, attack_graph);
                
                if (strstr(attack_line, attack_goal) != NULL){
                    sscanf(attack_line,"%d%*[^0123456789]",&i1);
                    state_list[i1-1] = 1; //set index of state to 

                }

                else if (strstr(attack_goal, "_") != NULL){
                    attack_goal1 = strtok(attack_goal, "_");
                    attack_goal_rest = strtok(NULL, "_");
                    while ((strstr(attack_goal_rest, "_") != NULL) && (strstr(attack_line, attack_goal1) == NULL)){
                        attack_goal1 = strtok(attack_goal_rest, "_");
                        attack_goal_rest = strtok(NULL, "_");
                    }
                    if (strstr(attack_line, attack_goal_rest) != NULL){
                        sscanf(attack_line,"%d%*[^0123456789]",&i1);
                        state_list[i1] = 1; //set index of state to                       
                    }
                    
              
                }   
            }            
      break;  
    }
}
}