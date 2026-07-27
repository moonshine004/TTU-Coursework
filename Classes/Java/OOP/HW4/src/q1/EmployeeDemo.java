package q1;

/**
 * EmployeeDemo is the driver class that creates Employee objects
 * and prints their information to the console.
 */
public class EmployeeDemo {

    /**
     * Program entry point. Creates three employees and displays their data.
     * @param args command-line arguments (not used)
     */
    public static void main(String[] args) {

        Employee e1 = new Employee("Susan Meyers", 47899, "Accounting", "Vice President");
        Employee e2 = new Employee("Mark Jones", 39119, "IT", "Programmer");
        Employee e3 = new Employee("Joy Rogers", 81774, "Manufacturing", "Engineer");

        printEmployee(e1);
        System.out.println();
        printEmployee(e2);
        System.out.println();
        printEmployee(e3);
    }

    /**
     * Prints an employee's information.
     * @param e Employee object to display
     */
    private static void printEmployee(Employee e) {
        System.out.println("Name: " + e.getName());
        System.out.println("ID Number: " + e.getIdNumber());
        System.out.println("Department: " + e.getDepartment());
        System.out.println("Position: " + e.getPosition());
    }
}

/**
 * The Employee class stores information about an employee including
 * name, ID number, department, and job position.
 */
class Employee {

    private String name; // Employee's full name
    private int idNumber; // Employee identification number
    private String department; // Department the employee works in
    private String position; // Employee job title

    /**
     * Full constructor that initializes all fields.
     * @param name employee name
     * @param idNumber employee ID
     * @param department employee department
     * @param position employee job title
     */
    public Employee(String name, int idNumber, String department, String position) {
        this.name = name;
        this.idNumber = idNumber;
        this.department = department;
        this.position = position;
    }

    /**
     * Constructor that initializes name and ID only.
     * @param name employee name
     * @param idNumber employee ID
     */
    public Employee(String name, int idNumber) {
        this.name = name;
        this.idNumber = idNumber;
        this.department = "";
        this.position = "";
    }

    /**
     * Default constructor.
     */
    public Employee() {
        name = "";
        idNumber = 0;
        department = "";
        position = "";
    }

    /**
     * Returns the employee name.
     * @return employee name
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the employee ID number.
     * @return employee ID
     */
    public int getIdNumber() {
        return idNumber;
    }

    /**
     * Returns the employee department.
     * @return department name
     */
    public String getDepartment() {
        return department;
    }

    /**
     * Returns the employee position.
     * @return job position
     */
    public String getPosition() {
        return position;
    }

    /**
     * Sets employee name.
     * @param name employee name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Sets employee ID.
     * @param idNumber employee ID
     */
    public void setIdNumber(int idNumber) {
        this.idNumber = idNumber;
    }

    /**
     * Sets employee department.
     * @param department department name
     */
    public void setDepartment(String department) {
        this.department = department;
    }

    /**
     * Sets employee position.
     * @param position job title
     */
    public void setPosition(String position) {
        this.position = position;
    }
}