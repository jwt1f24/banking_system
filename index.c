#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

// create a 'database' directory if it has not existed yet
void databaseDirectory(void)
{
    if (MKDIR("database") == -1)
    {
        if (errno == EEXIST) // if database exists then just continue
        {
            return;
        }
        else
        {
            perror("mkdir"); // create folder otherwise
        }
    }
}

// setup structure that contains data of a bank account
typedef struct
{
    char name[55];
    char id[10];
    char accType[9];
    char pin[6];
    long long accNo; // used long long cause it can store large numbers while having less risk of bugs
    double bal;
} bankAccount;

// takes in user input to store info inside a new bank account
void enterName(bankAccount *acc, size_t size)
{
    while (true)
    {
        printf("Enter your name: ");
        fgets(acc->name, size, stdin); // scan the user input
        if (strchr(acc->name, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF) // remove extra input to prevent it from carrying over to the next input iteration
                ;
        }
        acc->name[strcspn(acc->name, "\n")] = '\0'; // remove space between each input character
        bool inputValid = true;
        if (strlen(acc->name) == 0) // invalid if name is empty
        {
            inputValid = false;
        }
        for (int i = 0; i < strlen(acc->name); i++)
        {
            if (!isalpha(acc->name[i]) && acc->name[i] != ' ') // invalid if name has non-letters
            {
                inputValid = false;
                break;
            }
        }
        if (inputValid != true)
        {
            printf("Invalid! Name must contain only letters.\n\n");
            continue; // reiterate while loop again since input is invalid
        }
        break; // exit while loop & move on to next function since input is valid
    }
}
void enterID(bankAccount *acc, size_t size)
{
    while (true)
    {
        printf("Enter your ID number (8 digits): ");
        fgets(acc->id, size, stdin);
        if (strchr(acc->id, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                ;
        }
        acc->id[strcspn(acc->id, "\n")] = '\0';
        bool inputValid = true;
        if (strlen(acc->id) != 8)
        {
            inputValid = false;
        }
        for (int i = 0; i < strlen(acc->id); i++)
        {
            if (!isdigit(acc->id[i]))
            {
                inputValid = false;
                break;
            }
        }
        if (inputValid != true)
        {
            printf("Invalid! ID must contain only 8-digit numbers.\n\n");
            continue;
        }
        break;
    }
}
void enterAccType(bankAccount *acc, size_t size)
{
    while (true)
    {
        printf("Choose account type (Savings / Current): ");
        fgets(acc->accType, size, stdin);
        if (strchr(acc->accType, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                ;
        }
        acc->accType[strcspn(acc->accType, "\n")] = '\0';
        if (strcmp(acc->accType, "Savings") == 0 || strcmp(acc->accType, "Current") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid! Enter either 'Savings' or 'Current'.\n\n");
            continue;
        }
    }
}
void enterPIN(bankAccount *acc, size_t size)
{
    while (true)
    {
        printf("Enter your 4-digit PIN: ");
        fgets(acc->pin, size, stdin);
        if (strchr(acc->pin, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                ;
        }
        acc->pin[strcspn(acc->pin, "\n")] = '\0';
        bool inputValid = true;
        if (strlen(acc->pin) != 4)
        {
            inputValid = false;
        }
        for (int i = 0; i < strlen(acc->pin); i++)
        {
            if (!isdigit(acc->pin[i]))
            {
                inputValid = false;
                break;
            }
        }
        if (inputValid != true)
        {
            printf("Invalid! PIN must contain only 4-digit numbers.\n\n");
            continue;
        }
        break;
    }
}

// generate a random account number btwn 7-9 digits
long long createAccNo(bankAccount *acc)
{
    long long min = 1000000LL;   // smallest 7-digit number
    long long max = 999999999LL; // biggest 9-digit number
    long long GeneratedNumber = acc->accNo;
    GeneratedNumber = min + ((long long)rand() % (max - min + 1)); // formula to generate random number
    printf("Account Number: %lld\n", GeneratedNumber);
    return GeneratedNumber;
}

// checks if a file with the same number in database directory exists or not
void checkIfAccNoUnique(bankAccount *acc)
{
    char file[100];
    while (true)
    {
        createAccNo(acc);
        snprintf(file, sizeof(file), "database/%lld,txt", acc->accNo); // look for file with same name as generated number

        if (stat(file, &acc) == 0) // reloop & generate new number because account number already exists
        {
            continue;
        }
        else // create a txt file in 'database' directory
        {
            FILE *file;
            file = fopen("database/%lld.txt", "w");
            fclose(file);
        }
    }
}

// function of operations to create bank account
void createAcc()
{
    printf("\n---------- CREATE ACCOUNT ----------\n");
    bankAccount acc;

    enterName(&acc, sizeof(acc.name));
    enterID(&acc, sizeof(acc.id));
    enterAccType(&acc, sizeof(acc.accType));
    enterPIN(&acc, sizeof(acc.pin));
    acc.bal = 0.00;

    checkIfAccNoUnique(&acc);
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
            continue;
        }
    }
}

// starts up the program by calling menu()
int main()
{
    databaseDirectory(); // check if 'database' folder exists or not

    srand(time(NULL));
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    localTime->tm_hour += 8; // Malaysia local time
    mktime(localTime);
    char time[80];
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", localTime);

    printf("---------- SESSION INFO ----------\n");
    printf("Time: %s\n", time);
    printf("No. of loaded accounts: \n");
    printf("\nWelcome to the Banking System App!\n");
    menu(); // launch menu to start program
    return 0;
}