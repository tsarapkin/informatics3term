#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

const int MaxNumTasks = 100;
const int MaxLenTasks = 100;
const int MaxLenArg = 10;
const int MaxNumArg = 100;

typedef struct Task
{
    char *cmd;
    char **argCmd;
    int numArg;
    int sleepTime;
} Task;

void Split(char *string, char *delimiters, char ***tokens, int *countTokens);
void GetTask(FILE *fin, Task **tasks, int num);
void CompleteTasks(Task **tasks, int num);

int main()
{
    int i;
    int numCmd;

    Task *tasks =(Task *) malloc(MaxNumTasks * sizeof(Task) + 1);
    FILE *fin = fopen("input.txt", "r");
    char *c = (char *)malloc(MaxLenTasks * sizeof(char) + 1);

    fgets(c, MaxLenTasks, fin);
    numCmd = atoi(c);

    GetTask(fin, &tasks, numCmd);
    CompleteTasks(&tasks, numCmd);

    for (i = 0; i < numCmd; i++)
    {
        free(tasks[i].argCmd);
    }

    free(tasks);
    free(c);
    fclose(fin);

    return 0;
}

void Split(char *string, char *delimiters, char ***tokens, int *counterTokens)
{
    char *token;
    int sizeToken = 0, nextSize = 4;
    int counter = 0;
    char **temporary = *tokens;

    for (token = strtok(string, delimiters);
         token != NULL;
         token = strtok(NULL, delimiters))
    {
        if (counter == sizeToken)
        {
            sizeToken = nextSize;
            temporary = (char **)realloc(temporary, sizeToken * sizeof(char *) + 1);
            nextSize *= 2;
        }
        temporary[counter++] = token;
    }
    *tokens = temporary;
    *counterTokens = counter;
}

void GetTask(FILE *file, Task **tasks, int num)
{
    int i, k;
    char *delimiters = " \n";
    /*
     * +1?
     */
    char **pointersArg = (char **)malloc((MaxNumArg + 1) * sizeof(char *) + 1);

    for (i = 0; i < num; i++)
    {
        char* temporary = (char *)malloc(MaxLenTasks * sizeof(char));
        (*tasks)[i].argCmd = (char **)malloc(MaxNumArg * sizeof(char));

        fgets(temporary, MaxLenTasks, file);
        Split(temporary, delimiters, &pointersArg, &((*tasks)[i].numArg));

        (*tasks)[i].sleepTime = atoi(pointersArg[0]);
        (*tasks)[i].cmd = pointersArg[1];
        (*tasks)[i].numArg = (*tasks)[i].numArg - 1;

        for (k = 0; k < (*tasks)[i].numArg; k++)
        {
            ((*tasks)[i].argCmd)[k] = pointersArg[k + 1];
        }
        ((*tasks)[i].argCmd)[k + 1] = NULL;
    }
    free(pointersArg);
}

void CompleteTasks(Task **tasks, int num)
{
    pid_t pid = 0;
    int i, id;
    for (i = 0; i < num; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            sleep((*tasks)[i].sleepTime);
            execvp((*tasks)[i].cmd, (*tasks)[i].argCmd);
            printf("Error\n");
            exit(-1);
        }
    }
    for (i = 0; i < num; i++)
    {
        pid = wait(&id);
    }
}
