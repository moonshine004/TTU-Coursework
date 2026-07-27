package q2;

/**
 * PatientChargesDemo is the driver program that creates a Patient
 * and several Procedure objects, then calculates total charges.
 */
public class PatientChargesDemo {

    /**
     * Program entry point for Problem 2.
     * @param args command-line arguments (not used)
     */
    public static void main(String[] args) {

        Patient patient = new Patient(
                "John", "A.", "Smith",
                "123 Main St", "Lubbock", "TX", "79401",
                "806-555-1234",
                "Jane Smith", "806-555-9999"
        );

        Procedure p1 = new Procedure("Physical Exam", "03-04-2026", "Dr. Adams", 250.00);
        Procedure p2 = new Procedure("X-Ray", "03-04-2026", "Dr. Baker", 500.00);
        Procedure p3 = new Procedure("Blood Test", "03-04-2026", "Dr. Chen", 200.00);

        displayPatient(patient);

        displayProcedure(p1);
        displayProcedure(p2);
        displayProcedure(p3);

        double total = p1.getCharge() + p2.getCharge() + p3.getCharge();
        System.out.println("Total Charges: $" + total);
    }

    /**
     * Displays patient information.
     * @param p Patient object
     */
    private static void displayPatient(Patient p) {
        System.out.println("Patient Information:");
        System.out.println(p.getFirstName() + " " + p.getMiddleName() + " " + p.getLastName());
        System.out.println(p.getAddress());
        System.out.println(p.getCity() + ", " + p.getState() + " " + p.getZipCode());
        System.out.println("Phone: " + p.getPhoneNumber());
        System.out.println("Emergency Contact: " + p.getEmergencyContactName()
                + " (" + p.getEmergencyContactPhone() + ")");
        System.out.println();
    }

    /**
     * Displays procedure information.
     * @param proc Procedure object
     */
    private static void displayProcedure(Procedure proc) {
        System.out.println("Procedure: " + proc.getProcedureName());
        System.out.println("Date: " + proc.getProcedureDate());
        System.out.println("Practitioner: " + proc.getPractitionerName());
        System.out.println("Charge: $" + proc.getCharge());
        System.out.println();
    }
}

/**
 * The Patient class stores personal and emergency contact
 * information for a patient.
 */
class Patient {

    private String firstName; // Patient first name
    private String middleName; // Patient middle name
    private String lastName; // Patient last name
    private String address; // Patient street address
    private String city; // Patient city
    private String state; // Patient state
    private String zipCode; // Patient ZIP code
    private String phoneNumber; // Patient phone number
    private String emergencyContactName; // Emergency contact name
    private String emergencyContactPhone; // Emergency contact phone

    /**
     * Constructor that initializes all patient fields.
     */
    public Patient(String firstName, String middleName, String lastName,
                   String address, String city, String state, String zipCode,
                   String phoneNumber,
                   String emergencyContactName, String emergencyContactPhone) {

        this.firstName = firstName;
        this.middleName = middleName;
        this.lastName = lastName;
        this.address = address;
        this.city = city;
        this.state = state;
        this.zipCode = zipCode;
        this.phoneNumber = phoneNumber;
        this.emergencyContactName = emergencyContactName;
        this.emergencyContactPhone = emergencyContactPhone;
    }

    /** @return patient first name */
    public String getFirstName() { return firstName; }

    /** @return patient middle name */
    public String getMiddleName() { return middleName; }

    /** @return patient last name */
    public String getLastName() { return lastName; }

    /** @return patient address */
    public String getAddress() { return address; }

    /** @return patient city */
    public String getCity() { return city; }

    /** @return patient state */
    public String getState() { return state; }

    /** @return patient ZIP code */
    public String getZipCode() { return zipCode; }

    /** @return patient phone number */
    public String getPhoneNumber() { return phoneNumber; }

    /** @return emergency contact name */
    public String getEmergencyContactName() { return emergencyContactName; }

    /** @return emergency contact phone */
    public String getEmergencyContactPhone() { return emergencyContactPhone; }
}

/**
 * The Procedure class stores information about a medical procedure
 * including name, date, practitioner, and charge.
 */
class Procedure {

    private String procedureName; // Name of the procedure
    private String procedureDate; // Date the procedure occurred
    private String practitionerName; // Doctor performing procedure
    private double charge; // Cost of the procedure

    /**
     * Constructor that initializes procedure fields.
     */
    public Procedure(String procedureName, String procedureDate,
                     String practitionerName, double charge) {

        this.procedureName = procedureName;
        this.procedureDate = procedureDate;
        this.practitionerName = practitionerName;
        this.charge = charge;
    }

    /** @return procedure name */
    public String getProcedureName() { return procedureName; }

    /** @return procedure date */
    public String getProcedureDate() { return procedureDate; }

    /** @return practitioner name */
    public String getPractitionerName() { return practitionerName; }

    /** @return procedure charge */
    public double getCharge() { return charge; }
}