#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>
#include "testdb.h"
#include "studentrecord.h"
using namespace std;

/*
 * Updates an EXISTING user in an existing database file
 *
 * The user can pass the following parameters to this application:
 * 
 *  updaterecord -db <database file> -sid <id> [-name <name>] [-phone <phone-number>] [-modulecode <code> -grade <grade>]
 *
 *  updaterecord
 *   Without any parameters, this creates an example database file computing.txt (already done for you)
 *
 *  The -db <database file> parameters are ALWAYS required if you wish to update any records.
 *   If the file does not exist, it should be created (as an empty database)
 * 
 *  When -sid parameter is provided, you must also provide an integer <id>.
 *   The <id> must match a record in the database file.
 * 
 *  The following properties are optional, and may be provided to update data for a given student with id <sid>:
 *
 *   o The student name can be updated with the -name tag. The value <name> is a string, composed of at least two words, separated by spaces. This should replace the existing name
 * 
 *   o The phone number can be added OR updated by specifying the -phone parameter followed by a new phone number. 
 * 
 *   o An individual student grade can be added OR updated using the -modulecode and -grade parameters together.
 *
 * Note that the format of all data items should be consistent with those specified in the previous tasks.
 * The same error checking should also apply.
 *
 * **********************
 * *** VALID EXAMPLES ***
 * **********************
 *
 *
 * Create an empty database file computing.txt
 *  updaterecord -db computing.txt
 * 
 * update the phone number of a record with a student ID 12345
 *  updaterecord -db computing.txt -sid 12345 -phone 00-12-34567
 *
 * update the name and phone number of a record with a student ID 12345
 *  updaterecord -db computing.txt -sid 12345 -phone 00-12-34567 -name Jo Kingly Blunte
 *
 * update (or add) the COMP1001 grade for student with id 12345
 *  updaterecord -db computing.txt -sid 12345 -modulecode COMP1001 -grade 78.4
 *
 * Enrol the student in COMP1001 for student with id 12345
 *  updaterecord -db computing.txt -sid 12345 -modulecode COMP1001
 *
 * **********************
 * *** INVALID EXAMPLES *
 * **********************
 * 
 *  Examples of invalid parameters include:
 *
 *  Missing the database tag
 *   updaterecord -sid 12345
 *
 *  ID does not exist (assuming there is no record with ID 99999)
 *   updaterecord -db computing.txt -sid 99999 -modulecode COMP1001 -grade 78.4
 *
 *  Missing module code
 *   updaterecord -db computing.txt -sid 12345 -grade 78.4
 * 
 *  Spaces in the phone number
 *   updaterecord -db computing.txt -sid 12345 -phone 00 12 34567 -name Jo Kingly Blunte
 *
 *  Name is missing
 *   updaterecord -db computing.txt -sid 12345 -phone 00-12-34567 -name
 *
 *  Name must have at least 2 words
 *   updaterecord -db computing.txt -sid 12345 -phone 00-12-34567 -name Sam
 * 
 * *************
 * *** NOTES ***
 * *************
 * 
 * o The verb update implies you are changing an existing object. The object in this context is a student record.
 *  o You cannot update a student record that does not exist
 *  o You cannot add a student record with update command
 *  o You can update OR add data within an existing record
 *   o For example, if you update a student phone number and no number is currently held on record, 
 *     then in effect you are adding a phone number.
 * o Errors should be communicated with the user
 * o No tag should be repeated (e.g. you cannot specify -sid twice)
 * o Open files should always be closed before the program exits
 * o Code should be consistently indented and commented
 * o Marks are awarded for reuse of code from Tasks A and/or B (where possible)
 * o Marks are awarded for using Object Orientated Programming to make your code more modular and reusable
*/

// Main program here


class StudentRecord {
private:
    string studentID;
    string name;
    string phoneNumber;
    map<string, string> moduleGrades;

public:
    StudentRecord(const string& id, const string& studentName) : studentID(id), name(studentName) {}

    string getStudentID() const {
        return studentID;
    }
    string getName() const {
        return name;
    }
    string getPhoneNumber() const {
        return phoneNumber;
    }

    void setName(const string& studentName) {
        name = studentName;
    }

    void setPhoneNumber(const string& phone) {
        phoneNumber = phone;
    }

    void addModuleGrade(const string& moduleCode, const string& grade) {
        moduleGrades[moduleCode] = grade;
    }
};

vector<StudentRecord> readStudentRecords(const string& dbFile) {
    vector<StudentRecord> records;
    ifstream inFile(dbFile);
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open database file for reading\n";
        return records;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.find("#RECORD") != string::npos) {
            string sid, name, phone, moduleCode, grade;
            while (getline(inFile, line) && !line.empty()) {
                if (line.find("#SID") != string::npos) {
                    sid = line.substr(line.find_first_of("0123456789"));
                }
                else if (line.find("#NAME") != string::npos) {
                    name = line.substr(line.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
                }
                else if (line.find("#PHONE") != string::npos) {
                    phone = line.substr(line.find_first_of("0123456789"));
                }
                else if (line.find("#ENROLLMENTS") != string::npos) {
                    // Process enrollments
                }
                else if (line.find("#GRADES") != string::npos) {
                    // Process grades
                }
            }
            records.emplace_back(sid, name);
            records.back().setPhoneNumber(phone);
        }
    }

    inFile.close();
    return records;
}

