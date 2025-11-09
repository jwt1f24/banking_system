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

int loadedAccounts = 0;

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
    char filePath[25];
    struct stat address;
    while (true)
    {
        acc->accNo = createAccNo();
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
            loadedAccounts++;

            // store account info into array
            acc_arr[*acc_count] = acc;
            (*acc_count)++;
            printf("\n---------- Account created successfully! ----------\n");
            break;
        }
    }
}

// function of operations to create bank account
void createAcc(bankAccount *acc_arr[], int *acc_count)
{
    printf("\n---------- CREATE ACCOUNT ----------\n");
    bankAccount *acc = malloc(sizeof(bankAccount));

    enterName(acc);
    enterID(acc);
    enterAccType(acc);
    enterPIN(acc);
    acc->bal = 0.00;

    checkIfAccNoUnique(acc_arr, acc, acc_count);
}

// display all exisitng bank accounts
void loadAccounts(bankAccount *acc_arr[], int *acc_count)
{
    for (int i = 0; i < *acc_count; i++)
    {
        printf("%d) %lld\n", i + 1, acc_arr[i]->accNo);
    }
}

// multi-factor authentication to confirm account credentials
int confirmDetails(bankAccount *acc_arr[], int *acc_count)
{
    while (true)
    {
        int number;
        char input[10];

        printf("Enter index number: ");
        fgets(input, sizeof(input), stdin);
        if (strchr(input, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                ;
        }
        if (sscanf(input, "%d", &number) == 1 && number > 0 && number <= *acc_count)
        {
            while (true)
            {
                char input2[10];
                printf("Does the ID number end with ****%s? (y/n): ", acc_arr[number - 1]->id + 4);
                fgets(input2, sizeof(input2), stdin);
                if (strchr(input2, '\n') == NULL)
                {
                    int leftoverInput;
                    while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                        ;
                }
                input2[strcspn(input2, "\n")] = '\0';
                if (strcmp(input2, "y") == 0)
                {
                    while (true)
                    {
                        char input3[10];
                        printf("Verify 4-digit PIN: ");
                        fgets(input3, sizeof(input3), stdin);
                        if (strchr(input3, '\n') == NULL)
                        {
                            int leftoverInput;
                            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                                ;
                        }
                        input3[strcspn(input3, "\n")] = '\0';
                        if (strcmp(input3, acc_arr[number - 1]->pin) == 0)
                        {
                            printf("PIN verified!\n");
                            return number;
                        }
                        else
                        {
                            printf("Invalid! Enter the correct 4-digit PIN.\n\n");
                            continue;
                        }
                    }
                }
                else if (strcmp(input2, "n") == 0)
                {
                    break;
                }
                else
                {
                    printf("Invalid! Enter 'y' for YES or 'n' for NO.\n\n");
                    continue;
                }
            }
        }
        else
        {
            printf("Invalid! Enter an index number within the available range.\n\n");
            continue;
        }
    }
}

// delete file and free memory of selected bank account
void deleteFile(bankAccount *acc_arr[], bankAccount *acc, int *acc_count)
{
    char filePath[25];
    int index = confirmDetails(acc_arr, acc_count);

    // delete file from 'database' directory
    snprintf(filePath, sizeof(filePath), "database/%lld.txt", acc_arr[index - 1]->accNo);
    remove(filePath);

    // free current bank account memory from array to make space for next/new account
    free(acc_arr[index - 1]);
    for (int i = index - 1; i < *acc_count - 1; i++)
    {
        acc_arr[i] = acc_arr[i + 1];
    }
    loadedAccounts--;
    acc_arr[*acc_count - 1] = NULL;
    (*acc_count)--;
    printf("\n---------- Account deleted successfully! ----------\n");
}

// function of operations to delete bank account
void deleteAcc(bankAccount *acc_arr[], int *acc_count)
{
    bankAccount *acc = malloc(sizeof(bankAccount));
    printf("\n---------- DELETE ACCOUNT ----------\n");
    if (loadedAccounts == 0)
    {
        printf("There are no existing bank accounts!");
        return;
    }
    else
    {
        loadAccounts(acc_arr, acc_count);
        deleteFile(acc_arr, acc, acc_count);
    }
}

// add money into bank account
void depositBalance(bankAccount *acc_arr[], bankAccount *acc, int *acc_count)
{
    int index = confirmDetails(acc_arr, acc_count);

    while (true)
    {
        double amount;
        char input[10];
        printf("Enter amount to deposit: RM");
        fgets(input, sizeof(input), stdin);
        if (strchr(input, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                ;
        }
        input[strcspn(input, "\n")] = '\0';
        if (sscanf(input, "%lf", &amount) == 1 && amount > 0 && amount <= 50000)
        {
            acc_arr[index - 1]->bal += amount; // add deposit amount to current balance

            // update balance in txt file
            char filePath[25];
            snprintf(filePath, sizeof(filePath), "database/%lld.txt", acc_arr[index - 1]->accNo);
            FILE *file = fopen(filePath, "w");
            fclose(file);

            file = fopen(filePath, "a");
            fprintf(file, "Name: %s\n", acc_arr[index - 1]->name);
            fprintf(file, "ID: %s\n", acc_arr[index - 1]->id);
            fprintf(file, "Account Type: %s\n", acc_arr[index - 1]->accType);
            fprintf(file, "PIN: %s\n", acc_arr[index - 1]->pin);
            fprintf(file, "\nBalance: RM%.2f\n", acc_arr[index - 1]->bal);
            fclose(file);

            printf("\n---------- Deposit successful! ----------\n");
            printf("Your new balance is: RM%.2f\n", acc_arr[index - 1]->bal);
            break;
        }
        else
        {
            printf("Invalid! Amount must be more than RM0 and less than or equal to RM50,000.\n\n");
            continue;
        }
    }
}

// function of operations to deposit money into a bank account
void deposit(bankAccount *acc_arr[], int *acc_count)
{
    bankAccount *acc = malloc(sizeof(bankAccount));
    printf("\n---------- DEPOSIT ----------\n");
    if (loadedAccounts == 0)
    {
        printf("There are no existing bank accounts!");
        return;
    }
    else
    {
        loadAccounts(acc_arr, acc_count);
        depositBalance(acc_arr, acc, acc_count);
    }
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
            deposit(acc_arr, acc_count);
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
    int acc_count = 0;
    int acc_max = 999;
    bankAccount **acc_arr = malloc(acc_max * sizeof(bankAccount *)); // array to store bank accounts and their data

    databaseDirectory(); // check if 'database' folder exists, create one if not
    DIR *directory = opendir("database");
    struct dirent *dir;
    while ((dir = readdir(directory)) != NULL)
    {
        size_t len = strlen(dir->d_name);
        if (len > 4 && strcmp(dir->d_name + (len - 4), ".txt") == 0)
        {
            char filePath[25];
            snprintf(filePath, sizeof(filePath), "database/%s", dir->d_name);
            FILE *file = fopen(filePath, "r");
            if (file != NULL)
            {
                bankAccount *acc = malloc(sizeof(bankAccount));
                fscanf(file, "Name: %[^\n]\n", acc->name);
                fscanf(file, "ID: %[^\n]\n", acc->id);
                fscanf(file, "Account Type: %[^\n]\n", acc->accType);
                fscanf(file, "PIN: %[^\n]\n", acc->pin);
                fscanf(file, "\nBalance: RM%lf\n", &acc->bal);
                char accNoStr[20];
                strncpy(accNoStr, dir->d_name, len - 4);
                accNoStr[len - 4] = '\0';
                acc->accNo = atoll(accNoStr);
                fclose(file);

                acc_arr[acc_count] = acc;
                acc_count++;
                loadedAccounts++;
            }
        }
    }
    closedir(directory);

    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    char time[100];
    strftime(time, sizeof(time), "%d-%m-%Y %H:%M:%S", localTime);

    printf("---------- SESSION INFO ----------\n");
    printf("Time: %s\n", time);
    printf("No. of loaded accounts: %d\n", loadedAccounts);
    printf("\nWelcome to the Banking System App!\n");

    menu(acc_arr, &acc_count); // launch menu to start program
    return 0;
}
// cd OneDrive/Documents/zbanksys
// .\index.exe