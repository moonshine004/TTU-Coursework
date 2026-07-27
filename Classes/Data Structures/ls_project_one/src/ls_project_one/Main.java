package ls_project_one;

import java.util.Scanner;

// Main class provides user interface to interact with the ticket system.
public class Main {
    public static void main(String[] args) {
        Ticket ticket = new Ticket();
        Scanner scanner = new Scanner(System.in);
        boolean running = true;

        System.out.println("Welcome to the Theme Park Ticket System!");
        System.out.println("You can purchase tickets, join ride queues, and simulate rides.");
        
//main menu
        while (running) {
            System.out.println("\n--- Main Menu ---");
            System.out.println("1. Purchase Ticket");
            System.out.println("2. Join Ride Queue");
            System.out.println("3. Simulate Rides");
            System.out.println("4. Exit");
            System.out.print("Choose an option (1-4): ");
            int choice = scanner.nextInt();
            scanner.nextLine();
            
// choice one, purchase ticket
            switch (choice) {
                case 1:
                    System.out.print("Enter visitor name: ");
                    String name = scanner.nextLine();
                    System.out.print("Enter visitor age: ");
                    int age = scanner.nextInt();
                    scanner.nextLine();
                    System.out.print("Enter ride preference (Thrill, Calm, Both): ");
                    String ridePref = scanner.nextLine();

                    Visitor visitor = new Visitor(name, age, ridePref);
                    ticket.purchaseTicket(visitor);
                    break;
// choice 2, join ride queue
                case 2:
                    System.out.print("Enter visitor name: ");
                    String vName = scanner.nextLine();
                    System.out.print("Enter ride preference again (Thrill, Calm, Both): ");
                    String pref = scanner.nextLine();

                    // Show suggested rides first
                    ticket.displaySuggestedRides(pref);

                    // Also show full list of available rides
                    ticket.displayAvailableRides();

                    System.out.print("Enter ride name to join: ");
                    String rideName = scanner.nextLine();

                    // In this simple version, create a new Visitor again
                    Visitor rideVisitor = new Visitor(vName, 25, pref);
                    ticket.joinRideQueue(rideVisitor, rideName);
                    break;

// choice 3, start the rides
                case 3:
                    // Simulate rides
                    ticket.simulateRides();
                    break;

// choice 4 exit
                case 4:
                    running = false;
                    System.out.println("Exiting system. Goodbye!");
                    break;

                default:
                    System.out.println("Invalid option. Please try again.");
            }
        }

        scanner.close();
    }
}
