/**
 * Custom exception used for invalid budget-related inputs.
 * This exception is thrown when user input violates validation rules.
 */
public class InvalidBudgetException extends Exception {

    /**
     * Constructs an InvalidBudgetException with a specific error message.
     *
     * @param message the error message explaining what went wrong
     */
    public InvalidBudgetException(String message) {

        // Calls the parent Exception class constructor
        super(message);
    }
}