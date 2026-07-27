package ls_project_one;

// Visitor class represents a theme park visitor
public class Visitor {
    private String name;
    private int age;
    private String ticketType;
    private String ridePreference;

    // Constructor
    public Visitor(String name, int age, String ridePreference) {
        this.name = name;
        this.age = age;
        this.ridePreference = ridePreference;
        this.ticketType = assignTicketType();  // Automatically assigns ticket type
    }

    // Method to assign ticket type based on age
    private String assignTicketType() {
        if (age < 13) {
            return "Child";
        } else if (age < 60) {
            return "Adult";
        } else {
            return "Senior";
        }
    }

    // Getter methods
    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    public String getTicketType() {
        return ticketType;
    }

    public String getRidePreference() {
        return ridePreference;
    }
}
