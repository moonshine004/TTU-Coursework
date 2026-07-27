package q3;

import q2.Month;

/**
 * The Date class represents an immutable date.
 * It stores a month, day, and year and validates input values.
 */
public final class Date {

    // Stores the month using the Month class.
    private final Month month;

    // Stores the day of the month.
    private final int day;

    // Stores the year.
    private final int year;

    /**
     * Constructor that accepts month, day, and year.
     * Validates all values.
     *
     * @param m the month (1-12)
     * @param d the day
     * @param y the year
     */
    public Date(int m, int d, int y) {

        // Validate month
        if (m < 1 || m > 12) {
            m = 1;
        }

        this.month = new Month(m);

        // Validate year (cannot be 0)
        if (y == 0) {
            y = 1;
        }

        this.year = y;

        // Validate day based on month
        int maxDays = getDaysInMonth(m);

        if (d < 1 || d > maxDays) {
            d = 1;
        }

        this.day = d;
    }

    /**
     * Copy constructor.
     *
     * @param other another Date object
     */
    public Date(Date other) {
        this.month = new Month(other.month.getMonthNumber());
        this.day = other.day;
        this.year = other.year;
    }

    /**
     * Returns the number of days in a given month.
     *
     * @param m the month number
     * @return number of days
     */
    private int getDaysInMonth(int m) {
        switch (m) {
            case 2:
                return 28;
            case 4: case 6: case 9: case 11:
                return 30;
            default:
                return 31;
        }
    }

    /**
     * Returns the date as a formatted string.
     *
     * @return formatted date string
     */
    @Override
    public String toString() {
        return month.getMonthName() + " " + day + ", " + year;
    }
}