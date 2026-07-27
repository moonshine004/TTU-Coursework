/**
 * Represents an expense category used for budget tracking.
 * This class extends Account and adds budget limit and spending tracking.
 */
public class ExpenseCategory extends Account {

    /** Stores the budget limit for this category */
    private double budgetLimit;

    /** Stores how much has been spent so far */
    private double spentAmount;

    /**
     * Constructs an ExpenseCategory with a name, balance, and budget limit.
     *
     * @param name the category name
     * @param balance the starting balance
     * @param budgetLimit the maximum allowed spending
     */
    public ExpenseCategory(String name, double balance, double budgetLimit) {
        super(name, balance);   // Calls parent Account constructor
        this.budgetLimit = budgetLimit;
        this.spentAmount = 0.0; // Start with no spending
    }

    /**
     * Updates the budget limit.
     *
     * @param budgetLimit the new budget limit
     */
    public void setBudgetLimit(double budgetLimit) {
        this.budgetLimit = budgetLimit;
    }

    /**
     * Returns the current budget limit.
     *
     * @return the budget limit
     */
    public double getBudgetLimit() {
        return budgetLimit;
    }

    /**
     * Returns how much has been spent.
     *
     * @return the total spent amount
     */
    public double getSpentAmount() {
        return spentAmount;
    }

    /**
     * Adds to the total spent amount.
     *
     * @param amount the amount to add to spending
     */
    public void addSpentAmount(double amount) {
        spentAmount += amount;
    }
}