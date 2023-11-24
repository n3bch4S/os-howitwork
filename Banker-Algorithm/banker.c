#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define DEFAULT_STRING_SLOT 100

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int request_resources(int customer_num, int request[])
{
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (request[i] > available[i])
        {
            printf("not enough resource availabel[%d]\n", i);
            return -1;
        }
        if (request[i] > need[customer_num][i])
        {
            printf("more than promise need[%d][%d]\n", customer_num, i);
            return -1;
        }
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        available[i] = available[i] - request[i];
        allocation[customer_num][i] = allocation[customer_num][i] + request[i];
        need[customer_num][i] = need[customer_num][i] - request[i];
    }
    printf("request complete\n");
    return 0;
}

void release_resources(int customer_num, int release[])
{
    int canRelease = 1;
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (release[i] > allocation[customer_num][i])
        {
            printf("too much release allocation[%d][%d]\n", customer_num, i);
            canRelease = 0;
            break;
        }
    }
    if (canRelease)
    {
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        {
            available[i] = available[i] + release[i];
            allocation[customer_num][i] = allocation[customer_num][i] - release[i];
            need[customer_num][i] = need[customer_num][i] + release[i];
        }
        printf("release complete\n");
    }
}

void buildMaxMatrix(char filename[])
{
    FILE *fptr = fopen(filename, "r");
    char line[DEFAULT_STRING_SLOT];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        fgets(line, DEFAULT_STRING_SLOT, fptr);
        maximum[i][0] = atoi(&line[0]);
        maximum[i][1] = atoi(&line[2]);
        maximum[i][2] = atoi(&line[4]);
        maximum[i][3] = atoi(&line[6]);

        need[i][0] = atoi(&line[0]);
        need[i][1] = atoi(&line[2]);
        need[i][2] = atoi(&line[4]);
        need[i][3] = atoi(&line[6]);
    }
    fclose(fptr);
}

void buildAvailable(char *argv[])
{
    available[0] = atoi(argv[1]);
    available[1] = atoi(argv[2]);
    available[2] = atoi(argv[3]);
    available[3] = atoi(argv[4]);
}

void printArray(int arr[])
{
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        printf("%d, ", arr[i]);
    printf("\n");
}

void printMatrix(int matrix[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES])
{
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        printArray(matrix[i]);
}

void printAll()
{
    printf("available\n");
    printArray(available);
    printf("\nmaximum\n");
    printMatrix(maximum);
    printf("\nallocation\n");
    printMatrix(allocation);
    printf("\nneed\n");
    printMatrix(need);
}

void runCommand()
{
    printf("running...\n");
    char cmd[DEFAULT_STRING_SLOT];
    int custNum, r0, r1, r2, r3;
    while (1)
    {
        printf("\n\nthada-shell>");
        scanf("%s", cmd);
        if (strcasecmp(cmd, "*") == 0)
        {
            printAll();
            continue;
        }
        else if (strcasecmp(cmd, "\\q") == 0)
        {
            printf("bye\n");
            break;
        }
        else if (strcasecmp(cmd, "RQ") != 0 && strcasecmp(cmd, "RL") != 0)
        {
            printf("no command found\n");
            continue;
        }

        scanf("%d %d %d %d %d", &custNum, &r0, &r1, &r2, &r3);
        int resourceV[4] = {r0, r1, r2, r3};

        if (strcasecmp(cmd, "RQ") == 0)
        {
            request_resources(custNum, resourceV);
        }
        else if (strcasecmp(cmd, "RL") == 0)
        {
            release_resources(custNum, resourceV);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != NUMBER_OF_RESOURCES + 1)
    {
        printf("Number of arguments is not %d\n", NUMBER_OF_RESOURCES);
        return 1;
    }
    buildAvailable(argv);
    buildMaxMatrix("maxNeed.txt");
    runCommand();
    return 0;
}
