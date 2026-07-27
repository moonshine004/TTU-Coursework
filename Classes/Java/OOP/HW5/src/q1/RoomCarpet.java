package q1;

/**
 * The RoomCarpet class stores information about carpeting for a room.
 * It contains a RoomDimension object and the carpet cost per square foot.
 * It can calculate the total cost of carpeting the room.
 */
public class RoomCarpet {

    // Stores the dimensions of the room.
    private RoomDimension size;

    // Stores the carpet cost per square foot.
    private double carpetCost;

    /**
     * Constructs a RoomCarpet object with room dimensions and carpet cost.
     *
     * @param dim the RoomDimension object for the room
     * @param cost the carpet cost per square foot
     */
    public RoomCarpet(RoomDimension dim, double cost) {
        size = dim;
        carpetCost = cost;
    }

    /**
     * Calculates and returns the total carpet cost for the room.
     *
     * @return the total cost of the carpet
     */
    public double getTotalCost() {
        return size.getArea() * carpetCost;
    }

    /**
     * Returns a string representation of the carpet information.
     *
     * @return a string containing room details, carpet cost, and total cost
     */
    @Override
    public String toString() {
        return size.toString()
                + "\nCarpet Cost per Square Foot: $" + String.format("%.2f", carpetCost)
                + "\nTotal Carpet Cost: $" + String.format("%.2f", getTotalCost());}
    }