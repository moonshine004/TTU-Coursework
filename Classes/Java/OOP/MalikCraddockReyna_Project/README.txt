Budget Tracker Project README

Personal Finance / Budget Tracker
CS2365 Final Project – Object-Oriented Programming
Texas Tech University

Authors:
Amna Malik
Caden Craddock
Matthew Reyna

Date: April 26, 2026

--

PROJECT DESCRIPTION

This project is a Personal Finance / Budget Tracker application developed using Java and JavaFX. The purpose of the program is to allow users to manage financial activity by tracking income and expenses, setting budget limits, and viewing a summary of financial performance.

The application features a graphical user interface (GUI) that allows users to:

* Add income and expense transactions
* Categorize spending
* Edit savings goals and budget limits
* View real-time financial summaries
* Delete transactions and automatically recalculate totals

The system is designed using object-oriented programming principles including abstraction, encapsulation, inheritance, interfaces, and exception handling.

--

PROGRAM FEATURES

* Interactive JavaFX GUI
* Real-time balance and budget updates
* Transaction tracking (income and expenses)
* Editable savings goal and budget limit
* Monthly summary popup
* Input validation with error handling
* Dynamic recalculation after transaction deletion

--

CLASS STRUCTURE

The program is composed of the following main classes:

* Account (abstract class)
* SavingsAccount (extends Account)
* ExpenseCategory (extends Account)
* Transaction
* BudgetManager (implements Trackable interface)
* BudgetSummary
* Trackable (interface)
* InvalidBudgetException
* BudgetTrackerGUI (main GUI class)

--

REQUIREMENTS

Java JDK (version 8 or higher)
JavaFX SDK

--

FILE SETUP

All .java files must be placed in the same folder when compiling and running the program.

Make sure all source files are downloaded and opened together before attempting to compile or run the program.

If using IntelliJ IDEA, ensure all classes are included in the same project and properly configured.

--

HOW TO COMPILE

javac --module-path "PATH_TO_JAVAFX_LIB" --add-modules javafx.controls *.java

HOW TO RUN

java --module-path "PATH_TO_JAVAFX_LIB" --add-modules javafx.controls BudgetTrackerGUI

Note:
Replace PATH_TO_JAVAFX_LIB with the path to the "lib" folder inside your JavaFX SDK.

Example:
C:\javafx-sdk-21\lib

--

HOW TO USE

* Select transaction type (Income or Expense)
* Enter a category name
* Enter an amount
* Click "Add Transaction"

Additional actions:

* "Delete Selected" removes a transaction
* "Show Summary" displays a monthly summary
* Edit buttons allow updating savings goal and budget limit

The application will open as a JavaFX GUI window after running the main class.

--

EXCEPTION HANDLING

The program uses a custom exception class (InvalidBudgetException) to handle invalid inputs such as:

* Empty transaction type
* Invalid transaction type
* Negative or zero amounts
* Empty category names

Additional exceptions (such as NumberFormatException) are handled in the GUI to prevent crashes and improve user experience.

--

LIMITATIONS

* Supports only a single account
* Transactions cannot be edited once added
* Data is not saved between sessions
* Limited financial analytics

--

FUTURE IMPROVEMENTS

* Support multiple accounts
* Add persistent storage (file/database)
* Allow editing transactions
* Add charts and visual analytics
* Expand to multiple budget categories

--

NOTES

This project was designed to demonstrate core object-oriented programming principles while maintaining clean and modular code. The GUI enhances usability and provides a real-time interactive experience for financial tracking.