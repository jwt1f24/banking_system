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
    printf("\n========== CREATE ==========");
    char name[999], acctype[999];
    int idnum, pin;
    while (true)
    {
        printf("\nEnter your name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        bool inputValid = true;
        for (int i = 0; i < strlen(name); i++)
        {
            if (!isalpha(name[i]))
            {
                inputValid = false;
                break;
            }
        }
        if (inputValid != true)
        {
            printf("Invalid! Name must contain only letters.\n");
            continue;
        }
        else
        {
            break;
        }
    }

    printf("Enter your ID number: ");
    scanf("%d", &idnum);

    while (true)
    {
        printf("Choose account type (savings / current): ");
        fgets(acctype, sizeof(acctype), stdin);
        acctype[strcspn(acctype, "\n")] = '\0';
        bool inputValid = true;
        if (strcmp(acctype, "savings") == 0)
        {
            printf("Account type: Savings\n");
            break;
        }
        else if (strcmp(acctype, "current") == 0)
        {
            printf("Account type: Current\n");
            break;
        }
        else
        {
            printf("Invalid! Enter either 'savings' or 'current'.\n");
            continue;
        }
    }

    printf("Enter your 4-digit PIN: ");
    scanf("%d", &pin);
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
    while (true)
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
        else if (strcmp(option, "2") == 0 || strcmp(option, "delete") == 0)
        {
            deleteAcc();
            return;
        }
        else if (strcmp(option, "3") == 0 || strcmp(option, "deposit") == 0)
        {
            deposit();
            return;
        }
        else if (strcmp(option, "4") == 0 || strcmp(option, "withdraw") == 0)
        {
            withdraw();
            return;
        }
        else if (strcmp(option, "5") == 0 || strcmp(option, "remittance") == 0)
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
            printf("\nInvalid! Enter a number or a lowercase word specified in the brackets.\n");
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