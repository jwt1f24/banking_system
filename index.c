#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
// create bank account
void createAcc()
{
    printf("CREATE");
}

// delete bank account
void deleteAcc()
{
    printf("DELETE");
}

// deposit money into bank account
void deposit()
{
    printf("DEPOSIT");
}

// withdraw money from bank account
void withdraw()
{
    printf("WITHDRAW");
}

// transfer money from your account to another account
void remittance()
{
    printf("REMITTANCE");
}

// main menu that uses user input to execute operations, exit option closes the app
void menu()
{
    char option[999];
    bool loop = true;
    while (loop)
    {
        printf("Please choose an operation:\n"
               "- Create New Bank Account (1 / create)\n"
               "- Delete Bank Account (2 / delete)\n"
               "- Deposit Cash (3 / deposit)\n"
               "- Withdraw Cash (4 / withdraw)\n"
               "- Remittance Transfer (5 / remittance)\n"
               "- Exit (6 / exit)\n"
               "Enter option: ");
        fgets(option, sizeof(option), stdin);
        option[strcspn(option, "\n")] = '\0';
        if (strcmp(option, "1") == 0 || strcmp(option, "create") == 0)
        {
            createAcc();
            return;
        }
        if (strcmp(option, "2") == 0 || strcmp(option, "delete") == 0)
        {
            deleteAcc();
            return;
        }
        if (strcmp(option, "3") == 0 || strcmp(option, "deposit") == 0)
        {
            deposit();
            return;
        }
        if (strcmp(option, "4") == 0 || strcmp(option, "withdraw") == 0)
        {
            withdraw();
            return;
        }
        if (strcmp(option, "5") == 0 || strcmp(option, "remittance") == 0)
        {
            remittance();
            return;
        }
        else if (strcmp(option, "6") == 0 || strcmp(option, "exit") == 0)
        {
            printf("\nClosing the program... Have a nice day!");
            return;
        }
        else
        {
            printf("\nInvalid input! Enter a number or a lowercase word specified in the bracket.\n");
            menu();
            return;
        }
    }
}

// instantiate the app by calling menu()
int main()
{
    printf("\nWelcome to the Banking System App!\n");
    menu();
    return 0;
}