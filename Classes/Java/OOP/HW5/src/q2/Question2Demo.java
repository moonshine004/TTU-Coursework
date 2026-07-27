package q2;

/**
 * Demonstrates the Month class.
 */
public class Question2Demo {

    /**
     * Main method to test Month class.
     *
     * @param args command-line arguments
     */
    public static void main(String[] args) {

        Month m1 = new Month(1);          // January
        Month m2 = new Month(7);         // July
        Month m3 = new Month("March");   // March

        System.out.println("Month 1: " + m1 + " (" + m1.getMonthNumber() + ")");
        System.out.println("Month 2: " + m2 + " (" + m2.getMonthNumber() + ")");
        System.out.println("Month 3: " + m3 + " (" + m3.getMonthNumber() + ")");

        System.out.println("\nComparisons:");
        System.out.println("m1 equals m3: " + m1.equals(m3));
        System.out.println("m2 greater than m3: " + m2.greaterThan(m3));
        System.out.println("m1 less than m2: " + m1.lessThan(m2));
    }
}