#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep function
#include <conio.h>
#include <time.h>
#include <string.h>

char year;

void yearLevel() {
    printf("WHAT'S YOUR YEAR LEVEL: \n");
    printf("[1] 1ST YEAR \n");
    printf("[2] 2ND YEAR \n");
    printf("[3] 3RD YEAR \n");
    year = getch();
}

void loadingscreen() {
    int width = 20; // Width of the loading bar

    // Clear screen and set background to maroon with white text
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen

    // Move cursor to the middle of the screen (approximately 12 lines down)
    printf("\033[12;0H");

    // Loading bar loop
    for (int i = 0; i <= 100; i += 5) {
        int progress = i / (100 / width); // Calculate number of "#" for progress

        // Display loading message and progress bar
        printf("\t\t\t\t\t------------------------------------------\n");
        printf("\t\t\t\t\t|          LOADING, PLEASE WAIT...       |\n");
        printf("\t\t\t\t\t------------------------------------------\n");
        printf("\t\t\t\t\t["); // Start of the loading bar
        for (int j = 0; j < width; j++) {
            if (j < progress) {
                printf("#"); // Filled part of the bar
            } else {
                printf(" "); // Unfilled part of the bar
            }
        }
        printf("] %d%%\r", i); // Display progress percentage with carriage return
        fflush(stdout); // Ensure the progress bar updates immediately
        usleep(100000); // Pause for 100 milliseconds

        // Clear the loading bar text for the next frame
        printf("\033[A\033[A\033[A"); // Move cursor up 3 lines to overwrite
    }

    // Completed loading bar
    printf("\t\t\t\t\t------------------------------------------\n");
    printf("\t\t\t\t\t|             LOADING COMPLETE!          |\n");
    printf("\t\t\t\t\t------------------------------------------\n");
    sleep(1);

    // Clear screen again and reset colors
    printf("\033[0m"); // Reset colors
    printf("\033[2J"); // Clear screen
    printf("\033[H"); // Move cursor to the top-left corner
}

