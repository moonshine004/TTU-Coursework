import q1.RoomCarpet;
import q1.RoomDimension;

import java.util.Scanner;

/**
 * The Question1Demo class is the driver program for the carpet cost application.
 * It asks the user for room dimensions and carpet price, then displays total cost.
 */
public class Question1Demo {

    /**
     * The main method gets user input, creates objects, and displays results.
     *
     * @param args command-line arguments
     */
    public static void main(String[] args) {
        Scanner keyboard = new Scanner(System.in);

        double length;
        double width;
        double costPerSquareFoot;

        System.out.print("Enter the length of the room in feet: ");
        length = keyboard.nextDouble();

        System.out.print("Enter the width of the room in feet: ");
        width = keyboard.nextDouble();

        System.out.print("Enter the carpet cost per square foot: ");
        costPerSquareFoot = keyboard.nextDouble();

        RoomDimension room = new RoomDimension(length, width);
        RoomCarpet carpet = new RoomCarpet(room, costPerSquareFoot);

        System.out.println("\nRoom Carpet Information");
        System.out.println("-----------------------");
        System.out.println(carpet);

        keyboard.close();
    }
}