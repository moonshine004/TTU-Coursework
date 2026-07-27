/**
 * Defines tracking behavior for classes that manage transactions.
 * Any class that implements this interface must provide methods
 * to add transactions and generate a summary.
 */
public interface Trackable {

    /**
     * Adds a transaction to the system.
     *
     * @param transaction the transaction to add
     * @throws InvalidBudgetException if the transaction is invalid
     */
    void addTransaction(Transaction transaction) throws InvalidBudgetException;

    /**
     * Returns a summary of the tracked data.
     *
     * @return a summary string
     */
    String getSummary();
}