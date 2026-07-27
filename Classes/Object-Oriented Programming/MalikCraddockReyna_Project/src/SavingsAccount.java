/**
 * Represents a savings account.
 * This class extends Account and adds a savings goal.
 */
public class SavingsAccount extends Account {

    /** Stores the savings goal for this account */
    private double savingsGoal;

    /**
     * Constructs a SavingsAccount with a name, balance, and savings goal.
     *
     * @param name the name of the savings account
     * @param balance the starting balance
     * @param savingsGoal the savings goal amount
     */
    public SavingsAccount(String name, double balance, double savingsGoal) {
        super(name, balance); // Calls parent Account constructor
        this.savingsGoal = savingsGoal;
    }

    /**
     * Updates the savings goal.
     *
     * @param savingsGoal the new savings goal amount
     */
    public void setSavingsGoal(double savingsGoal) {
        this.savingsGoal = savingsGoal;
    }

    /**
     * Returns the current savings goal.
     *
     * @return the savings goal
     */
    public double getSavingsGoal() {
        return savingsGoal;
    }
}