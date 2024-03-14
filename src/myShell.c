#include "myShell.h"
#include "myFunction.h"

int main()
{
    welcome();
    while (1)
    {
        int piping = 0;
        getLocation();
        char *input = getInputFromUser();
        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0)
            logout(input);
        char **arguments = splitArgument(input);
        if (strcmp(input, "echo") == 0)
            echo(arguments);
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "cp") == 0)
            cp(arguments);
        else if (strcmp(input, "delete") == 0)
            delete (arguments);
        else if (strcmp(input, "dir") == 0)
            get_dir();
        else if (piping)
        {
            char *argv1[] = {arguments[0], NULL};
            char *argv2[] = {arguments[2], arguments[3], NULL};
            mypipe(argv1, argv2);
            wait(NULL);
        }
        else
        {
            systemCall(arguments);
            wait(NULL);
        }
        free(arguments);
        free(input);
    }
    return 1;
}
void welcome() {
    puts("Welcome to my Shell");
    printf("   W   W EEEEE L     CCCC OOOOO MMMMM EEEEE\n");
    printf("   W W W E     L     C    O   O M   M E    \n");
    printf("   W W W EEEE  L     C    O   O MMMMM EEEE \n");
    printf("   W   W E     L     C    O   O M   M E    \n");
    printf("    W W  EEEEE LLLLL CCCC OOOOO M   M EEEEE\n");
    printf("Welcome to myShell v2.4.3\n");
    printf("WELCOME USER!\n");
}