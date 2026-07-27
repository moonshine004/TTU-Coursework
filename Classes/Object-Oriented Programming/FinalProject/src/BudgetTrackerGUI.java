import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.*;
import javafx.stage.Stage;

/**
 * Main JavaFX class that creates the Budget Tracker window.
 * This class handles:
 * - Adding income and expense transactions
 * - Deleting selected transactions
 * - Editing the savings goal
 * - Editing the budget limit
 * - Showing savings progress with a progress bar
 * - Displaying budget summary information
 */
public class BudgetTrackerGUI extends Application {

    /** Object used to calculate budget status and monthly summaries */
    private BudgetSummary summary = new BudgetSummary();

    /** List that stores all transactions shown in the table */
    private final ObservableList<Transaction> transactionList = FXCollections.observableArrayList();

    /** Main savings account object */
    private SavingsAccount savingsAccount;

    /** Expense category object used for budget tracking */
    private ExpenseCategory expenseCategory;

    /** Labels used to display summary information */
    private final Label balanceLabel = new Label();
    private final Label goalLabel = new Label();
    private final Label spentLabel = new Label();
    private final Label budgetLabel = new Label();
    private final Label statusLabel = new Label();

    /** Progress bar used to show savings progress */
    private final ProgressBar savingsProgressBar = new ProgressBar();

    /** Label displayed on top of the progress bar */
    private final Label progressPercentLabel = new Label();

