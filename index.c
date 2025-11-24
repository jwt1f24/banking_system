#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <limits.h>

int loadedAccounts = 0;
int arr_index;
int arr_index2;

// append entry for each action to transction.log
void appendLog(char *action)
{
    FILE *log = fopen("database/transaction.log", "a");

    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    char time[100];
    strftime(time, sizeof(time), "%d-%m-%Y %H:%M:%S", localTime);

    fprintf(log, "[%s] %s\n", time, action); // add changes to transaction.log
    fclose(log);
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
    while (true)
    {
        acc->accNo = createAccNo();
        snprintf(filePath, sizeof(filePath), "database/%lld.txt", acc->accNo); // look for file with same name as generated number
        FILE *accountFile = fopen(filePath, "r");
        if (accountFile != NULL) // reloop & generate new number because account number already exists
        {
            fclose(accountFile);
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

            // store account info into array
            acc_arr[*acc_count] = acc;
            (*acc_count)++;
            printf("\n---------- Account created successfully! ----------\n");
            // append changes to transaction log
            char action[100];
            snprintf(action, sizeof(action), "Created account: %lld", acc->accNo);
            appendLog(action);
            loadedAccounts++;
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

    // append changes to transaction log
    char action[100];
    snprintf(action, sizeof(action), "Deleted account: %lld", acc_arr[index - 1]->accNo);
    appendLog(action);

    // delete file from 'database' directory
    snprintf(filePath, sizeof(filePath), "database/%lld.txt", acc_arr[index - 1]->accNo);
    remove(filePath);

    // free current bank account memory from array to make space for next/new account
    free(acc_arr[index - 1]);
    for (int i = index - 1; i < *acc_count - 1; i++)
    {
        acc_arr[i] = acc_arr[i + 1];
    }
    acc_arr[*acc_count - 1] = NULL;
    (*acc_count)--;
    printf("\n---------- Account deleted successfully! ----------\n");
    loadedAccounts--;
}

// function of operations to delete bank account
void deleteAcc(bankAccount *acc_arr[], int *acc_count)
{
    bankAccount *acc = malloc(sizeof(bankAccount));
    printf("\n---------- DELETE ACCOUNT ----------\n");
    if (loadedAccounts > 0)
    {
        loadAccounts(acc_arr, acc_count);
        deleteFile(acc_arr, acc, acc_count);
    }
    else
    {
        printf("There are no existing bank accounts!\n");
        return;
    }
}

// rewrite txt file's data
void updateFile(bankAccount *acc_arr[])
{
    char filePath[25];
    snprintf(filePath, sizeof(filePath), "database/%lld.txt", acc_arr[arr_index - 1]->accNo);
    FILE *file = fopen(filePath, "w");
    fclose(file);

    file = fopen(filePath, "a");
    fprintf(file, "Name: %s\n", acc_arr[arr_index - 1]->name);
    fprintf(file, "ID: %s\n", acc_arr[arr_index - 1]->id);
    fprintf(file, "Account Type: %s\n", acc_arr[arr_index - 1]->accType);
    fprintf(file, "PIN: %s\n", acc_arr[arr_index - 1]->pin);
    fprintf(file, "\nBalance: RM%.2f\n", acc_arr[arr_index - 1]->bal);
    fclose(file);
}

// rewrite txt file's data, mainly for receiver account during remittance
void updateReceiverFile(bankAccount *acc_arr[])
{
    char filePath[25];
    snprintf(filePath, sizeof(filePath), "database/%lld.txt", acc_arr[arr_index2 - 1]->accNo);
    FILE *file = fopen(filePath, "w");
    fclose(file);

    file = fopen(filePath, "a");
    fprintf(file, "Name: %s\n", acc_arr[arr_index2 - 1]->name);
    fprintf(file, "ID: %s\n", acc_arr[arr_index2 - 1]->id);
    fprintf(file, "Account Type: %s\n", acc_arr[arr_index2 - 1]->accType);
    fprintf(file, "PIN: %s\n", acc_arr[arr_index2 - 1]->pin);
    fprintf(file, "\nBalance: RM%.2f\n", acc_arr[arr_index2 - 1]->bal);
    fclose(file);
}

// add money into bank account
void depositBalance(bankAccount *acc_arr[], bankAccount *acc, int *acc_count)
{
    arr_index = confirmDetails(acc_arr, acc_count); // get the array index of selected account
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
            acc_arr[arr_index - 1]->bal += amount; // add deposit amount to current balance
            updateFile(acc_arr);                   // update txt file
            printf("\n---------- Deposit successful! ----------\n");
            printf("Your new balance is: RM%.2f\n", acc_arr[arr_index - 1]->bal);
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
    if (loadedAccounts > 0)
    {
        loadAccounts(acc_arr, acc_count);
        depositBalance(acc_arr, acc, acc_count);
    }
    else
    {
        printf("There are no existing bank accounts!\n");
        return;
    }
}

// deduct money from bank account
void withdrawBalance(bankAccount *acc_arr[], bankAccount *acc, int *acc_count)
{
    arr_index = confirmDetails(acc_arr, acc_count);
    while (true)
    {
        double amount;
        char input[10];
        printf("Your current balance is: RM%.2f\n", acc_arr[arr_index - 1]->bal);
        if (acc_arr[arr_index - 1]->bal == 0) // no money to withdraw
        {
            printf("Invalid! Your account has no money to withdraw!\n");
            return;
        }
        printf("Enter amount to withdraw: RM");
        fgets(input, sizeof(input), stdin);
        if (strchr(input, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                ;
        }
        input[strcspn(input, "\n")] = '\0';
        if (sscanf(input, "%lf", &amount) == 1 && amount > 0 && amount <= (acc_arr[arr_index - 1]->bal))
        {
            acc_arr[arr_index - 1]->bal -= amount; // deduct amount from current balance
            updateFile(acc_arr);                   // update txt file
            printf("\n---------- Withdraw successful! ----------\n");
            printf("Your new balance is: RM%.2f\n", acc_arr[arr_index - 1]->bal);
            break;
        }
        else
        {
            printf("Invalid! Amount must be more than RM0 but not more than the current balance.\n\n");
            continue;
        }
    }
}

// function of operations to withdraw money from bank account
void withdraw(bankAccount *acc_arr[], int *acc_count)
{
    bankAccount *acc = malloc(sizeof(bankAccount));
    printf("\n---------- WITHDRAW ----------\n");
    if (loadedAccounts > 0)
    {
        loadAccounts(acc_arr, acc_count);
        withdrawBalance(acc_arr, acc, acc_count);
    }
    else
    {
        printf("There are no existing bank accounts!\n");
        return;
    }
}

// transfer money from sender to receiver's bank account
void transferMoney(bankAccount *acc_arr[], bankAccount *acc, int *acc_count)
{
    arr_index = confirmDetails(acc_arr, acc_count); // get sender account
    printf("\n---------- Select Receiver Account ----------\n");
    while (true) // get receiver account
    {
        char input[10];
        printf("Enter index number: ");
        fgets(input, sizeof(input), stdin);
        if (strchr(input, '\n') == NULL)
        {
            int leftoverInput;
            while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                ;
        }
        if (sscanf(input, "%d", &arr_index2) == 1 && arr_index2 > 0 && arr_index2 <= *acc_count)
        {
            // ensure that sender & receiver accounts are different
            if (acc_arr[arr_index - 1]->accNo == acc_arr[arr_index2 - 1]->accNo)
            {
                printf("Invalid! Sender & Receiver accounts must be DISTINCT from each other.\n\n");
                continue;
            }
            else // proceed if both accounts are different
            {
                while (true)
                {
                    double amount;
                    char input[10];
                    printf("Your current balance: RM%.2f\n", acc_arr[arr_index - 1]->bal);
                    if (acc_arr[arr_index - 1]->bal == 0)
                    {
                        printf("Invalid! Your account has no money to transfer!\n");
                        return;
                    }
                    printf("Enter amount to transfer: RM");
                    fgets(input, sizeof(input), stdin);
                    if (strchr(input, '\n') == NULL)
                    {
                        int leftoverInput;
                        while ((leftoverInput = getchar()) != '\n' && leftoverInput != EOF)
                            ;
                    }
                    input[strcspn(input, "\n")] = '\0';
                    if (sscanf(input, "%lf", &amount) == 1 && amount > 0)
                    {
                        double SavingsToCurrentFee = amount * 1.2;
                        double CurrentToSavingsFee = amount * 1.3;
                        // condition for Savings sender to Current receiver
                        if (strcmp(acc_arr[arr_index - 1]->accType, "Savings") == 0 && strcmp(acc_arr[arr_index2 - 1]->accType, "Current") == 0)
                        {
                            printf("Account type of sender is SAVINGS & receiver is CURRENT, 2% remittance fee will be charged.\n");
                            printf("Transferred amount: RM%.2f\n", amount);
                            printf("Remittance fee: RM%.2f\n", (amount * 0.2));
                            printf("Total charged: RM%.2f\n", SavingsToCurrentFee);

                            // if remittance fee exceeds sender's current balance
                            if (SavingsToCurrentFee <= acc_arr[arr_index - 1]->bal)
                            {
                                acc_arr[arr_index - 1]->bal -= SavingsToCurrentFee; // deduct transfer amount + remittance fee from sender's balance
                                acc_arr[arr_index2 - 1]->bal += amount;             // receiver's balance receives transfer amount
                                updateFile(acc_arr);                                // update data in sender's txt file
                                updateReceiverFile(acc_arr);                        // update data in receiver's txt file
                                printf("\n---------- Remittance successful! ----------\n");
                                printf("Sender's new balance is: RM%.2f\n", acc_arr[arr_index - 1]->bal);
                                printf("Receiver's new balance is: RM%.2f\n", acc_arr[arr_index2 - 1]->bal);
                                return;
                            }
                            else
                            {
                                printf("Invalid! Total amount exceeds the current balance.\n\n");
                                continue;
                            }
                        }
                        // condition for Current sender to Savings receiver
                        else if (strcmp(acc_arr[arr_index - 1]->accType, "Current") == 0 && strcmp(acc_arr[arr_index2 - 1]->accType, "Savings") == 0)
                        {
                            printf("Account type of sender is CURRENT & receiver is SAVINGS, 3% remittance fee will be charged.\n");
                            printf("Transferred amount: RM%.2f\n", amount);
                            printf("Remittance fee: RM%.2f\n", (amount * 0.3));
                            printf("Total charged: RM%.2f\n", CurrentToSavingsFee);
                            if (CurrentToSavingsFee <= acc_arr[arr_index - 1]->bal)
                            {
                                acc_arr[arr_index - 1]->bal -= CurrentToSavingsFee;
                                acc_arr[arr_index2 - 1]->bal += amount;
                                updateFile(acc_arr);
                                updateReceiverFile(acc_arr);
                                printf("\n---------- Remittance successful! ----------\n");
                                printf("Sender's new balance is: RM%.2f\n", acc_arr[arr_index - 1]->bal);
                                printf("Receiver's new balance is: RM%.2f\n", acc_arr[arr_index2 - 1]->bal);
                                return;
                            }
                            else
                            {
                                printf("Invalid! Total amount exceeds the current balance.\n\n");
                                continue;
                            }
                        }
                        // condition if sender & receiver have same sccount type (no remittance fee)
                        else
                        {
                            printf("Account type of sender & receiver are the same, no remittance fee will be charged.\n");
                            if (amount <= acc_arr[arr_index - 1]->bal)
                            {
                                acc_arr[arr_index - 1]->bal -= amount;
                                acc_arr[arr_index2 - 1]->bal += amount;
                                updateFile(acc_arr);
                                updateReceiverFile(acc_arr);
                                printf("\n---------- Remittance successful! ----------\n");
                                printf("Sender's new balance is: RM%.2f\n", acc_arr[arr_index - 1]->bal);
                                printf("Receiver's new balance is: RM%.2f\n", acc_arr[arr_index2 - 1]->bal);
                                return;
                            }
                            else
                            {
                                printf("Invalid! Total amount exceeds the current balance.\n\n");
                                continue;
                            }
                        }
                    }
                    else
                    {
                        printf("Invalid! Amount must be more than RM0 but not more than the current balance.\n\n");
                        continue;
                    }
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

// function of operations to transfer money from an account to another
void remittance(bankAccount *acc_arr[], int *acc_count)
{
    bankAccount *acc = malloc(sizeof(bankAccount));
    printf("\n---------- REMITTANCE ----------\n");
    if (loadedAccounts >= 2)
    {
        loadAccounts(acc_arr, acc_count);
        printf("---------- Select Sender Account ----------\n");
        transferMoney(acc_arr, acc, acc_count);
    }
    else
    {
        printf("At least 2 bank accounts are required for remittance!\n");
        return;
    }
}

// main menu that uses user input to execute operations, exit option closes the app
void menu(bankAccount *acc_arr[], int *acc_count)
{
    char option[999];
    while (true)
    {
        printf("\nWelcome to the Banking System App!\n"
               "Please choose an operation:\n"
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
            continue;
        }
        else if (strcmp(option, "2") == 0 || strcmp(option, "delete") == 0)
        {
            deleteAcc(acc_arr, acc_count);
            continue;
        }
        else if (strcmp(option, "3") == 0 || strcmp(option, "deposit") == 0)
        {
            deposit(acc_arr, acc_count);
            continue;
        }
        else if (strcmp(option, "4") == 0 || strcmp(option, "withdraw") == 0)
        {
            withdraw(acc_arr, acc_count);
            continue;
        }
        else if (strcmp(option, "5") == 0 || strcmp(option, "remittance") == 0)
        {
            remittance(acc_arr, acc_count);
            continue;
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

    // check if 'database' directory exists or not
    FILE *db = fopen("database/transaction.log", "a");
    if (db == NULL)
    {
        printf("Error! 'database' directory not found. Please create a folder 'database' in the same directory as this file.\n");
        return 1; // always exit program, until 'database' directory exists
    }
    fclose(db);

    // get current date & time
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    char time[100];
    strftime(time, sizeof(time), "%d-%m-%Y %H:%M:%S", localTime);

    printf("---------- SESSION INFO ----------\n");
    printf("Time: %s\n", time);
    printf("No. of loaded accounts: %d\n", loadedAccounts);

    menu(acc_arr, &acc_count); // launch menu to start program
    return 0;
}
// cd OneDrive/Documents/zbanksys
// .\index.exe