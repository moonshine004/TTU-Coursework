package ls_project_one;

import java.util.*;

//Ticket class manages the theme park ticket booth and ride queues.

public class Ticket {
    private Stack<Visitor> ticketBooth;
    private ArrayList<String> rideNames;
    private Map<String, Queue<Visitor>> rideQueues;

    private List<String> thrillRides = Arrays.asList("Roller Coaster", "Haunted House");
    private List<String> calmRides = Arrays.asList("Ferris Wheel", "Water Slide");

    public Ticket() {
        ticketBooth = new Stack<>();
        rideNames = new ArrayList<>(Arrays.asList("Roller Coaster", "Ferris Wheel", "Haunted House", "Water Slide"));
        rideQueues = new HashMap<>();

        for (String ride : rideNames) {
            rideQueues.put(ride, new LinkedList<>());
        }
    }

    public void purchaseTicket(Visitor visitor) {
        ticketBooth.push(visitor);
        System.out.println(visitor.getName() + " purchased a " + visitor.getTicketType() + " ticket.");
    }

    public void joinRideQueue(Visitor visitor, String rideName) {
        if (!rideQueues.containsKey(rideName)) {
            System.out.println("Error: Ride not found.");
            return;
        }

        rideQueues.get(rideName).add(visitor);
        System.out.println(visitor.getName() + " joined the " + rideName + " ride queue.");
    }

    public void simulateRides() {
        Random rand = new Random();

        for (String rideName : rideNames) {
            Queue<Visitor> queue = rideQueues.get(rideName);
            System.out.println("\n--- Starting ride: " + rideName + " ---");

            while (!queue.isEmpty()) {
                Visitor visitor = queue.poll();
                System.out.println(visitor.getName() + " is riding " + rideName + "...");

                try {
                    int sleepTime = 1000 * (1 + rand.nextInt(5));
                    Thread.sleep(sleepTime);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                System.out.println(visitor.getName() + " finished the " + rideName + ".");
            }

            System.out.println("Ride " + rideName + " is now empty.");
        }
    }

    public void displayAvailableRides() {
        System.out.println("Available rides:");
        for (String ride : rideNames) {
            System.out.println("- " + ride);
        }
    }

    // Show rides based on visitor's ride preference for choice 2
    public void displaySuggestedRides(String ridePreference) {
        System.out.println("Suggested rides for preference: " + ridePreference);
 
        if (ridePreference.equalsIgnoreCase("Thrill")) {
            for (String ride : thrillRides) {
                System.out.println("- " + ride);
            }
        } else if (ridePreference.equalsIgnoreCase("Calm")) {
            for (String ride : calmRides) {
                System.out.println("- " + ride);
            }
        } else if (ridePreference.equalsIgnoreCase("Both")) {
            displayAvailableRides();
        } else {
            System.out.println("Unknown preference. Showing all rides:");
            displayAvailableRides();
        }
    }
}
