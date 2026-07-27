import java.util.ArrayList;

/**
 * Manages accounts and transactions.
 * Implements the Trackable interface to handle transaction tracking
 * and provide summary information.
 */
public class BudgetManager implements Trackable {

    /** List to store all accounts */
    private ArrayList<Account> accounts;

    /** List to store all transactions */
    private ArrayList<Transaction> transactions;

    /**
     * Constructs a BudgetManager with empty account and transaction lists.
     */
    public BudgetManager() {
        accounts = new ArrayList<>();
        transactions = new ArrayList<>();
    }

    /**
     * Adds an account to the list.
     *
     * @param account the account to add
     * @throws InvalidBudgetException if the account is null
     */
    public void addAccount(Account account) throws InvalidBudgetException {

        if (account == null) {
            throw new InvalidBudgetException("Account cannot be null.");
        }

        accounts.add(account);
    }

    /**
     * Adds a transaction and updates the account balance.
     *
     * @param transaction the transaction to add
     * @throws InvalidBudgetException if the transaction is null
     */
    @Override
    public void addTransaction(Transaction transaction) throws InvalidBudgetException {

        if (transaction == null) {
            throw new InvalidBudgetException("Transaction cannot be null.");
        }

        transactions.add(transaction);

        // Update balance based on transaction type
        if (transaction.getType().equalsIgnoreCase("income")) {
            accounts.get(0).deposit(transaction.getAmount());
        } else if (transaction.getType().equalsIgnoreCase("expense")) {
            accounts.get(0).withdraw(transaction.getAmount());
        }
    }

    /**
     * Calculates and prints the monthly income, expenses, and net total.
     */
    public void calculateMonthlySummary() {

        double totalIncome = 0;
        double totalExpenses = 0;

        for (Transaction transaction : transactions) {
            if (transaction.getType().equalsIgnoreCase("income")) {
                totalIncome += transaction.getAmount();
            } else if (transaction.getType().equalsIgnoreCase("expense")) {
                totalExpenses += transaction.getAmount();
            }
        }

        System.out.println("Total Income: $" + totalIncome);
        System.out.println("Total Expenses: $" + totalExpenses);
        System.out.println("Net Total: $" + (totalIncome - totalExpenses));
    }

    /**
     * Checks the budget status.
     * (Currently a placeholder for future implementation.)
     */
    public void checkBudgetStatus() {
        System.out.println("Budget status checked.");
    }

    /**
     * Returns a summary of the number of accounts and transactions.
     *
     * @return summary string
     */
    @Override
    public String getSummary() {
        return "Accounts: " + accounts.size() +
                "\nTransactions: " + transactions.size();
    }
}