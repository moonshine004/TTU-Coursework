package ls_project_two;

import java.util.ArrayList;
import java.util.List;

public class Student {
    // Unique identifier for the student
    private String studentId;

    // Student's full name
    private String name;

    // Student's age
    private int age;

    // Major or field of study
    private String major;

    // List of course IDs that the student is enrolled in
    private List<String> enrolledCourseIds;

    // Constructor to initialize a new student object
    public Student(String studentId, String name, int age, String major) {
        this.studentId = studentId;
        this.name = name;
        this.age = age;
        this.major = major;
        this.enrolledCourseIds = new ArrayList<>();
    }

    // Getters
    public String getStudentId() { return studentId; }
    public String getName() { return name; }
    public int getAge() { return age; }
    public String getMajor() { return major; }

    // Returns the list of course IDs the student is enrolled in
    public List<String> getEnrolledCourseIds() { return enrolledCourseIds; }

    // Adds a course ID to the student's list if not already enrolled
    public void enrollCourse(String courseId) {
        if (!enrolledCourseIds.contains(courseId)) {
            enrolledCourseIds.add(courseId);
        }
    }

    // Removes a course ID from the student's list
    public void dropCourse(String courseId) {
        enrolledCourseIds.remove(courseId);
    }

    // Displays student information and their enrolled courses
    public void displayInfo() {
        System.out.println("ID: " + studentId + ", Name: " + name + ", Age: " + age + ", Major: " + major);
        System.out.println("Enrolled Courses: " + 
            (enrolledCourseIds.isEmpty() ? "None" : String.join(", ", enrolledCourseIds)));
    }
}