



/*
 * Adds a NEW user to the end of an existing database file or the start of a new file
 *
 * The user can pass the following parameters to this application:
 *
 * addrecord -db <database file> [-sid <id> -name <name> [-phone <phone-number>] [-modulecodes <code1> <code2> ... <codeN> [-grades <grade1> <grade2> ... <gradeM> ]]]
 *
 * addrecord
 *  Without any parameters, this creates an example database file computing.txt (already done for you)
 *
 * The -db <database file> parameters are ALWAYS required if you wish to add a record.
 *  If the file does not exist, it should be created (as an empty database)
 *
 * Where -sid is provided.
 *    • A numerical student ID <id> MUST also be provided.
 *       o <id> must be an unsigned integer.
 *       o <id> must be unique. If another record in the database has the same value, an error should be reported.
 *
 * Where -sid is provided, the following MUST also be included:
 *    • The tag -name and a student name <name>.
 *       o <name> is a full name, so will comprise only alphabetical characters and spaces.
 *       o <name> must contain at least two words separated by spaces.
 *
 * Where -sid is provided, the following MAY also be included:
 *    • The tag -phone followed by a phone number.
 *      o <phone-number> is a string with no spaces.
 *    • The tag -modulecodes, followed by a list of module codes.
 *       o Each module code is a single alpha-numeric word. It cannot contain any symbols.
 *       o Each module code is separated by a space.
 *       o There must be at least one module code provided.
 *    • IF a -modulecodes tag is provided, the following MAY also be provided:
 *       o The tag -grades, followed by a list of grades.
 *       o Each grade must be a fractional number.
 *       o Each grade must be separated by a space.
 *       o The number of grades MUST match the number of module codes.
 *       o You cannot use -grades tag without an accompanying -modulecodes tag
 *
 *
 * **********************
 * *** VALID EXAMPLES ***
 * **********************
 *
 * Create an empty database file computing.txt
 *  addrecord -db computing.txt
 *
 * Add a record with a student ID <id> and name <name>, where <id> is an integer and name <name> is a string (space separated)
 *  addrecord -db computing.txt -sid 12345 -name Sam Eold
 *
 * This version also includes the phone number <phone>, where <phone> is a string with no spaces
 *  addrecord -db computing.txt -sid 12345 -name Sam Eold -phone 44-1234-456123
 *
 * This version includes the modules the student is enrolled on
 *  addrecord -db computing.txt -sid 13579 -name Les Sismore -modulecodes COMP101 COMP110 COMP123 COMP145 COMP165
 *
 * This includes the grades as well as the module codes. The number of grades must equal the number of codes or an error is displayed
 *  addrecord -db computing.txt -sid 24680 -name Jo King -modulecodes COMP101 COMP110 COMP123 COMP145 COMP165 -grades 40.5 55.6 35.7 67.5 80.1
 *
 *
 * *************************************
 * *** EXAMPLES OF INVALID ARGUMENTS ***
 * *************************************
 *
 *  Missing the database filename
 *   addrecord -db -sid 12345 -name Sam Eold
 *
 *  Name is missing
 *   addrecord -db computing.txt -sid 12345
 *
 *  -sid is missing
 *   addrecord -db computing.txt -name Sam Eold
 *
 *  <id> is not an integer
 *   addrecord -db computing.txt -sid a2345 -name Sam Eold
 *
 *  phone number cannot contain spaces
 *   addrecord -db computing.txt -sid 12345 -name Sam Eold -phone 44 01234 456123
 *
 *  grades cannot be provided without module codes
 *   addrecord -db computing.txt -sid 24680 -name Jo King -grades 40.5 55.6 35.7 67.5 80.1
 *
 *  Number of module codes and grades do not match
 *  addrecord -db computing.txt -sid 24680 -name Jo King -modulecodes COMP101 COMP110 -grades 40.5 55.6 35.7
 *
 * *************
 * *** NOTES ***
 * *************
 *
 * o The verb ADD implies you are created a new object. The object in this context is a student record.
 *  o You cannot add a student record that already exists
 *  o To add data to a student record that already exist, see task C
 *
 * o Errors should be communicated with the user
 * o No tag should be repeated (e.g. you cannot specify -sid twice)
 * o New records should be added to the end of the database file (known as appending). See the labs on FileIO and ios::app
 * o Open files should always be closed before the program exits
 * o You may copy or (better) reuse code from Task A where possible
 * o Code should be consistently indented and commented
*/

// Main program here - you may shared / reuse as much code as you like with other projects included in this starter code


