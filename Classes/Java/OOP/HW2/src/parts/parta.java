package parts;
import java.util.Scanner;

/**
 * Converts a number from 1 to 90 into Roman numerals.
 */
public class parta {

    /*
     * Turns a number into Roman numerals
     */
    public static String toRoman(int number) {

        // Stores final Roman numeral
        String roman = "";

        // Roman numeral values
        int[] values = {90, 50, 40, 10, 9, 5, 4, 1};

        // Roman numeral symbols
        String[] numerals = {"XC", "L", "XL", "X", "IX", "V", "IV", "I"};

        // Builds the Roman numeral string
        for (int i = 0; i < values.length; i++) {
            while (number >= values[i]) {
                roman += numerals[i];
                number -= values[i];
            }
        }

        return roman;
    }

    /*
     * Runs the program and handles user input/output
     */
    public static void main(String[] args) {

        // Used for input
        Scanner scanner = new Scanner(System.in);

        // Stores user number
        int number;

        // Keep asking until valid
        while (true) {
            System.out.print("Enter a number 1 to 90: ");
            number = scanner.nextInt();

            if (number >= 1 && number <= 90) {
                break;
            }

            System.out.println("Sorry, that choice is invalid. Please try again.");
        }

        // Stores Roman numeral result
        String romanNumeral = toRoman(number);

        // Prints result
        System.out.println("Roman Numeral of " + number + ": " + romanNumeral);

        scanner.close();
    }
}
