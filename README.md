# banking_system
Banking system application made with C, part of programming coursework.

# How to run & use this project
Before running 'index.c', please ensure 'database' folder exists in the same directory as 'index.c', it is needed to run the program. Download the folder if it doesn't exist.
To run the program, enter in terminal: cd Downloads/banking_system_36400092
Afterwards, enter in terminal: ./index.c 

# Main Menu
Upon startup, user is prompted with the main menu, you can choose an operation by entering the numbers or LOWERCASE words specified.
To create an account, enter: "1" or "create"
To delete an account, enter: "2" or "delete"
To deposit money in account, enter: "3" or "deposit"
To withdraw money from account, enter: "4" or "withdraw"
To transfer money between accounts, enter: "5" or "remittance"
To exit the program, enter: "6" or "exit"

# Creating New Bank Account
To create an account, note that you CANNOT cancel the process. Complete a 4-step process to proceed.
First, enter your name only in LETTERS (e.g. John Doe, Jane Doe)
Second, enter your ID number of 8 DIGITS. (e.g. 67676767, 12345678)
Third, choose your account type by entering: "Savings" or "Current"
Lastly, enter your 4-digit PIN (e.g. 6767, 1234)
Once account is created, a .txt file named after the generated account number will be stored in 'database' folder. The file contains all details of that individual account.

# Delete Bank Account
To delete an account, choose from the list of loaded accounts by entering index number starting from "1" onwards. 
If there are no currently existing accounts, you will be returned to the main menu.
Once an account is chosen, confirm if the last 4 digits of your ID matches the program's prompt by entering: "y" or "n"
Complete the process by entering the 4-digit PIN identical to that account's PIN.
To cancel the process and return to menu, enter "C"
Once validated, the file storing the account's contents and its memory will be removed from the 'database' directory and the program entirely.

# Deposit Money into Bank Account
To deposit, choose from the list of loaded accounts by entering index number starting from "1" onwards. 
If there are no currently existing accounts, you will be returned to the main menu.
Once an account is chosen, confirm if the last 4 digits of your ID matches the program's prompt by entering: "y" or "n"
Complete the process by entering the 4-digit PIN identical to that account's PIN.
Deposit money by entering a number MORE than 0, but max number is 50000 (No commas)
To cancel the process and return to menu, enter "C"
Once validated, the file of the depositted account is rewritten with the updated balance.

# Withdraw Money from Bank Account
To withdraw, choose from the list of loaded accounts by entering index number starting from "1" onwards. 
If there are no currently existing accounts, you will be returned to the main menu.
Once an account is chosen, confirm if the last 4 digits of your ID matches the program's prompt by entering: "y" or "n"
Complete the process by entering the 4-digit PIN identical to that account's PIN.
Withdraw money by entering a number MORE than 0.
If the withdraw amount exceeds the current balance, or the current balance is 0 (insufficient), you will be returned back to the main menu.
To cancel the process and return to menu, enter "C"
Once validated, the file of the withdrawn account is rewritten with the updated balance.

# Remittance
To transfer money, choose SENDER account from the list of loaded accounts by entering index number starting from "1" onwards. 
If there are no currently existing accounts, you will be returned to the main menu.
Once an account is chosen, confirm if the last 4 digits of your ID matches the program's prompt by entering: "y" or "n"
Complete the process by entering the 4-digit PIN identical to that account's PIN.

Once Sender account is validated, choose RECEIVER account from the list of loaded accounts by entering index number starting from "1" onwards. 
Note that Sender and Receiver accounts must be different.
Transfer money by entering a number MORE than 0.
If the transfer amount exceeds the Sender's current balance, or the Sender's current balance is 0 (insufficient), you will be returned back to the main menu.
Note that if account type of Sender is SAVINGS, but Receiver is CURRENT, Sender is charged an extra 2% of their transfer amount.
Note that if account type of Sender is CURRENT, but Receiver is SAVINGS, Sender is charged an extra 3% of their transfer amount.
No remittance fee is charged if both accounts have the same type.
To cancel the process and return to menu, enter "C"
Once validated, both files are rewritten with the updated balance.

# Other Info
All account action changes will be stored in a file called 'transaction.log' located in the 'database' folder.
Once an operation has been completed, user will always be returned to the main menu.
The main menu will always loop until user has called the exit function by entering "6" or "exit".
