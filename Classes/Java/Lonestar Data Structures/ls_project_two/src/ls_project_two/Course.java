package ls_project_two;

import java.util.ArrayList;
import java.util.List;

public class Course {
    // Unique identifier for the course
    private String courseId;

    // Name/title of the course
    private String courseName;

    // Instructor name for the course
    private String instructor;

    // Maximum number of students allowed to enroll
    private int capacity;

    // List of student IDs currently enrolled in the course
    private List<String> enrolledStudentIds;

    // Constructor to initialize a new Course
    public Course(String courseId, String courseName, String instructor, int capacity) {
        this.courseId = courseId;
        this.courseName = courseName;
        this.instructor = instructor;
        this.capacity = capacity;
        this.enrolledStudentIds = new ArrayList<>();
    }

    // Getters for course attributes
    public String getCourseId() { return courseId; }
    public String getCourseName() { return courseName; }
    public String getInstructor() { return instructor; }
    public int getCapacity() { return capacity; }
    public List<String> getEnrolledStudentIds() { return enrolledStudentIds; }

    // Setters to update course attributes
    public void setCourseName(String courseName) { this.courseName = courseName; }
    public void setInstructor(String instructor) { this.instructor = instructor; }
    public void setCapacity(int capacity) { this.capacity = capacity; }

    // Enrolls a student if capacity allows and not already enrolled
    public boolean enrollStudent(String studentId) {
        if (enrolledStudentIds.size() < capacity && !enrolledStudentIds.contains(studentId)) {
            enrolledStudentIds.add(studentId);
            return true;
        }
        return false;
    }

    // Removes a student from the course
    public boolean dropStudent(String studentId) {
        return enrolledStudentIds.remove(studentId);
    }

    // Displays course information
    public void displayInfo() {
        System.out.println("Course ID: " + courseId + ", Name: " + courseName + ", Instructor: " + instructor +
                ", Capacity: " + capacity + ", Enrolled: " + enrolledStudentIds.size());
    }
}
