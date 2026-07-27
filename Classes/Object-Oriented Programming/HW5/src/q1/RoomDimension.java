package q1;

/**
 * The RoomDimension class stores the length and width of a room.
 * It can calculate and return the area of the room in square feet.
 */
public class RoomDimension {

    // Stores the length of the room.
    private double length;

    // Stores the width of the room.
    private double width;

    /**
     * Constructs a RoomDimension object with the given length and width.
     *
     * @param len the length of the room
     * @param w the width of the room
     */
    public RoomDimension(double len, double w) {
        length = len;
        width = w;
    }

    /**
     * Calculates and returns the area of the room.
     *
     * @return the area of the room in square feet
     */
    public double getArea() {
        return length * width;
    }

    /**
     * Returns a string representation of the room dimensions.
     *
     * @return a string containing the room length and width
     */
    @Override
    public String toString() {
        return "Length: " + length + " ft, Width: " + width + " ft";
    }
}