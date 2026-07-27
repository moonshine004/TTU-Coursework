package q2;

/**
 * The Month class stores the number of a month and provides
 * methods to get the month name and compare months.
 */
public class Month {

    // Stores the number of the month.
    private int monthNumber;

    /**
     * No-arg constructor sets monthNumber to 1.
     */
    public Month() {
        monthNumber = 1;
    }

    /**
     * Constructor that accepts a month number.
     * If invalid, sets to 1.
     *
     * @param month the month number
     */
    public Month(int month) {
        if (month >= 1 && month <= 12) {
            monthNumber = month;
        } else {
            monthNumber = 1;
        }
    }

    /**
     * Constructor that accepts a month name.
     *
     * @param name the month name
     */
    public Month(String name) {
        setMonthFromName(name);
    }

    /**
     * Sets the month number.
     *
     * @param month the month number
     */
    public void setMonthNumber(int month) {
        if (month >= 1 && month <= 12) {
            monthNumber = month;
        } else {
            monthNumber = 1;
        }
    }

    /**
     * Returns the month number.
     *
     * @return month number
     */
    public int getMonthNumber() {
        return monthNumber;
    }

    /**
     * Returns the name of the month.
     *
     * @return month name
     */
    public String getMonthName() {
        String[] months = {
                "January", "February", "March", "April",
                "May", "June", "July", "August",
                "September", "October", "November", "December"
        };
        return months[monthNumber - 1];
    }

    /**
     * Returns the month name.
     *
     * @return month name
     */
    @Override
    public String toString() {
        return getMonthName();
    }

    /**
     * Checks if months are equal.
     *
     * @param other another Month object
     * @return true if equal
     */
    public boolean equals(Month other) {
        return this.monthNumber == other.monthNumber;
    }

    /**
     * Checks if this month is greater.
     *
     * @param other another Month object
     * @return true if greater
     */
    public boolean greaterThan(Month other) {
        return this.monthNumber > other.monthNumber;
    }

    /**
     * Checks if this month is less.
     *
     * @param other another Month object
     * @return true if less
     */
    public boolean lessThan(Month other) {
        return this.monthNumber < other.monthNumber;
    }

    /**
     * Converts a month name to its number.
     *
     * @param name the month name
     */
    private void setMonthFromName(String name) {
        String[] months = {
                "January", "February", "March", "April",
                "May", "June", "July", "August",
                "September", "October", "November", "December"
        };

        for (int i = 0; i < months.length; i++) {
            if (months[i].equalsIgnoreCase(name)) {
                monthNumber = i + 1;
                return;
            }
        }

        monthNumber = 1;
    }
}