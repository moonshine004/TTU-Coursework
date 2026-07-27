package parts;

import java.util.Scanner;

/**
 * Gets 3 names and prints them in alphabetical order.
 */
public class partb {

    /*
     * Sorts three names A to Z
     */
    public static String[] sortNames(String a, String b, String c) {

        // Holds the three names
        String[] names = {a, b, c};

        // Swap first two if needed
        if (names[0].compareToIgnoreCase(names[1]) > 0) {
            String temp = names[0];
            names[0] = names[1];
            names[1] = temp;
        }

        // Swap last two if needed
        if (names[1].compareToIgnoreCase(names[2]) > 0) {
            String temp = names[1];
            names[1] = names[2];
            names[2] = temp;
        }

        // Check first two again
        if (names[0].compareToIgnoreCase(names[1]) > 0) {
            String temp = names[0];
            names[0] = names[1];
            names[1] = temp;
        }

        return names;
    }

    /*
     * Runs the program and prints sorted names
     */
    public static void main(String[] args) {

        // Used for input
        Scanner scanner = new Scanner(System.in);

        // First name
        System.out.print("Type the first name: ");
        String first = scanner.nextLine();

        // Second name
        System.out.print("Type the second name: ");
        String second = scanner.nextLine();

        // Third name
        System.out.print("Type the third name: ");
        String third = scanner.nextLine();

        // Sorted result
        String[] sorted = sortNames(first, second, third);

        // Print output
        System.out.println("Here are the names in order:");
        System.out.println("1. " + sorted[0]);
        System.out.println("2. " + sorted[1]);
        System.out.println("3. " + sorted[2]);

        scanner.close();
    }
}
