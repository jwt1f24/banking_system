# banking_system
Banking system application made with C, part of programming coursework.

# How to use this project
Before running 'index.c', Please download the 'database' folder as it is required to run the program.
Once 'database' folder exists in the same directory as 'index.c', run 'index.c'.

Upon startup, user is prompted with the main menu interface, you can choose an operation by entering the numbers or lowercase words stated in the bracket.
When creating an account, note that you CANNOT cancel the process. 
You must enter your name only in letters, ID number of 8 digits, choose between a Savings or Current account, and a 4-digit PIN number.
Once account is created, a .txt file named after the generated account number will be stored in 'database' folder. The file contains all details of that individual account.

When deleting an account, depositing or withdrawing money, or transferring money to another account, please choose from the list of loaded accounts. If there are no currently existing accounts, you will be returned to the main menu.
When deleting an existing account, confirm all account credentials before proceeding. Once validated, the file storing the account contents and its memory will be removed from the directory and program entirely.

When depositing or withdrawing money from an existing account, confirm all account credentials to proceed. Once validated, the file storing the account contents will be rewritten along with the updated balance.
If you want to withdraw but the selected account has insufficient balance, you will be returned back to the main menu.

When transferring money from one account to another, you must choose the sender account first. Once credentials are validated, choose the receiver account. Make sure sender and receiver accounts are different individual accounts.
In terms of account types, if sender is SAVINGS, but receiver is CURRENT, the receiver will receive the same amount entered by the sender, but sender will be charged a remittance fee of an extra 2% of the amount they've entered.
If sender is CURRENT, but receiver is SAVINGS, sender is charged a remittance fee of 3% instead.
If both accounts have the same type, no remittance fee will be charged.

Note that you can cancel an ongoing process for most operations except account creation.
All account action changes will be stored in a file called 'transaction.log' located in the 'database' folder.
It is not necessary to manually create 'transaction.log', as it will automatically be created when you run the program after 'database' folder exists.

Once an operation has been completed, user will always be returned to the main menu.
The main menu will always loop unless user has entered the exit function.
