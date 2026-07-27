package q3;

import java.util.Scanner;

/**
 * Demonstrates the Date class using user input.
 */
public class Question3Demo {

    /**
     * Main method asks user for a date and displays it.
     *
     * @param args command-line arguments
     */
    public static void main(String[] args) {

        Scanner keyboard = new Scanner(System.in);

        System.out.print("Enter month (1-12): ");
        int month = keyboard.nextInt();

        System.out.print("Enter day: ");
        int day = keyboard.nextInt();

        System.out.print("Enter year: ");
        int year = keyboard.nextInt();

        Date date = new Date(month, day, year);

        System.out.println("\nFormatted Date:");
        System.out.println(date);

        keyboard.close();
    }
}