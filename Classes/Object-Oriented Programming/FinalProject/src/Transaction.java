/**
 * Represents a single income or expense transaction.
 * Includes validation to ensure all transaction data is valid.
 */
public class Transaction {

    /** Stores the type of transaction (Income or Expense) */
    private String type;

    /** Stores the transaction amount */
    private double amount;

    /** Stores the category name (e.g., Food, Rent) */
    private String categoryName;

    /**
     * Constructs a Transaction with validation.
     *
     * @param type the type of transaction (income or expense)
     * @param amount the transaction amount
     * @param categoryName the category of the transaction
     * @throws InvalidBudgetException if any input is invalid
     */
    public Transaction(String type, double amount, String categoryName)
            throws InvalidBudgetException {

        // Check if transaction type is empty
        if (type == null || type.trim().isEmpty()) {
            throw new InvalidBudgetException("Transaction type cannot be empty.");
        }

        // Check if type is valid (must be income or expense)
        if (!type.equalsIgnoreCase("income") && !type.equalsIgnoreCase("expense")) {
            throw new InvalidBudgetException("Transaction type must be income or expense.");
        }

        // Check if amount is valid (must be greater than 0)
        if (amount <= 0) {
            throw new InvalidBudgetException("Transaction amount must be greater than zero.");
        }

        // Check if category name is empty
        if (categoryName == null || categoryName.trim().isEmpty()) {
            throw new InvalidBudgetException("Category name cannot be empty.");
        }

        // Assign values after validation passes
        this.type = type;
        this.amount = amount;
        this.categoryName = categoryName;
    }

    /**
     * Returns the transaction type.
     *
     * @return the transaction type
     */
    public String getType() {
        return type;
    }

    /**
     * Returns the transaction amount.
     *
     * @return the transaction amount
     */
    public double getAmount() {
        return amount;
    }

    /**
     * Returns the category name.
     *
     * @return the category name
     */
    public String getCategoryName() {
        return categoryName;
    }
}