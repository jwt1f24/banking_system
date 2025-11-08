#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
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
void enterName(bankAccount *acc)
{
    while (true)
    {
        printf("Enter your name: ");
        fgets(acc->name, sizeof(acc->name), stdin); // scan the user input
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
void enterID(bankAccount *acc)
{
    while (true)
    {
        printf("Enter your ID number (8 digits): ");
        fgets(acc->id, sizeof(acc->id), stdin);
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
void enterAccType(bankAccount *acc)
{
    while (true)
    {
        printf("Choose account type (Savings / Current): ");
        fgets(acc->accType, sizeof(acc->accType), stdin);
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
void enterPIN(bankAccount *acc)
{
    while (true)
    {
        printf("Enter your 4-digit PIN: ");
        fgets(acc->pin, sizeof(acc->pin), stdin);
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
long long createAccNo()
{
    long long min = 1000000LL;                                               // smallest 7-digit number
    long long max = 999999999LL;                                             // biggest 9-digit number
    long long GeneratedNumber = min + ((long long)rand() % (max - min + 1)); // formula to generate random number
    printf("Account Number: %lld\n", GeneratedNumber);
    return GeneratedNumber;
}

// checks if a file with the same number in database directory exists or not
void checkIfAccNoUnique(bankAccount *acc_arr[], bankAccount *acc, int *acc_count)
{
    char filePath[50];
    struct stat address;
    while (true)
    {
        long long GeneratedNumber = createAccNo();
        acc->accNo = GeneratedNumber;
        snprintf(filePath, sizeof(filePath), "database/%lld.txt", acc->accNo); // look for file with same name as generated number
        if (stat(filePath, &address) == 0)                                     // reloop & generate new number because account number already exists
        {
            printf("Account number already exists. Generating new account number...\n\n");
            continue;
        }
        else // create a txt file in 'database' directory
        {
            FILE *newFile = fopen(filePath, "w");
            fclose(newFile);

            // put account info into the txt file
            newFile = fopen(filePath, "a");
            fprintf(newFile, "Name: %s\n", acc->name);
            fprintf(newFile, "ID: %s\n", acc->id);
            fprintf(newFile, "Account Type: %s\n", acc->accType);
            fprintf(newFile, "PIN: %s\n", acc->pin);
            fprintf(newFile, "\nBalance: RM%.2f\n", acc->bal);
            fclose(newFile);
            printf("\n---------- Account created successfully! ----------\n");

            // store account number into array
            acc_arr[*acc_count] = acc;
            *acc_count++;
            break;
        }
    }
}

// function of operations to create bank account
void createAcc(bankAccount *acc_arr[], int *acc_count)
{
    printf("\n---------- CREATE ACCOUNT ----------\n");
    bankAccount acc;

    enterName(&acc);
    enterID(&acc);
    enterAccType(&acc);
    enterPIN(&acc);
    acc.bal = 0.00;

    checkIfAccNoUnique(acc_arr, &acc, acc_count);
}

void loadAccounts(bankAccount *acc_arr[], int *acc_count)
{
    DIR *directory = opendir("database");
    struct dirent *dir;
    char fileName[50];
    if (acc_count == 0)
    {
        printf("There are no existing bank accounts!");
        return;
    }
    while ((dir = readdir(directory)) != NULL)
    {
        size_t len = strlen(dir->d_name);
        if (len > 4 && strcmp(dir->d_name + (len - 4), ".txt") == 0)
        {
            strncpy(fileName, dir->d_name, len - 4);
            fileName[len - 4] = '\0';
            for (int i = 0; i < *acc_count; i++)
            {
                printf("%d) %s\n", i, acc_arr[i]);
            }
        }
    }
    closedir(directory);
}

void confirmIDNumber(bankAccount *acc_arr[], int *acc_count)
{
    bankAccount acc;
    int index;
    while (true)
    {
        int number;
        char input[10];

        printf("Enter index number: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &number) == 1 && number > 0 && number <= *acc_count)
        {
            break;
        }
        else
        {
            printf("Invalid! Enter an index number within the available range.\n\n");
            continue;
        }
    }
    /*
        char accNo[10];
        char fileSelected[10];
        strcpy(accNo, acc_arr[index - 1]);
        sprintf(fileSelected, "database/%s.bin", accNo);

        FILE *openFile = fopen(fileSelected, "rb");
        fread(&acc, sizeof(bankAccount), 1, openFile);
        fclose(openFile);

        while (true)
        {
            char input[10];
            printf("Does the ID number end with ****%s? (y/n): ", fileSelected);
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            if (strcmp(input, "y") == 0)
            {
                printf("hello");
                break;
            }
        }*/
}

void deleteFile()
{
}

// functions of operation to delete bank account
void deleteAcc(bankAccount *acc_arr[], int *acc_count)
{
    printf("\n---------- DELETE ACCOUNT ----------\n");
    loadAccounts(acc_arr, acc_count);
    confirmIDNumber(acc_arr, acc_count);
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
void menu(bankAccount *acc_arr[], int *acc_count)
{
    databaseDirectory(); // check if 'database' folder exists or not
    DIR *directory = opendir("database");
    struct dirent *dir;
    while ((dir = readdir(directory)) != NULL)
    {
        size_t len = strlen(dir->d_name);
        if (len > 4 && strcmp(dir->d_name + (len - 4), ".txt") == 0)
        {
            acc_count++;
        }
    }
    closedir(directory);

    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    char time[100];
    strftime(time, sizeof(time), "%d-%m-%Y %H:%M:%S", localTime);

    printf("---------- SESSION INFO ----------\n");
    printf("Time: %s\n", time);
    printf("No. of loaded accounts: %d\n", acc_count);
    printf("\nWelcome to the Banking System App!\n");

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
            createAcc(acc_arr, acc_count);
            return;
        }
        else if (strcmp(option, "2") == 0 || strcmp(option, "delete") == 0)
        {
            deleteAcc(acc_arr, acc_count);
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
    int *acc_count = 0;
    bankAccount *acc_arr[999];

    menu(acc_arr, acc_count); // launch menu to start program
    return 0;
}
// cd OneDrive/Documents/zbanksys
// .\index.exe