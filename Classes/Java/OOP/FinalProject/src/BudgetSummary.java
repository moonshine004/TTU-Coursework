/**
 * Handles budget calculations and generates summary output.
 * Provides methods to calculate remaining budget, determine budget status,
 * and generate a formatted monthly summary.
 */
public class BudgetSummary {

    /**
     * Calculates how much budget is remaining.
     *
     * @param category the expense category
     * @return remaining budget amount
     */
    public double getRemainingBudget(ExpenseCategory category) {
        return category.getBudgetLimit() - category.getSpentAmount();
    }

    /**
     * Determines whether the user is over or under budget.
     *
     * @param category the expense category
     * @return a string indicating budget status
     */
    public String getBudgetStatus(ExpenseCategory category) {

        // Get remaining budget
        double remaining = getRemainingBudget(category);

        // If negative, user is over budget
        if (remaining < 0) {
            return "Over budget by $" + String.format("%.2f", Math.abs(remaining));
        }
        // Otherwise, user is under budget
        else {
            return "Under budget by $" + String.format("%.2f", remaining);
        }
    }

    /**
     * Generates a full monthly summary as a formatted string.
     *
     * @param savingsAccount the savings account
     * @param category the expense category
     * @return formatted summary string
     */
    public String generateSummary(SavingsAccount savingsAccount, ExpenseCategory category) {

        return "Monthly Summary\n"
                // Shows current savings balance
                + "Savings Balance: $" + String.format("%.2f", savingsAccount.getBalance()) + "\n"

                // Shows total amount spent
                + "Spent Amount: $" + String.format("%.2f", category.getSpentAmount()) + "\n"

                // Shows budget limit
                + "Budget Limit: $" + String.format("%.2f", category.getBudgetLimit()) + "\n"

                // Shows remaining budget
                + "Remaining Budget: $" + String.format("%.2f", getRemainingBudget(category)) + "\n"

                // Shows budget status (over/under)
                + "Budget Status: " + getBudgetStatus(category);
    }
}