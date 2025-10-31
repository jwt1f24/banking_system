#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// setup structure that contains data of a bank account
typedef struct
{
    char name[55];
    char id[9];
    char accType[8];
    char pin[5];
    char accNo[10];
    double bal;
} bankAccount;

// takes in user input for creating a bank account
void enterName(char *name)
{
    while (true)
    {
        printf("Enter your name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        bool inputValid = true;
        if (name == NULL || strlen(name) == 0) // invalid if name is empty
        {
            inputValid = false;
        }
        for (int i = 0; i < strlen(name); i++)
        {
            if (!isalpha(name[i])) // invalid if name has non-letters
            {
                inputValid = false;
                break;
            }
        }
        if (inputValid != true)
        {
            printf("Invalid! Name must contain only letters.\n");
            continue; // loop again since input is invalid
        }
        else
        {
            break; // exit loop & move on to next function since input is valid
        }
    }
}
void enterID(char *id)
{
    while (true)
    {
        printf("Enter your ID number: ");
        fgets(id, sizeof(id), stdin);
        id[strcspn(id, "\n")] = '\0';
        bool inputValid = true;
        if (strlen(id) != 8)
        {
            inputValid = false;
        }
        for (int i = 0; i < strlen(id); i++)
        {
            if (!isdigit(id[i]))
            {
                inputValid = false;
                break;
            }
        }
        if (inputValid != true)
        {
            printf("Invalid! ID must contain 8 digits and no letters.\n");
            continue;
        }
        else
        {
            break;
        }
    }
}
void enterAccType(char *accType)
{
    while (true)
    {
        printf("Choose account type (Savings / Current): ");
        fgets(accType, sizeof(accType), stdin);
        accType[strcspn(accType, "\n")] = '\0';
        bool inputValid = true;
        if (strcmp(accType, "Savings") == 0)
        {
            printf("Account type: Savings\n");
            break;
        }
        else if (strcmp(accType, "Current") == 0)
        {
            printf("Account type: Current\n");
            break;
        }
        else
        {
            printf("Invalid! Enter either 'Savings' or 'Current'.\n");
            continue;
        }
    }
}
void enterPIN(char *pin)
{
    printf("Enter your 4-digit PIN: ");
    scanf("%d", &pin);
}
/*
// checks if a random generated account number is available or not
int uniqueAccNo(char *accNo)
{
    FILE *file = fopen("database/index.txt", "r");
    if (file == NULL)
        return 1;

    char takenNo[10];
    while (fscanf(file, "%s", takenNo) != EOF)
    {
        if (strcmp(takenNo, accNo) == 0)
        {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}
*/
void createAccNo(char *accNo)
{ /*
     while (!uniqueAccNo(accNo))
     {
         sprintf(accNo, "%07d", rand() % 9000000 + 1000000);
     }*/
}

// function of operations to create bank account
void createAcc()
{
    printf("\n========== CREATE ACCOUNT ==========\n");
    bankAccount acc;
    enterName(acc.name);
    enterID(acc.id);
    enterAccType(acc.accType);
    enterPIN(acc.pin);
    createAccNo(acc.accNo);
    acc.bal = 0;
}

// functions of operation to delete bank account
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

// starts up the program by calling menu()
int main()
{
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    localTime->tm_hour += 8;
    mktime(localTime);
    char time[80];
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", localTime);

    double bal = 0;

    printf("========== SESSION INFO ==========\n");
    printf("Time: %s\n", time);
    printf("No. of loaded accounts: \n");
    printf("\nWelcome to the Banking System App!\n");
    menu();
    return 0;
}