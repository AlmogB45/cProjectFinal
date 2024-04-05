#include "myFunction.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size * sizeof(char));
    }
    *(str + index) = '\0';

    return str;
}
char **splitArgument(char *str)
{
    char *subStr;
    int size = 2;
    int index = 0;
    subStr = strtok(str, "\0");
    char **argumnts = (char **)malloc(size * sizeof(char *)); // dynamically allocates memory for the array of strings.
    *(argumnts + index) = subStr; // Using pointers to assign subStr to first element of arguments.
    while ((subStr = strtok(NULL, "\0")) != NULL) // If more toekns are found, it increase the 'size' var and reallocate memory for the array of strings.
    {
        size++;
        index++;
        *(argumnts + index) = subStr;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
    }
    *(argumnts + (index + 1)) = NULL; // At the end, it assign NULL to last element of 'arguments'.

    return argumnts;
}

void getLocation() {
    char location[BUFF_SIZE]; // Chars arrays defined to store location and username.
    char username[BUFF_SIZE]; // 

    struct passwd *pw = getpwuid(getuid()); // Get info on current user.
    if (!pw) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    const char *user = pw->pw_name; // Retrieve username.

    // Retrieve current working dir.
    if (getcwd(location, BUFF_SIZE) == NULL) {
        puts("Error");
    } else {
        bold();
        blue();
        printf("%s@%s:", user, location);
        reset();
        printf("$ ");
    }
}

void logout(char * input) {
// Remove leading and trailing whitespace from the input string.
    char *trimmedInput = strdup(input);
    char *end;

    // Trim leading space.
    while (isspace((unsigned char)*trimmedInput))
        trimmedInput++;

    if (*trimmedInput == 0) { 
        // Handle the case where the input contains only spaces.
        free(input);
        puts("Exiting...");
        exit(EXIT_SUCCESS);
    }

    // Trim trailing space.
    end = trimmedInput + strlen(trimmedInput) - 1;
    while (end > trimmedInput && isspace((unsigned char)*end))
        end--;

    // Null-terminate the trimmed string.
    *(end + 1) = 0;

    // Check if the trimmed input is "exit".
    if (strcmp(trimmedInput, "exit") == 0) {
        free(input);
        puts("Exiting...");
        exit(EXIT_SUCCESS);
    } else {
        if (input[0] == '\"') {
            // If input starts with a double quote, it's part of the path.
            char *newPath = strdup(input); // Duplicate input string to avoid modification of the original string.
            puts("New path received:");
            puts(newPath);
            free(newPath);
        } else {
            // Handle input as usual.
            free(input);
            puts("log out");
            exit(EXIT_SUCCESS);
        }
    }
}

void echo(char **arg)
{
    while (*(++arg))
        printf("%s ", *arg);
    puts("");
}

// Concatenate all arguments after "cd".
    char newPath[1024]; // 
    newPath[0] = '\0'; // Initialize newPath as an empty string, and assuming it's max length is 1024.
    for (int i = 1; arg[i] != NULL; i++)
    {
        strcat(newPath, arg[i]);
        strcat(newPath, " "); // Adds a space (" ") between each arg.
    }

    // Check if the path starts with a double quote.
    if (newPath[0] == '\"')
    {
        // Find the end of the quoted path.
        char *endQuote = strchr(newPath + 1, '\"');
        if (endQuote != NULL)
        {
            // Null-terminate the quoted path.
            *endQuote = '\0';
            if (chdir(newPath + 1) != 0)
                printf("-myShell: cd: %s: No such file or directory\n", newPath + 1);
        }
        else
        {
            printf("-myShell: cd: Unterminated quoted string\n");
        }
    }
    else
    {
        // Pass the path to the function that changes the process path.
        if (chdir(newPath) != 0)
            printf("-myShell: cd: %s: No such file or directory\n", newPath);
    }


void cp(char **arguments)
{
    char ch;
    FILE *src, *des;

    // Open source file
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("Error opening source file.");
        return;
    }

    // Open destination file
    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        puts("Error opening destination file.");
        fclose(src);
        return;
    }

    // Copy contents from source to destination
    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }

    // Close files
    fclose(src);
    fclose(des);
}


void get_dir()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./")) == NULL)
    {
        /* could not open directory */
        perror("");
        return;
    }
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL)
    {
        printf("%s ", ent->d_name);
    }
    puts("");
    closedir(dir);
}

void delete(char **arg)
{
    // Check if the path starts with a double quote.
    if (arg[1][0] == '\"') {
        // If the path starts with a double quote, it's a quoted path and it removes the quotes from the beginning and end of the path.
        char *path = arg[1] + 1; // Skip the starting quote.
        int len = strlen(path);
        if (path[len - 1] == '\"') {
            path[len - 1] = '\0'; // Remove the ending quote.
        }
        // Now 'path' contains the path without surrounding quotes and deletes the file using the unlink function.
        // 
        if (unlink(path) != 0) {
            // If unlink fails, print an error message.
            printf("-myShell: unlink: %s: No such file or directory\n", path);
        }
    } else {
        // If the path does not start with a double quote, it's a regular path and attempt to delete the file using the unlink function.
        if (unlink(arg[1]) != 0) {
            // If unlink fails, print an error message.
            printf("-myShell: unlink: %s: No such file or directory\n", arg[1]);
        }
    }
}

void systemCall(char **arg)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork err\n");
        return;
    }
    if (pid == 0 && execvp(arg[0], arg) == -1)
        exit(1);
}

void mypipe(char **argv1, char **argv2) {
    int fildes[2];
    
    // Check if the input contains the '|' character
    char **pipe_argv1 = argv1;
    char **pipe_argv2 = argv2;
    char *pipe_symbol = "|";
    int found_pipe = 0;

    while (*pipe_argv1 != NULL) {
        if (strcmp(*pipe_argv1, pipe_symbol) == 0) {
            found_pipe = 1;
            *pipe_argv1 = NULL;  // Replace '|' with NULL to terminate the first command
            pipe_argv2 = pipe_argv1 + 1;  // Start the second command after '|'
            break;
        }
        pipe_argv1++;
    }
    
    // If '|' was not found, print an error and return
    if (!found_pipe) {
        printf("Error: '|' not found\n");
        return;
    }
    
    // Fork a child process
    if (fork() == 0) {
        // Inside the child process
        
        // Create a pipe
        pipe(fildes);
        
        // Fork another child process
        if (fork() == 0) {
            // Inside the grandchild process (first component of command line)
            
            // Close standard output (STDOUT_FILENO)
            close(STDOUT_FILENO);
            
            // Redirect standard output to the write end of the pipe
            dup(fildes[1]);
            
            // Close the write end of the pipe
            close(fildes[1]);
            
            // Close the read end of the pipe
            close(fildes[0]);
            
            // Execute the command specified by argv1
            execvp(argv1[0], argv1);
        }
        
        // Close standard input (STDIN_FILENO)
        close(STDIN_FILENO);
        
        // Redirect standard input to the read end of the pipe
        dup(fildes[0]);
        
        // Close the read end of the pipe
        close(fildes[0]);
        
        // Close the write end of the pipe
        close(fildes[1]);
        
        // Execute the command specified by argv2
        execvp(argv2[0], argv2);
    }
}
