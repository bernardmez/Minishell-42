#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // Test standard echo
    printf("Test 1: echo -n hello world\n");
    printf("Expected: hello world (no newline)\n");
    
    // Test with quoted strings
    printf("\nTest 2: echo 'hello $USER'\n");
    printf("Expected: hello $USER\n");
    
    // Test with variable
    printf("\nTest 3: echo \"hello $USER\"\n");
    char *user = getenv("USER");
    if (user)
        printf("Expected: hello %s\n", user);
    
    printf("\nTest 4: echo ~\n");
    char *home = getenv("HOME");
    if (home)
        printf("Expected: %s\n", home);
    
    return 0;
}
