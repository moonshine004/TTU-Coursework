package parts.part2;

import java.util.Scanner;

/**
 * ChargeAccountValidator validates charge account numbers by comparing
 * a provided number against a list of known valid account numbers.
 */
class ChargeAccountValidator {

    // Stores the list of valid charge account numbers.
    private int[] validAccounts;

    /**
     * Constructs a ChargeAccountValidator and loads the valid account list.
     * Parameters:
     * - none
     * Intended use:
     * - Initializes the validator with the required set of valid account numbers.
     */
    public ChargeAccountValidator() {

        // Initializes the array of valid charge account numbers.
        validAccounts = new int[] {
                5658845, 4520125, 7895122, 8777541, 8451277, 1302850,
                8080152, 4562555, 5552012, 5050552, 7825877, 1250255,
                1005231, 6545231, 3852085, 7576651, 7881200, 4581002
        };
    }

    /**
     * Determines whether an account number is valid using linear search.
     * Parameters:
     * - accountNumber: the account number to validate
     * Intended use:
     * - Returns true if accountNumber exists in validAccounts, otherwise false.
     */
    public boolean isValid(int accountNumber) {

        // Loops through each valid account number.
        for (int i = 0; i < validAccounts.length; i++) { // i is the index used to search the array.
            // Checks if the current stored account matches the input.
            if (validAccounts[i] == accountNumber) {
                // Returns true immediately if found.
                return true;
            }
        }

        // Returns false if the number was not found.
        return false;
    }
}

/**
 * ChargeAccountDemo prompts the user to enter an account number and
 * reports whether it is valid using ChargeAccountValidator.
 */
public class ChargeAccountDemo {

    /**
     * Program entry point.
     * Parameters:
     * - args: command-line arguments (unused)
     * Intended use:
     * - Reads an account number from the user with validation and prints valid or invalid.
     */
    public static void main(String[] args) {

        // input reads user input from the keyboard.
        Scanner input = new Scanner(System.in); // input is used to read keyboard input.

        // validator checks whether an account number exists in the valid list.
        ChargeAccountValidator validator = new ChargeAccountValidator(); // validator performs the validation.

        // accountNumber stores the numeric account number entered by the user.
        int accountNumber = 0; // accountNumber holds the user's valid integer input.

        // isInputValid controls the input validation loop.
        boolean isInputValid = false; // isInputValid is true only after a valid integer is read.

        // Keeps asking until a valid integer is entered.
        while (!isInputValid) {

            // Prompts the user to enter an account number.
            System.out.print("Enter a charge account number: ");

            // Checks whether the next token is an integer.
            if (input.hasNextInt()) {
                accountNumber = input.nextInt();
                isInputValid = true;
            } else {
                // badInput stores the invalid token so we can consume it.
                String badInput = input.next(); // badInput removes the invalid entry from the scanner.
                System.out.println("Invalid input. Please enter a numeric charge account number.");
            }
        }

        // valid stores whether the account number exists in the list.
        boolean valid = validator.isValid(accountNumber); // valid is true if accountNumber was found.

        // Prints the appropriate result message.
        if (valid) {
            System.out.println("Valid, the charge account number exists.");
        } else {
            System.out.println("Invalid, the charge account number does not exist.");
        }

        // Closes the scanner.
        input.close();
    }
}