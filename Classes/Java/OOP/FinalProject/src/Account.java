/**
 * Represents a general financial account.
 * This abstract class provides basic account functionality
 * such as storing a name and balance, and performing deposits and withdrawals.
 */
public abstract class Account {

    /** Stores the account name */
    private String name;

    /** Stores the account balance */
    private double balance;

    /**
     * Constructs an Account with a name and initial balance.
     *
     * @param name the name of the account
     * @param balance the starting balance of the account
     */
    public Account(String name, double balance) {
        this.name = name;
        this.balance = balance;
    }

    /**
     * Adds money to the account balance.
     *
     * @param amount the amount to deposit
     */
    public void deposit(double amount) {
        balance += amount;
    }

    /**
     * Subtracts money from the account balance.
     *
     * @param amount the amount to withdraw
     */
    public void withdraw(double amount) {
        balance -= amount;
    }

    /**
     * Returns the account name.
     *
     * @return the account name
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the current account balance.
     *
     * @return the account balance
     */
    public double getBalance() {
        return balance;
    }

    /**
     * Updates the account name.
     *
     * @param name the new account name
     */
    public void setName(String name) {
        this.name = name;
    }
}