void studentAttendance() {
    // Declare student name
    char studentName[100];

    // Get student's name from user
    printf("\033[H"); // Move cursor to the top-left corner
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen

    // Student details prompt
    printf("                    --------------------------------------------------------------------------------\n");
    printf("                   |                     Please enter the student's name                          |");
    printf("                    --------------------------------------------------------------------------------\n");
    printf("                    _________________________ \n");
    printf("                   | Enter student's name: ");
    fgets(studentName, sizeof(studentName), stdin);
    studentName[strcspn(studentName, "\n")] = 0; // Remove trailing newline

    // Clear the screen
    printf("\033[H");
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen

    // Print student info
    printf("                   -----------------------------------------------------------------------------\n");
    printf("                   |  Student: %-64s|\n", studentName);
    printf("                   -----------------------------------------------------------------------------\n");

    // Month Input Loop (Only September to May)
    int month;
    const char* monthNames[] = {
        "Invalid", "September", "October", "November", "December",
        "January", "February", "March", "April", "May"
    };

    while (1) {
        // Display numbered choices for months
        printf("                    ---------------------------------------------------------------------------\n");
        printf("                   |                               SELECT MONTH:                               |\n");
        printf("                   |---------------------------------------------------------------------------|\n");
        printf("                   |   1. September 2024 (09) | 2. October 2024 (10) | 3. November 2024 (11)   |\n");
        printf("                   |   4. December 2024 (12)  | 5. January 2025 (01) | 6. February 2025 (02)   |\n");
        printf("                   |   7. March 2025 (03)     | 8. April 2025 (04)   | 9. May 2025 (05)        |\n");
        printf("                    ---------------------------------------------------------------------------\n");
        printf("                   | Please select a month by entering the number (1-9): ");
        scanf("%d", &month);

        if (month >= 1 && month <= 9) {
            break; // Exit loop if valid month is selected
        } else {
            printf("\033[1;31mInvalid month selection! Please choose a valid month between 1 and 9.\033[0m\n");
        }
    }

    // Get current year
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900; // tm_year is years since 1900

    // Define days of the week for display
    const char* daysOfWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    // Days in month (hardcoded for simplicity, check for leap years for February)
    int daysInMonth;
    switch (month) {
        case 6:
            daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        case 4: case 9: case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
    }

    // Randomly generate attendance (1 for present, 0 for absent)
    srand(time(NULL));
    int attendance[daysInMonth];
    for (int i = 0; i < daysInMonth; i++) {
        attendance[i] = 1; // Initially set all to present
    }

    // Limit the number of absences (1-3 days per month)
    int absenceCount = rand() % 3 + 1; // Random number between 1 and 3
    for (int i = 0; absenceCount > 0 && i < daysInMonth; i++) {
        if (attendance[i] == 1 && rand() % 2 == 0) {
            attendance[i] = 0;
            absenceCount--;
        }
    }

    // Display the calendar header
    printf("\n");
    printf("                           Calendar for Year %d, %s\n", year, monthNames[month]);

    // Center and display day names
    printf("                           ");
    for (int i = 0; i < 7; i++) {
        printf(" %s ", daysOfWeek[i]);
    }
    printf("\n");

    // Start the calendar from the first day of the month
    int startDay = (tm.tm_wday + 1) % 7; // Calculate which day the month starts on
    int presents = 0, absences = 0;

    // Print the calendar rows
    int currentDay = 1;
    for (int i = 0; i < startDay + daysInMonth; i++) {
        if (i % 7 == 0) {
            printf("                           "); // Center the calendar rows
        }

        if (i < startDay) {
            printf("     "); // Empty spaces before the first day
        } else {
            if (attendance[currentDay - 1] == 0) {
                printf("\033[1;31m%2d   \033[0m", currentDay); // Red for absences
                absences++;
            } else {
                printf("\033[1;37m%2d   \033[0m", currentDay); // White for present
                presents++;
            }
            currentDay++;
        }

        // Move to the next line after every 7 days (end of the week)
        if ((i + 1) % 7 == 0) {
            printf("\n");
        }
    }

    // Display attendance summary
    printf("\n\n");
    printf("                           Presents: %d\n", presents);
    printf("                           Absences: %d\n", absences);

    // Reset color
    printf("                    ---------------------------------------------------------------------------\n");
    printf("\033[0m");
}