// Function to add a new student record to the database
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
int main(int argc, char* argv[]) {
    if (argc == 1) {
        // Welcome message
        cout << "addrecord (c)2023" << endl;

        // Create some test data
        ofstream testFile("computing.txt");
        if (testFile.is_open()) {
            testFile << "Sample data for testing\n";
            testFile.close();
        }
        else {
            cerr << "Error: Unable to create test database file\n";
            return EXIT_FAILURE;
        }

        // Done
        return EXIT_SUCCESS;
    }

    if (argc < 6) {
        cerr << "Error: Insufficient command line arguments\n";
        return EXIT_FAILURE;
    }

    // Extract command line arguments
    string filename, sid, name, phone;
    int Sid;
    vector<string> moduleCodes, grades;

    bool hasModuleCodes = false;
    bool hasGrades = false;

    // Flags to ensure that required parameters are provided
    bool hasSID = false;
    bool hasName = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "-db") {
            if (i + 1 < argc) {
                filename = argv[i + 1];
                // Check if the next argument is also a flag
                if (filename.size() > 0 && filename[0] == '-') {
                    cerr << "Error: Missing database filename\n";
                    return EXIT_FAILURE;
                }
                ++i; // Move to the next argument
            }
            else {
                cerr << "Error: Missing database filename\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-sid") {
            if (i + 1 < argc) {
                // Validate that SID is a valid unsigned integer
                try {
                    Sid = stoi(argv[i + 1]);
                    if (Sid < 0) {
                        cerr << "Error: Student ID must be a positive integer\n";
                        return EXIT_FAILURE;
                    }
                    hasSID = true; // Flag that SID is provided
                }
                catch (const invalid_argument& e) {
                    cerr << "Error: Invalid student ID. Please provide a positive integer.\n";
                    return EXIT_FAILURE;
                }
                sid = to_string(Sid);
                ++i; // Move to the next argument
            }
            else {
                cerr << "Error: Missing student ID\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-name") {
            if (i + 1 < argc) {
                name = argv[i + 1];
                // Move to the next argument
                ++i;
                while (i < argc && argv[i][0] != '-') {
                    name += " " + string(argv[i]);
                    ++i; // Move to the next argument
                }
                hasName = true; // Flag that Name is provided
            }
            else {
                cerr << "Error: Missing student name\n";
                return EXIT_FAILURE;
            }
        }
        else if (arg == "-phone") {
            if (i + 1 < argc) {
                phone = argv[i + 1];
                ++i; // Move to the next argument
            }
            else {
                cerr << "Error: Missing phone number\n";
                return EXIT_FAILURE;
            }
        }

        else if (arg == "-grades") {
            hasGrades = true;
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                // Validate that grades are valid fractional numbers
                try {
                    stof(argv[i + 1]);
                }
                catch (const invalid_argument& e) {
                    cerr << "Error: Invalid grade. Please provide a valid fractional number.\n";
                    return EXIT_FAILURE;
                }
                grades.push_back(argv[i + 1]);
                ++i; // Move to the next argument
            }
        }
        else if (arg == "-modulecodes") {
            hasModuleCodes = true;
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                // Validate that module codes are alphanumeric
                if (!regex_match(argv[i + 1], regex("^[a-zA-Z0-9]+$"))) {
                    cerr << "Error: Invalid module code. Module codes must be alphanumeric.\n";
                    return EXIT_FAILURE;
                }
                moduleCodes.push_back(argv[i + 1]);
                ++i; // Move to the next argument
            }
        }
    }

    // Validate input

    if (!hasSID) {
        cerr << "Error: Missing SID required\n";
        return EXIT_FAILURE;
    }

     if (!hasName) {
        cerr << "Error: Missing Name required\n";
        return EXIT_FAILURE;
    }

    if (moduleCodes.size() == grades.size()) {
        cerr << "good\n";
    }
    else {
        cerr << "Error: Number of module codes and grades do not match\n";
        return EXIT_FAILURE;
    }

    if (!hasModuleCodes) {
        cerr << "Error: Grades cannot be given unless modules are not there!\n";
        
    }

    // Check for duplicate student IDs
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            if (line.find("#SID") != string::npos) {
                getline(inFile, line); // Read the SID value
                if (line.find(sid) != string::npos) {
                    cerr << "Error: Student ID already exists\n";
                    inFile.close();
                    return EXIT_FAILURE;
                }
            }
        }
        inFile.close();
    }
    else {
        cerr << "Error: Unable to open database file for reading\n";
        return EXIT_FAILURE;
    }

    // Add record to the file
    ofstream outFile(filename, ios::app);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open database file for writing\n";
        return EXIT_FAILURE;
    }

    // Write the record to the file in the desired format
    outFile << "#RECORD\n";
    outFile << " #SID\n";
    outFile << "     " << sid << "\n";
    outFile << " #NAME\n";
    outFile << "     " << name << "\n";
    if (!moduleCodes.empty()) {
        outFile << " #ENROLLMENTS\n";
        outFile << "     ";
        for (const auto& code : moduleCodes) {
            outFile << code << " ";
        }
        outFile << "\n";
    }

    if (!grades.empty()) {
        outFile << " #GRADES\n";
        outFile << "     ";
        for (const auto& grade : grades) {
            outFile << grade << " ";
        }
        outFile << "\n";
    }
    
    
    if (!phone.empty()) {
        outFile << " #PHONE\n";
        outFile << "     " << phone << "\n";
    }
    
    cout << "Data Added Successfully!\n";
    // Close the file
    outFile.close();

    return EXIT_SUCCESS;
}