bool isUnsignedInteger(const string& str) {
    return !str.empty() && str.find_first_not_of("0123456789") == string::npos;
}

bool isValidName(const string& name) {
    bool isValid = !name.empty() && regex_match(name, regex("^[a-zA-Z]+ ([\\s][a-zA-Z]+)+$"));
    if (!isValid) {
        cerr << "Error: Invalid name format - " << name << endl;
    }
    return isValid;
}
bool isValidPhoneNumber(const string& phone) {
    return !phone.empty() && regex_match(phone, regex("^[0-9-]+$"));
}

bool isValidModuleCode(const string& code) {
    return !code.empty() && regex_match(code, regex("^[a-zA-Z0-9]+$"));
}

bool isValidGrade(const string& grade) {
    return !grade.empty() && regex_match(grade, regex("^[0-9]+(\\.[0-9]+)?$"));
}

int updateRecord(const string& dbFile, const string& sid, const string& name, const string& phone, const string& moduleCode, const string& grade) {
    // Validate the provided student ID
    if (!isUnsignedInteger(sid)) {
        cerr << "Error: Student ID must be a positive integer\n";
        return EXIT_FAILURE;
    }

    // Validate the provided name
    if (!isValidName(name)) {
        cerr << "Error: Invalid student name\n";
        return EXIT_FAILURE;
    }

    // Validate the provided phone number
    if (!phone.empty() && !isValidPhoneNumber(phone)) {
        cerr << "Error: Invalid phone number format\n";
        return EXIT_FAILURE;
    }

    // Validate the provided module code and grade
    if (!moduleCode.empty() && !grade.empty()) {
        if (!isValidModuleCode(moduleCode)) {
            cerr << "Error: Invalid module code format\n";
            return EXIT_FAILURE;
        }
        if (!isValidGrade(grade)) {
            cerr << "Error: Invalid grade format\n";
            return EXIT_FAILURE;
        }
    }

    // Read the existing student records from the database file
    vector<StudentRecord> records = readStudentRecords(dbFile);

    // Locate the record with the provided student ID
    auto it = find_if(records.begin(), records.end(), [&](const StudentRecord& record) {
        return record.getStudentID() == sid;
        });

    if (it != records.end()) {
        // Update the student record with the provided information
        it->setName(name);
        it->setPhoneNumber(phone);
        it->addModuleGrade(moduleCode, grade);
    }
    else {
        cerr << "Error: Student record with ID " << sid << " not found\n";
        return EXIT_FAILURE;
    }

    // Write the updated student records back to the database file
    ofstream outFile(dbFile);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open database file for writing\n";
        return EXIT_FAILURE;
    }

    for (const auto& record : records) {
        outFile << "#RECORD\n";
        outFile << " #SID\n";
        outFile << "     " << record.getStudentID() << "\n";
        outFile << " #NAME\n";
        outFile << "     " << record.getName() << "\n";
        if (!record.getPhoneNumber().empty()) {
            outFile << " #PHONE\n";
            outFile << "     " << record.getPhoneNumber() << "\n";
        }
        // Write enrollments and grades
    }

    outFile.close();
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Error: Insufficient arguments\n";
        return EXIT_FAILURE;
    }

    string dbFile;
    string sid;
    string name;
    string phone;
    string moduleCode;
    string grade;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-db") {
            if (i + 1 < argc) {
                dbFile = argv[i + 1];
                i++; // Move to the next argument
            }
            else {
                cerr << "Error: Missing database file name\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-sid") {
            if (i + 1 < argc) {
                sid = argv[i + 1];
                i++; // Move to the next argument
            }
            else {
                cerr << "Error: Missing student ID\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-name") {
            if (i + 1 < argc) {
                name = argv[i + 1];
                i++; // Move to the next argument
            }
            else {
                cerr << "Error: Missing student name\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-phone") {
            if (i + 1 < argc) {
                phone = argv[i + 1];
                i++; // Move to the next argument
            }
            else {
                cerr << "Error: Missing phone number\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-modulecode") {
            if (i + 1 < argc) {
                moduleCode = argv[i + 1];
                i++; // Move to the next argument
            }
            else {
                cerr << "Error: Missing module code\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-grade") {
            if (i + 1 < argc) {
                grade = argv[i + 1];
                i++; // Move to the next argument
            }
            else {
                cerr << "Error: Missing grade\n";
                return EXIT_FAILURE;
            }
        }
    }

    // Call the updateRecord function with the extracted information
    int result = updateRecord(dbFile, sid, name, phone, moduleCode, grade);

    if (result == EXIT_SUCCESS) {
        cout << "Student record updated successfully!\n";
    }
    else {
        cerr << "Failed to update student record.\n";
    }

    return result;
}
