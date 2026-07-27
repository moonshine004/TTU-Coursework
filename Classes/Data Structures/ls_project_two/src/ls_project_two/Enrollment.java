package ls_project_two;

import java.util.*;

public class Enrollment {
    // Scanner to read user input from the console
    private static final Scanner scanner = new Scanner(System.in);

    // Maps to store student and course objects by their IDs
    private static final Map<String, Student> students = new HashMap<>();
    private static final Map<String, Course> courses = new HashMap<>();

    // Priority queue to manage course scheduling by enrollment demand
    private static final PriorityQueue<Course> courseSchedule = new PriorityQueue<>((c1, c2) ->
            Integer.compare(c2.getEnrolledStudentIds().size(), c1.getEnrolledStudentIds().size()));

    // Entry point of the application with a CLI menu
    public static void main(String[] args) {
        boolean running = true;
        while (running) {
            System.out.println("\n--- Student Enrollment System ---");
            System.out.println("1. Add Student");
            System.out.println("2. Add Course");
            System.out.println("3. Update Course");
            System.out.println("4. Remove Course");
            System.out.println("5. Enroll Student in Course");
            System.out.println("6. Drop Student from Course");
            System.out.println("7. View Student Details");
            System.out.println("8. View Course Details");
            System.out.println("9. View Course Schedule (by demand)");
            System.out.println("0. Exit");
            System.out.print("Choose an option: ");
            int option = scanner.nextInt();
            scanner.nextLine();

            switch (option) {
                case 1 -> addStudent();
                case 2 -> addCourse();
                case 3 -> updateCourse();
                case 4 -> removeCourse();
                case 5 -> enrollStudent();
                case 6 -> dropStudent();
                case 7 -> viewStudent();
                case 8 -> viewCourse();
                case 9 -> showCourseSchedule();
                case 0 -> running = false;
                default -> System.out.println("Invalid option.");
            }
        }
        System.out.println("Goodbye!");
    }

    // Method to add a new student to the system
    private static void addStudent() {
        System.out.print("Student ID: ");
        String id = scanner.nextLine();
        System.out.print("Name: ");
        String name = scanner.nextLine();
        System.out.print("Age: ");
        int age = scanner.nextInt();
        scanner.nextLine();
        System.out.print("Major: ");
        String major = scanner.nextLine();

        students.put(id, new Student(id, name, age, major));
        System.out.println("Student added.");
    }

    // Method to add a new course
    private static void addCourse() {
        System.out.print("Course ID: ");
        String id = scanner.nextLine();
        System.out.print("Course Name: ");
        String name = scanner.nextLine();
        System.out.print("Instructor: ");
        String instructor = scanner.nextLine();
        System.out.print("Capacity: ");
        int capacity = scanner.nextInt();
        scanner.nextLine();

        Course course = new Course(id, name, instructor, capacity);
        courses.put(id, course);
        courseSchedule.add(course);
        System.out.println("Course added.");
    }

    // Method to update course details
    private static void updateCourse() {
        System.out.print("Enter Course ID to update: ");
        String id = scanner.nextLine();
        if (courses.containsKey(id)) {
            Course course = courses.get(id);
            System.out.print("New Course Name (or enter to skip): ");
            String name = scanner.nextLine();
            if (!name.isBlank()) course.setCourseName(name);

            System.out.print("New Instructor (or enter to skip): ");
            String instructor = scanner.nextLine();
            if (!instructor.isBlank()) course.setInstructor(instructor);

            System.out.print("New Capacity (or -1 to skip): ");
            int capacity = scanner.nextInt();
            scanner.nextLine();
            if (capacity >= 0) course.setCapacity(capacity);

            System.out.println("Course updated.");
        } else {
            System.out.println("Course not found.");
        }
    }

    // Method to remove a course and unenroll students from it
    private static void removeCourse() {
        System.out.print("Enter Course ID to remove: ");
        String id = scanner.nextLine();
        if (courses.remove(id) != null) {
            courseSchedule.removeIf(c -> c.getCourseId().equals(id));
            for (Student s : students.values()) {
                s.dropCourse(id);
            }
            System.out.println("Course removed and students unenrolled.");
        } else {
            System.out.println("Course not found.");
        }
    }

    // Method to enroll a student in a course
    private static void enrollStudent() {
        System.out.print("Student ID: ");
        String studentId = scanner.nextLine();
        System.out.print("Course ID: ");
        String courseId = scanner.nextLine();

        Student student = students.get(studentId);
        Course course = courses.get(courseId);

        if (student != null && course != null) {
            if (course.enrollStudent(studentId)) {
                student.enrollCourse(courseId);
                System.out.println("Enrolled successfully.");
            } else {
                System.out.println("Course is full or already enrolled.");
            }
        } else {
            System.out.println("Invalid student or course ID.");
        }
    }

    // Method to drop a student from a course
    private static void dropStudent() {
        System.out.print("Student ID: ");
        String studentId = scanner.nextLine();
        System.out.print("Course ID: ");
        String courseId = scanner.nextLine();

        Student student = students.get(studentId);
        Course course = courses.get(courseId);

        if (student != null && course != null) {
            if (course.dropStudent(studentId)) {
                student.dropCourse(courseId);
                System.out.println("Dropped successfully.");
            } else {
                System.out.println("Student not enrolled in course.");
            }
        } else {
            System.out.println("Invalid student or course ID.");
        }
    }

    // Method to display details of a student by ID
    private static void viewStudent() {
        System.out.print("Enter Student ID: ");
        String id = scanner.nextLine();
        Student s = students.get(id);
        if (s != null) s.displayInfo();
        else System.out.println("Student not found.");
    }

    // Method to display details of a course by ID
    private static void viewCourse() {
        System.out.print("Enter Course ID: ");
        String id = scanner.nextLine();
        Course c = courses.get(id);
        if (c != null) c.displayInfo();
        else System.out.println("Course not found.");
    }

    // Method to display all courses sorted by enrollment count (priority schedule)
    private static void showCourseSchedule() {
        System.out.println("\n--- Course Schedule (most popular first) ---");
        PriorityQueue<Course> tempQueue = new PriorityQueue<>(courseSchedule);
        while (!tempQueue.isEmpty()) {
            tempQueue.poll().displayInfo();
        }
    }
}