void teachersAttendance() {
    // Declare teacher name and subject
    char teacherName[100], subject[100];

    // Get teacher's name and subject from user
    printf("\033[H"); // Move cursor to the top-left corner
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen

    // Teacher details prompt
    printf("                    --------------------------------------------------------------------------------\n");
    printf("                   |                Please enter the teacher's name and subject details             |\n");
    printf("                    --------------------------------------------------------------------------------\n");
    printf("                    _________________________ \n");
    printf("                   | Enter teacher's name: ");
    fgets(teacherName, sizeof(teacherName), stdin);
    teacherName[strcspn(teacherName, "\n")] = 0; // Remove trailing newline

    printf("                    _________________________ \n");
    printf("                   | Enter subject's name: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0; // Remove trailing newline

    // Clear the screen
    printf("\033[H");
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen

    // Print teacher and subject info
    printf("                   -----------------------------------------------------------------------------\n");
    printf("                   |  Teacher: %-64s|\n", teacherName);
    printf("                   |  Subject: %-64s|\n", subject);
    printf("                   -----------------------------------------------------------------------------\n");

    // Month Input Loop (Only September to May)
    int month;
    const char* monthNames[] = {
        "Invalid", "September", "October", "November", "December",
        "January", "February", "March", "April", "May"
    };

    while (1) {
        // Display numbered choices for months
        printf("                    ---------------------------------------------------------------------------\n");
        printf("                   |                               SELECT MONTH:                               |\n");
        printf("                   |---------------------------------------------------------------------------|\n");
        printf("                   |   1. September 2024 (09) | 2. October 2024 (10) | 3. November 2024 (11)   |\n");
        printf("                   |   4. December 2024 (12)  | 5. January 2025 (01) | 6. February 2025 (02)   |\n");
        printf("                   |   7. March 2025 (03)     | 8. April 2025 (04)   | 9. May 2025 (05)        |\n");
        printf("                    ---------------------------------------------------------------------------\n");
        printf("                   | Please select a month by entering the number (1-9): ");
        scanf("%d", &month);

        if (month >= 1 && month <= 9) {
            break; // Exit loop if valid month is selected
        } else {
            printf("\033[1;31mInvalid month selection! Please choose a valid month between 1 and 9.\033[0m\n");
        }
    }

    // Get current year
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900; // tm_year is years since 1900

    // Define days of the week for display
    const char* daysOfWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    // Days in month (hardcoded for simplicity, check for leap years for February)
    int daysInMonth;
    switch (month) {
        case 6:
            daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        case 4: case 9: case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
    }

    // Randomly generate attendance (1 for present, 0 for absent)
    srand(time(NULL));
    int attendance[daysInMonth];
    for (int i = 0; i < daysInMonth; i++) {
        attendance[i] = 1; // Initially set all to present
    }

    // Limit the number of absences (1-3 days per month)
    int absenceCount = rand() % 3 + 1; // Random number between 1 and 3
    for (int i = 0; absenceCount > 0 && i < daysInMonth; i++) {
        if (attendance[i] == 1 && rand() % 2 == 0) {
            attendance[i] = 0;
            absenceCount--;
        }
    }

    // Display the calendar header
    printf("\n");
    printf("                           Calendar for Year %d, %s\n", year, monthNames[month]);

    // Center and display day names
    printf("                           ");
    for (int i = 0; i < 7; i++) {
        printf(" %s ", daysOfWeek[i]);
    }
    printf("\n");

    // Start the calendar from the first day of the month
    int startDay = (tm.tm_wday + 1) % 7; // Calculate which day the month starts on
    int presents = 0, absences = 0;

    // Print the calendar rows
    int currentDay = 1;
    for (int i = 0; i < startDay + daysInMonth; i++) {
        if (i % 7 == 0) {
            printf("                           "); // Center the calendar rows
        }

        if (i < startDay) {
            printf("     "); // Empty spaces before the first day
        } else {
            if (attendance[currentDay - 1] == 0) {
                printf("\033[1;31m%2d   \033[0m", currentDay); // Red for absences
                absences++;
            } else {
                printf("\033[1;37m%2d   \033[0m", currentDay); // White for present
                presents++;
            }
            currentDay++;
        }

        // Move to the next line after every 7 days (end of the week)
        if ((i + 1) % 7 == 0) {
            printf("\n");
        }
    }

    // Display attendance summary
    printf("\n\n");
    printf("                           Presents: %d\n", presents);
    printf("                           Absences: %d\n", absences);

    // Reset color
    printf("                    ---------------------------------------------------------------------------\n");
    printf("\033[0m");
}

float generateRandomGrade() {
    float grades[] = {1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 4.0, 5.0};
    int index = rand() % (sizeof(grades) / sizeof(grades[0]));
    return grades[index];
}

char* evaluateGrade(float grade) {
    if (grade == 5.0 || grade == 4.0) {
        return "FAILED";
    } else if (grade <= 3.0) {
        return "PASSED";
    } else {
        return "INCOMPLETE";
    }
}

void printSubjectDetails(char* code, char* description, float grade) {
    printf("\033[48;5;234m\033[38;5;15m"); // Set background to dark gray, text to white
    printf("                   | %-15s | %-40s |     %.2f      | %-12s |\n", 
           code, description, grade, evaluateGrade(grade));
    printf("\033[0m"); // Reset to default colors
}

void eogRequest() {
    char course[100], yearLevel[100], studentStatus[20], studentNumber[20], studentName[100], schoolYear[100];
    float grades[18];
    char subjects[18][50] = {
        "UNDERSTANDING THE SELF", "READINGS IN PHIL. HISTORY", "PURPOSIVE COMMUNICATION",
        "MATHEMATICS IN THE MODERN WORLD", "INTRODUCTION TO COMPUTING", 
        "FUNDAMENTAL OF PROGRAMMING 1", "PHILTECH LIFE AND SPIRIT", "READINGS IN THE PHILIPPINE HISTORY", 
        "PHYSICAL FITNESS 1", "NATIONAL SERVICE TRAINING PROGRAM 1", "ETHICS", "WEB SYSTEM AND TECHNOLOGIES",
        "ART APPRECIATION", "INTERMEDIATE PROGRAMMING", "COMPUTER SYSTEM SERVICING", "PHYSICAL FITNESS 2",
        "NATIONAL TRAINING PROGRAM 2", "VISUAL GRAPHIC DESIGN"
    };
    char codes[18][10] = {
        "GE1", "GE2", "GE3", "GE4", "CC111", "CC112", "PLS", "PE1", "NSTP", "GE5",
        "WST", "CSS121", "PE2", "NSTP2", "VGD"
    };
    
    printf("\033[H"); // Move cursor to the top-left corner
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen

    // Initialize random number generator
    srand(time(0));

    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    -------------------------------------------------------------------------------------------------\n");
    printf("                   |                                   EVALUATION OF GRADES REQUEST                                  |\n");
    printf("                    -------------------------------------------------------------------------------------------------\n");
    printf("\033[0m"); // Reset to default colors

    // Get student details
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    Enter your course: ");
    printf("\033[0m"); // Reset to default colors
    fgets(course, sizeof(course), stdin);
    course[strcspn(course, "\n")] = 0;

    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    Enter your year level: ");
    printf("\033[0m"); // Reset to default colors
    fgets(yearLevel, sizeof(yearLevel), stdin);
    yearLevel[strcspn(yearLevel, "\n")] = 0;

    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    Enter your student status (Regular, Non-Regular): ");
    printf("\033[0m"); // Reset to default colors
    fgets(studentStatus, sizeof(studentStatus), stdin);
    studentStatus[strcspn(studentStatus, "\n")] = 0;

    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    Enter your student number: ");
    printf("\033[0m"); // Reset to default colors
    fgets(studentNumber, sizeof(studentNumber), stdin);
    studentNumber[strcspn(studentNumber, "\n")] = 0;

    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    Enter your student name: ");
    printf("\033[0m"); // Reset to default colors
    fgets(studentName, sizeof(studentName), stdin);
    studentName[strcspn(studentName, "\n")] = 0;

    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    Enter your school year: ");
    printf("\033[0m"); // Reset to default colors
    fgets(schoolYear, sizeof(schoolYear), stdin);
    schoolYear[strcspn(schoolYear, "\n")] = 0;

    sleep(2);

    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux/macOS
    #endif
    
    // Generate random grades for all subjects
    for (int i = 0; i < 18; i++) {
        grades[i] = generateRandomGrade();
    }

    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("                    --------------------------------------------------------------------------------------------\n");
    printf("                   | Student Name: %-26s       Status: %-33s|\n", studentName, studentStatus);
    printf("                   | Student Number: %-24s       School Year: %-28s|\n", studentNumber, schoolYear);
    printf("                   | Course: %-32s       Year Level: %-29s|\n", course, yearLevel);
    printf("                   ---------------------------------------------------------------------------------------------\n");
    printf("                   |  SUBJECT CODE   |           DESCRIPTION                    |    GRADES     |  EVALUATION  |\n");
    printf("                   |-----------------|------------------------------------------|---------------|--------------|\n");
    printf("\033[0m"); // Reset to default colors

    // Print details of all subjects
    for (int i = 0; i < 18; i++) {
        printSubjectDetails(codes[i], subjects[i], grades[i]);
    }
}

void enrollment() {
	
	printf("\033[H"); // Move cursor to the top-left corner
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen
    
    
    char fullName[100], email[50], contactNumber[20], homeAddress[100], currentAddress[100];
    char gender[10], dateOfBirth[20], gmail[50], highSchoolName[100], highSchoolAddress[100];
    char yearOfGraduation[10], preferredCourse[10], preferredSchedule[20], academicLevel[20];
    char parentGuardianName[100], parentGuardianContact[20], howDidYouKnow[100];
    char editChoice[5];
    int editField;

    printf("+--------------------------------------------------------------------------------------------------------------------+\n");
    printf("||             \t\t\t\t           ENROLLMENT FORM             \t\t\t\t            ||\n");
    printf("+--------------------------------------------------------------------------------------------------------------------+\n");

    printf("|| Full Name:                ");
    fgets(fullName, sizeof(fullName), stdin);
    fullName[strcspn(fullName, "\n")] = 0;

    printf("|| Email:                    ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0;

    printf("|| Contact Number:           ");
    fgets(contactNumber, sizeof(contactNumber), stdin);
    contactNumber[strcspn(contactNumber, "\n")] = 0;

    printf("|| Home Address:             ");
    fgets(homeAddress, sizeof(homeAddress), stdin);
    homeAddress[strcspn(homeAddress, "\n")] = 0;

    printf("|| Current Address:          ");
    fgets(currentAddress, sizeof(currentAddress), stdin);
    currentAddress[strcspn(currentAddress, "\n")] = 0;

    printf("|| Gender:                   ");
    fgets(gender, sizeof(gender), stdin);
    gender[strcspn(gender, "\n")] = 0;

    printf("|| Date of Birth (dd/mm/yyyy): ");
    fgets(dateOfBirth, sizeof(dateOfBirth), stdin);
    dateOfBirth[strcspn(dateOfBirth, "\n")] = 0;

    printf("|| Gmail Account:            ");
    fgets(gmail, sizeof(gmail), stdin);
    gmail[strcspn(gmail, "\n")] = 0;

    printf("|| High School Name:         ");
    fgets(highSchoolName, sizeof(highSchoolName), stdin);
    highSchoolName[strcspn(highSchoolName, "\n")] = 0;

    printf("|| High School Address:      ");
    fgets(highSchoolAddress, sizeof(highSchoolAddress), stdin);
    highSchoolAddress[strcspn(highSchoolAddress, "\n")] = 0;

    printf("|| Year of Graduation:       ");
    fgets(yearOfGraduation, sizeof(yearOfGraduation), stdin);
    yearOfGraduation[strcspn(yearOfGraduation, "\n")] = 0;

    printf("|| Preferred Course (BTVTED/BSOA/BSCS): ");
    fgets(preferredCourse, sizeof(preferredCourse), stdin);
    preferredCourse[strcspn(preferredCourse, "\n")] = 0;

    printf("|| Preferred Schedule (Regular/Sunday Class): ");
    fgets(preferredSchedule, sizeof(preferredSchedule), stdin);
    preferredSchedule[strcspn(preferredSchedule, "\n")] = 0;

    printf("|| Academic Level:           ");
    fgets(academicLevel, sizeof(academicLevel), stdin);
    academicLevel[strcspn(academicLevel, "\n")] = 0;

    printf("|| Parents/Guardians Name:   ");
    fgets(parentGuardianName, sizeof(parentGuardianName), stdin);
    parentGuardianName[strcspn(parentGuardianName, "\n")] = 0;

    printf("|| Parents/Guardians Contact Number: ");
    fgets(parentGuardianContact, sizeof(parentGuardianContact), stdin);
    parentGuardianContact[strcspn(parentGuardianContact, "\n")] = 0;

    printf("|| How did you know about this school?: ");
    fgets(howDidYouKnow, sizeof(howDidYouKnow), stdin);
    howDidYouKnow[strcspn(howDidYouKnow, "\n")] = 0;

    do {
        printf("\n+--------------------------------------------------------------------------------------------------------------------+\n");
        printf("|| \t\t\t\t              >>>Submitted Information<<<          \t\t\t\t   ||\n");
        printf("+--------------------------------------------------------------------------------------------------------------------+\n");
        printf("|| Full Name:                %s\n", fullName);
        printf("|| Email:                    %s\n", email);
        printf("|| Contact Number:           %s\n", contactNumber);
        printf("|| Home Address:             %s\n", homeAddress);
        printf("|| Current Address:          %s\n", currentAddress);
        printf("|| Gender:                   %s\n", gender);
        printf("|| Date of Birth:            %s\n", dateOfBirth);
        printf("|| Gmail Account:            %s\n", gmail);
        printf("|| High School Name:         %s\n", highSchoolName);
        printf("|| High School Address:      %s\n", highSchoolAddress);
        printf("|| Year of Graduation:       %s\n", yearOfGraduation);
        printf("|| Preferred Course:         %s\n", preferredCourse);
        printf("|| Preferred Schedule:       %s\n", preferredSchedule);
        printf("|| Academic Level:           %s\n", academicLevel);
        printf("|| Parents/Guardians Name:   %s\n", parentGuardianName);
        printf("|| Parents/Guardians Contact: %s\n", parentGuardianContact);
        printf("|| How did you know about this school?: %s\n", howDidYouKnow);
        printf("+--------------------------------------------------------------------------------------------------------------------+\n");

        printf("\nDo you need to edit any information? (yes/no): ");
        fgets(editChoice, sizeof(editChoice), stdin);
        editChoice[strcspn(editChoice, "\n")] = 0;

        if (strcmp(editChoice, "yes") == 0) {
            printf("\nWhich field do you want to edit? (1-17):\n");
            printf("1. Full Name\n2. Email\n3. Contact Number\n4. Home Address\n5. Current Address\n6. Gender\n7. Date of Birth\n8. Gmail Account\n9. High School Name\n10. High School Address\n11. Year of Graduation\n12. Preferred Course\n13. Preferred Schedule\n14. Academic Level\n15. Parents/Guardians Name\n16. Parents/Guardians Contact Number\n17. How did you know about this school?\n");
            scanf("%d", &editField);
            getchar(); // Consume newline left by scanf

            switch (editField) {
                case 1: printf("|| Full Name:                "); fgets(fullName, sizeof(fullName), stdin); fullName[strcspn(fullName, "\n")] = 0; break;
                case 2: printf("|| Email:                    "); fgets(email, sizeof(email), stdin); email[strcspn(email, "\n")] = 0; break;
                case 3: printf("|| Contact Number:           "); fgets(contactNumber, sizeof(contactNumber), stdin); contactNumber[strcspn(contactNumber, "\n")] = 0; break;
                case 4: printf("|| Home Address:             "); fgets(homeAddress, sizeof(homeAddress), stdin); homeAddress[strcspn(homeAddress, "\n")] = 0; break;
                case 5: printf("|| Current Address:          "); fgets(currentAddress, sizeof(currentAddress), stdin); currentAddress[strcspn(currentAddress, "\n")] = 0; break;
                case 6: printf("|| Gender:                   "); fgets(gender, sizeof(gender), stdin); gender[strcspn(gender, "\n")] = 0; break;
                case 7: printf("|| Date of Birth (dd/mm/yyyy): "); fgets(dateOfBirth, sizeof(dateOfBirth), stdin); dateOfBirth[strcspn(dateOfBirth, "\n")] = 0; break;
                case 8: printf("|| Gmail Account:            "); fgets(gmail, sizeof(gmail), stdin); gmail[strcspn(gmail, "\n")] = 0; break;
                case 9: printf("|| High School Name:         "); fgets(highSchoolName, sizeof(highSchoolName), stdin); highSchoolName[strcspn(highSchoolName, "\n")] = 0; break;
                case 10: printf("|| High School Address:      "); fgets(highSchoolAddress, sizeof(highSchoolAddress), stdin); highSchoolAddress[strcspn(highSchoolAddress, "\n")] = 0; break;
                case 11: printf("|| Year of Graduation:       "); fgets(yearOfGraduation, sizeof(yearOfGraduation), stdin); yearOfGraduation[strcspn(yearOfGraduation, "\n")] = 0; break;
                case 12: printf("|| Preferred Course (BTVTED/BSOA/BSCS): "); fgets(preferredCourse, sizeof(preferredCourse), stdin); preferredCourse[strcspn(preferredCourse, "\n")] = 0; break;
                case 13: printf("|| Preferred Schedule (Regular/Sunday Class): "); fgets(preferredSchedule, sizeof(preferredSchedule), stdin); preferredSchedule[strcspn(preferredSchedule, "\n")] = 0; break;
                case 14: printf("|| Academic Level:           "); fgets(academicLevel, sizeof(academicLevel), stdin); academicLevel[strcspn(academicLevel, "\n")] = 0; break;
                case 15: printf("|| Parents/Guardians Name:   "); fgets(parentGuardianName, sizeof(parentGuardianName), stdin); parentGuardianName[strcspn(parentGuardianName, "\n")] = 0; break;
                case 16: printf("|| Parents/Guardians Contact Number: "); fgets(parentGuardianContact, sizeof(parentGuardianContact), stdin); parentGuardianContact[strcspn(parentGuardianContact, "\n")] = 0; break;
                case 17: printf("|| How did you know about this school?: "); fgets(howDidYouKnow, sizeof(howDidYouKnow), stdin); howDidYouKnow[strcspn(howDidYouKnow, "\n")] = 0; break;
                default: printf("Invalid choice. Please select a valid option (1-17).\n");
            }
        }
    } while (strcmp(editChoice, "yes") == 0);

    printf("+--------------------------------------------------------------------------------------------------------------------+\n");
    printf("|| \t\t\t\t              >>>FINAL SUBMISSION<<<          \t\t\t\t                   ||\n");
    printf("+--------------------------------------------------------------------------------------------------------------------+\n");
    printf("|| Full Name:                %s\n", fullName);
    printf("|| Email:                    %s\n", email);
    printf("|| Contact Number:           %s\n", contactNumber);
    printf("|| Home Address:             %s\n", homeAddress);
    printf("|| Current Address:          %s\n", currentAddress);
    printf("|| Gender:                   %s\n", gender);
    printf("|| Date of Birth:            %s\n", dateOfBirth);
    printf("|| Gmail Account:            %s\n", gmail);
    printf("|| High School Name:         %s\n", highSchoolName);
    printf("|| High School Address:      %s\n", highSchoolAddress);
    printf("|| Year of Graduation:       %s\n", yearOfGraduation);
    printf("|| Preferred Course:         %s\n", preferredCourse);
    printf("|| Preferred Schedule:       %s\n", preferredSchedule);
    printf("|| Academic Level:           %s\n", academicLevel);
    printf("|| Parents/Guardians Name:   %s\n", parentGuardianName);
    printf("|| Parents/Guardians Contact: %s\n", parentGuardianContact);
    printf("|| How did you know about this school?: %s\n", howDidYouKnow);
    printf("+--------------------------------------------------------------------------------------------------------------------+\n");
    printf("\nThank you for completing the enrollment form. Have a great day!\n");
    
        // Clear screen again and reset colors
    printf("\033[2J"); // Clear screen
    printf("\033[H"); // Move cursor to the top-left corner
    printf("\033[48;5;52m\033[38;5;15m"); // Set background to maroon, text to white again
}

void classroomManagement() {
	printf("\033[H"); // Move cursor to the top-left corner
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen
    
    printf("CLASSROOM MANAGEMENT!\n");
}

void exitScreen() {
    int width = 20; // Width of the loading bar

    // Clear screen and set background to maroon with white text
    printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
    printf("\033[2J"); // Clear screen

    // Move cursor to the middle of the screen (about 10 lines down)
    printf("\033[12;0H");

    // Loading bar loop
    for (int i = 0; i <= 100; i += 5) {
        int progress = i / (100 / width); // Calculate number of "#" for progress

        // Ensure alignment matches the border's horizontal position
        printf("\t\t\t\t\t------------------------------------------\n");
        printf("\t\t\t\t\t|          EXITING, PLEASE WAIT...       |\n");
        printf("\t\t\t\t\t------------------------------------------\n");
        printf("\t\t\t\t\t["); // Start of the loading bar
        for (int j = 0; j < width; j++) {
            if (j < progress) {
                printf("#"); // Filled part of the bar
            } else {
                printf(" "); // Unfilled part of the bar
            }
        }
        printf("] %d%%\r", i); // Display progress percentage with carriage return
        fflush(stdout); // Ensure the progress bar updates immediately
        usleep(100000); // Pause for 100 milliseconds

        // Clear the loading bar text for the next frame
        printf("\033[A\033[A\033[A"); // Move cursor up 3 lines to overwrite
    }

    // Completed loading bar
    printf("\t\t\t\t\t------------------------------------------\n");
    printf("\t\t\t\t\t|          YOU HAVE A GREAT DAY!         |\n");
    printf("\t\t\t\t\t------------------------------------------\n");
    sleep(2);

    // Clear screen again and reset colors
    printf("\033[2J"); // Clear screen
    printf("\033[H"); // Move cursor to the top-left corner
    printf("\033[48;5;52m\033[38;5;15m"); // Set background to maroon, text to white again
}

int main() {
    loadingscreen();
    char choice;

    while (1) {
        // Clear the screen and reset colors for each loop
        printf("\033[H"); // Move cursor to the top-left corner
        printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
        printf("\033[2J"); // Clear screen

        // Display the menu
        printf("                   ---------------------------------------------------------------------------------\n");
        printf("                   |                    WELCOME TO PHILTECH MANAGEMENT SYSTEM!                     |\n");
        printf("                   |-------------------------------------------------------------------------------|\n");
        printf("                   |-------------------------------------------------------------------------------|\n");
        printf("                   |              [1]  STUDENT ATTENDANCE                                          |\n");
        printf("                   |-------------------------------------------------------------------------------|\n");
        printf("                   |              [2]  TEACHER'S ATTENDANCE                                        |\n");
        printf("                   |-------------------------------------------------------------------------------|\n");
        printf("                   |              [3]  ENROLLMENT                                                  |\n");
        printf("                   |-------------------------------------------------------------------------------|\n");
        printf("                   |              [4]  EOG REQUEST                                                 |\n");
        printf("                   |-------------------------------------------------------------------------------|\n");
        printf("                   |              [5]  CLASSROOM MANAGEMENT                                        |\n");
        printf("                   |-------------------------------------------------------------------------------|\n");
        printf("                   |              [0]  EXIT                                                        |\n");
        printf("                   |                                                                               |\n");
        printf("                   ---------------------------------------------------------------------------------\n");
        printf("\033[0m"); // Reset colors to default

        // Get user input
        choice = getch();

        // Check the input and take appropriate action
        switch (choice) {
            case '1':
                studentAttendance();
                return 0;
            case '2':
                teachersAttendance();
                return 0;
            case '3':
                enrollment();
                return 0;
            case '4':
                eogRequest();
                return 0;
            case '5':
                classroomManagement();
                return 0;
            case '0':
                exitScreen();
                return 0;
            default:
                // Invalid input message
                printf("\033[H"); // Move cursor to the top-left corner
                printf("\033[48;5;52m\033[38;5;15m"); // Background maroon, text white
                printf("\033[2J"); // Clear screen
                printf("\n\n\n\n\n");
                printf("\033[1;52m                                 INVALID INPUT. Please select only from the choices (0-5)!\n");
                printf("                                 Press any key to try again...\033[0m");
                getch(); // Wait for user to press a key before looping
                break;
        }
    }
}