    /**
     * Starts the JavaFX application and builds the main GUI window.
     *
     * @param primaryStage the main application window
     */
    @Override
    public void start(Stage primaryStage) {

        // Create starting account and budget data
        savingsAccount = new SavingsAccount("Main Savings", 0.0, 2000.0);
        expenseCategory = new ExpenseCategory("General Expenses", 0.0, 500.0);

        // Create title label
        Label titleLabel = new Label("Personal Finance / Budget Tracker");
        titleLabel.setStyle("-fx-font-size: 20px; -fx-font-weight: bold;");

        // Dropdown box for choosing income or expense
        ComboBox<String> typeBox = new ComboBox<>();
        typeBox.getItems().addAll("Income", "Expense");
        typeBox.setValue("Expense");

        // Text field for category name
        TextField categoryField = new TextField();
        categoryField.setPromptText("Category Name");

        // Text field for transaction amount
        TextField amountField = new TextField();
        amountField.setPromptText("Amount");

        // Buttons for adding, deleting, and showing summary
        Button addButton = new Button("Add Transaction");
        Button deleteButton = new Button("Delete Selected");
        Button summaryButton = new Button("Show Summary");

        // Hidden text field used when editing the savings goal
        TextField goalField = new TextField();
        goalField.setPromptText("New Savings Goal");
        goalField.setVisible(false);
        goalField.setManaged(false);

        // Pencil button used to edit the savings goal
        Button updateGoalButton = new Button("✎");
        updateGoalButton.setPrefWidth(40);
        updateGoalButton.setTooltip(new Tooltip("Edit Savings Goal"));

        // Hidden text field used when editing the budget limit
        TextField budgetField = new TextField();
        budgetField.setPromptText("New Budget Limit");
        budgetField.setVisible(false);
        budgetField.setManaged(false);

        // Pencil button used to edit the budget limit
        Button updateBudgetButton = new Button("✎");
        updateBudgetButton.setPrefWidth(40);
        updateBudgetButton.setTooltip(new Tooltip("Edit Budget"));

        // Create table and connect it to the transaction list
        TableView<Transaction> tableView = createTableView();
        tableView.setItems(transactionList);

        // Runs when the Add Transaction button is clicked
        addButton.setOnAction(e -> {
            try {
                String type = typeBox.getValue();
                String category = categoryField.getText().trim();
                String amountText = amountField.getText().trim();

                if (category.isEmpty()) {
                    showAlert("Input Error", "Please enter a category name.");
                    return;
                }

                double amount = Double.parseDouble(amountText);

                if (amount <= 0) {
                    showAlert("Input Error", "Amount must be greater than 0.");
                    return;
                }

                // Create and add transaction to the table
                Transaction transaction = new Transaction(type, amount, category);
                transactionList.add(transaction);

                // Income adds to savings, expenses subtract from savings
                if (type.equalsIgnoreCase("Income")) {
                    savingsAccount.deposit(amount);
                } else {
                    savingsAccount.withdraw(amount);
                    expenseCategory.addSpentAmount(amount);
                }

                updateSummaryLabels();

                categoryField.clear();
                amountField.clear();

            } catch (InvalidBudgetException ex) {
                showAlert("Input Error", ex.getMessage());
            } catch (NumberFormatException ex) {
                showAlert("Input Error", "Please enter a valid number.");
            }
        });

        // Runs when the edit savings goal button is clicked
        updateGoalButton.setOnAction(e -> {
            if (!goalField.isVisible()) {

                // Show text field so user can enter a new goal
                goalField.setVisible(true);
                goalField.setManaged(true);
                goalField.setText(String.valueOf(savingsAccount.getSavingsGoal()));
                updateGoalButton.setText("✔");

            } else {
                try {
                    double newGoal = Double.parseDouble(goalField.getText().trim());

                    if (newGoal <= 0) {
                        showAlert("Input Error", "Savings goal must be greater than 0.");
                        return;
                    }

                    // Update savings goal
                    savingsAccount.setSavingsGoal(newGoal);
                    updateSummaryLabels();

                    // Hide field again after updating
                    goalField.setVisible(false);
                    goalField.setManaged(false);
                    updateGoalButton.setText("✎");

                } catch (NumberFormatException ex) {
                    showAlert("Input Error", "Please enter a valid savings goal.");
                }
            }
        });

        // Runs when the edit budget button is clicked
        updateBudgetButton.setOnAction(e -> {
            if (!budgetField.isVisible()) {

                // Show text field so user can enter a new budget limit
                budgetField.setVisible(true);
                budgetField.setManaged(true);
                budgetField.setText(String.valueOf(expenseCategory.getBudgetLimit()));
                updateBudgetButton.setText("✔");

            } else {
                try {
                    double newLimit = Double.parseDouble(budgetField.getText().trim());

                    if (newLimit <= 0) {
                        showAlert("Input Error", "Budget limit must be greater than 0.");
                        return;
                    }

                    // Update budget limit
                    expenseCategory.setBudgetLimit(newLimit);
                    updateSummaryLabels();

                    // Hide field again after updating
                    budgetField.setVisible(false);
                    budgetField.setManaged(false);
                    updateBudgetButton.setText("✎");

                } catch (NumberFormatException ex) {
                    showAlert("Input Error", "Please enter a valid budget amount.");
                }
            }
        });

        // Runs when the Delete Selected button is clicked
        deleteButton.setOnAction(e -> {
            Transaction selected = tableView.getSelectionModel().getSelectedItem();

            if (selected == null) {
                showAlert("Selection Error", "Please select a transaction to delete.");
                return;
            }

            // Remove selected transaction and recalculate totals
            transactionList.remove(selected);
            recalculateTotals();
            updateSummaryLabels();
        });

        // Shows full monthly summary in a popup
        summaryButton.setOnAction(e -> {
            showAlert("Monthly Summary",
                    summary.generateSummary(savingsAccount, expenseCategory));
        });

        // Create grid layout for input fields
        GridPane inputPane = new GridPane();
        inputPane.setHgap(10);
        inputPane.setVgap(10);
        inputPane.add(new Label("Type:"), 0, 0);
        inputPane.add(typeBox, 1, 0);
        inputPane.add(new Label("Category:"), 0, 1);
        inputPane.add(categoryField, 1, 1);
        inputPane.add(new Label("Amount:"), 0, 2);
        inputPane.add(amountField, 1, 2);

        // Put main action buttons in one row
        HBox buttonBox = new HBox(10, addButton, deleteButton, summaryButton);
        buttonBox.setAlignment(Pos.CENTER_LEFT);

        // Left side contains input fields and buttons
        VBox leftPanel = new VBox(15, inputPane, buttonBox);
        leftPanel.setPadding(new Insets(15));

        // Put savings goal label and edit button together
        HBox goalEditRow = new HBox(10, goalLabel, updateGoalButton);
        goalEditRow.setAlignment(Pos.CENTER_RIGHT);

        // Put budget label and edit button together
        HBox budgetRow = new HBox(10, budgetLabel, updateBudgetButton);
        budgetRow.setAlignment(Pos.CENTER_LEFT);

        // Style the savings progress bar
        savingsProgressBar.setPrefWidth(430);
        savingsProgressBar.setPrefHeight(25);
        savingsProgressBar.setStyle("-fx-accent: red;");

        // Style progress text
        progressPercentLabel.setStyle("-fx-text-fill: white; -fx-font-weight: bold;");

        // StackPane places the percentage label on top of the progress bar
        StackPane progressStack = new StackPane();
        progressStack.getChildren().addAll(savingsProgressBar, progressPercentLabel);

        // This row keeps savings balance on the left and savings goal on the right
        HBox balanceGoalRow = new HBox();
        balanceGoalRow.setAlignment(Pos.CENTER);
        balanceGoalRow.setPadding(new Insets(8, 0, 5, 0));

        Region spacer = new Region();
        HBox.setHgrow(spacer, Priority.ALWAYS);

        balanceGoalRow.getChildren().addAll(balanceLabel, spacer, goalEditRow);

        // Progress section goes at the top of the summary box
        VBox progressBox = new VBox(10, progressStack, balanceGoalRow);
        progressBox.setAlignment(Pos.CENTER);

        // Summary panel displays progress, spending, budget, and status
        VBox summaryBox = new VBox(10,
                progressBox,
                goalField,
                spentLabel,
                budgetRow,
                budgetField,
                statusLabel
        );

        summaryBox.setPadding(new Insets(15));
        summaryBox.setStyle("-fx-border-color: gray; -fx-border-width: 1;");

        // Right side contains table and summary panel
        VBox rightPanel = new VBox(15, tableView, summaryBox);
        rightPanel.setPadding(new Insets(15));
        VBox.setVgrow(tableView, Priority.ALWAYS);

        // Display starting summary values after GUI components are created
        updateSummaryLabels();

        // Main layout for the full window
        BorderPane root = new BorderPane();
        root.setTop(wrapTop(titleLabel));
        root.setLeft(leftPanel);
        root.setCenter(rightPanel);
        root.setPadding(new Insets(15));

        // Create and show the window
        Scene scene = new Scene(root, 900, 550);
        primaryStage.setTitle("Budget Tracker");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    /**
     * Creates the table used to display income and expense transactions.
     *
     * @return the completed transaction table
     */
    private TableView<Transaction> createTableView() {
        TableView<Transaction> table = new TableView<>();

        // Type column shows Income or Expense
        TableColumn<Transaction, String> typeCol = new TableColumn<>("Type");
        typeCol.setCellValueFactory(new PropertyValueFactory<>("type"));
        typeCol.setPrefWidth(120);

        // Amount column shows the transaction amount
        TableColumn<Transaction, Double> amountCol = new TableColumn<>("Amount");
        amountCol.setCellValueFactory(new PropertyValueFactory<>("amount"));
        amountCol.setPrefWidth(120);

        // Category column shows the transaction category name
        TableColumn<Transaction, String> categoryCol = new TableColumn<>("Category");
        categoryCol.setCellValueFactory(new PropertyValueFactory<>("categoryName"));
        categoryCol.setPrefWidth(220);

        table.getColumns().addAll(typeCol, amountCol, categoryCol);

        return table;
    }

    /**
     * Updates the savings progress bar and all summary labels.
     * This method is called after adding, deleting, or editing values.
     */
    private void updateSummaryLabels() {
        double balance = savingsAccount.getBalance();
        double goal = savingsAccount.getSavingsGoal();

        // Calculate savings progress as a decimal value
        double progress = 0.0;

        if (goal > 0) {
            progress = balance / goal;
        }

        // ProgressBar only accepts values from 0.0 to 1.0
        savingsProgressBar.setProgress(Math.min(progress, 1.0));

        // Update progress bar text
        progressPercentLabel.setText(String.format("%.1f%%", progress * 100));

        // Update regular summary labels
        balanceLabel.setText(String.format("Savings Balance: $%.2f", balance));
        goalLabel.setText(String.format("Savings Goal: $%.2f", goal));
        spentLabel.setText(String.format("Spent Amount: $%.2f", expenseCategory.getSpentAmount()));
        budgetLabel.setText(String.format("Budget Limit: $%.2f", expenseCategory.getBudgetLimit()));
        statusLabel.setText("Budget Status: " + summary.getBudgetStatus(expenseCategory));
    }

    /**
     * Recalculates account balance and spending after a transaction is deleted.
     * This keeps totals correct because removing a transaction should also undo
     * its effect on the savings balance and spent amount.
     */
    private void recalculateTotals() {
        double currentBudgetLimit = expenseCategory.getBudgetLimit();
        double currentSavingsGoal = savingsAccount.getSavingsGoal();

        // Reset account and category while keeping edited goal and budget values
        savingsAccount = new SavingsAccount("Main Savings", 0.0, currentSavingsGoal);
        expenseCategory = new ExpenseCategory("General Expenses", 0.0, currentBudgetLimit);

        // Add each remaining transaction back into the totals
        for (Transaction transaction : transactionList) {
            if (transaction.getType().equalsIgnoreCase("Income")) {
                savingsAccount.deposit(transaction.getAmount());
            } else {
                savingsAccount.withdraw(transaction.getAmount());
                expenseCategory.addSpentAmount(transaction.getAmount());
            }
        }
    }

    /**
     * Centers the title label at the top of the window.
     *
     * @param titleLabel the title label to display
     * @return an HBox containing the centered title
     */
    private HBox wrapTop(Label titleLabel) {
        HBox box = new HBox(titleLabel);
        box.setAlignment(Pos.CENTER);
        box.setPadding(new Insets(10));
        return box;
    }

    /**
     * Displays a popup message for errors or summaries.
     *
     * @param title the alert window title
     * @param message the alert message text
     */
    private void showAlert(String title, String message) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }

    /**
     * Launches the JavaFX application.
     *
     * @param args command-line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
}