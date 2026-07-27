package parts;

import java.util.Scanner;

/**
 * Helps walk through fixing Wi-Fi problems.
 */
public class partc {

    /*
     * Checks if input means yes
     */
    public static boolean saidYes(String input) {

        // Cleaned input
        String text = input.trim().toLowerCase();

        return text.equals("yes") || text.equals("y");
    }

    /*
     * Runs the troubleshooting steps
     */
    public static void main(String[] args) {

        // Used for input
        Scanner scanner = new Scanner(System.in);

        // User response
        String reply;

        // Step 1
        System.out.println("Reboot the computer and try to connect.");
        System.out.print("Did that fix the problem? ");
        reply = scanner.nextLine();

        if (saidYes(reply)) {
            scanner.close();
            return;
        }

        // Step 2
        System.out.println("Restart the router and try to connect.");
        System.out.print("Did that fix the problem? ");
        reply = scanner.nextLine();

        if (saidYes(reply)) {
            scanner.close();
            return;
        }

        // Step 3
        System.out.println("Check that the modem and router cables are plugged in.");
        System.out.print("Did that fix the problem? ");
        reply = scanner.nextLine();

        if (saidYes(reply)) {
            scanner.close();
            return;
        }

        // Step 4
        System.out.println("Move the router somewhere else.");
        System.out.print("Did that fix the problem? ");
        reply = scanner.nextLine();

        if (saidYes(reply)) {
            scanner.close();
            return;
        }

        // Final step
        System.out.println("You may need a new router.");

        scanner.close();
    }
}
