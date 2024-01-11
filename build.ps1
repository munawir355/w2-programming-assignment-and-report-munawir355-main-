
. .\MyFunctions.ps1

Push-Location
if ($args[0] -eq "VS") {
    cd .\tasks-VS2022
    delIfExists(".\\x64\\Debug\\01-querydb.exe")
    delIfExists(".\\x64\\Debug\\02-addrecord.exe")
    delIfExists(".\\x64\\Debug\\03-updaterecord.exe")
    msbuild comp1000C1W2.sln
    clear
    cd x64\Debug
} elseif ($args[0] -eq "QT") {
    cd .\tasks
    delIfExists(".\\build")
    mkdir build
    cmake -DCMAKE_PREFIX_PATH=c:C:\Qt\6.5.1\msvc2019_64\ -S .\01-querydb\ -B .\build\
    cd build
    MSBuild.exe .\querydb.sln
    mv .\Debug\querydb.exe .\01-querydb.exe
    if (Test-Path -Path .\01-querydb.exe) {
        banner "Qt BUILD of 01-querydb.exe complete"    
    } else {
        banner "Failed to build 01-querydb.exe with Qt"
        return
    }
} else {
    banner "Please specify which IDE is used:`nbuild.ps1 VS`nbuild.ps1 QT"
    return 
}

Clear-Host
Write-Host "THE FOLLOWING TESTS 1-10 CHECK FOR REGRESSIONS`n(THE STARTER CODE SHOULD PASS TESTS 1-4 and 6-10)`nThey should have spotted that Test5 fails"
Read-Host

banner "TEST 1: did it build?" $true
#echo "-----------------------`n"
if (Test-Path -Path ".\\01-querydb.exe") {
    Write-Host "PASS: Executable File exists."
} else {
    Write-Host "FAIL: 01-querydb.exe does not exist. Did the code build?"
}
evalOutput("PASS: Executable File exists.")

banner "TEST 2: try to open a database when none exists" $true
delIfExists(".\\computing.txt")
.\01-querydb.exe -db computing.txt
evalOutput("There should be an error indicating that the database does not exist")

banner "TEST 3: run with no parameters, creates a database" $true
delIfExists(".\\computing.txt")
.\01-querydb.exe
if (Test-Path -Path ".\\computing.txt") {
    Write-Host "PASS: computing.txt exists"
} else {
    Write-Host "ERROR: Default database missing"
}
evalOutput("There should be output that states computing.txt exists")

banner "TEST 4: confirm database exists using -db"  $true
.\01-querydb.exe -db computing.txt
evalOutput("Data base: computing.txt")

banner "TEST 5: confirm error when database filename is missing"  $true
try {
    .\01-querydb.exe -db
}
catch {
    Write-Host "ERROR: Looks like this one failed (and the student did not test and/or fix it)"
}
finally {
    evalOutput("There should be output that states the database file is not specified")
}

banner "TEST 6: -showAll flag" $true
.\01-querydb.exe -db computing.txt -showAll
evalOutput("Three student records should appear as described in the spec")

banner "TEST 7: -sid 12345"  $true
.\01-querydb.exe -db computing.txt -sid 12345
evalOutput("One student (Jo Kingly Blunt) record with ID 12345 should appear")

banner "TEST 8: -sid 15309"  $true
.\01-querydb.exe -db computing.txt -sid 15309
evalOutput("One student record (Gee Refferty) with ID 15309 should appear")

banner "TEST 9: -sid 9999 (invalid ID)" $true
.\01-querydb.exe -db computing.txt -sid 9999
evalOutput("An error message indicating that the student ID 9999 does not exist")

banner "TEST 10: -sid a12345 (invalid ID format)" $true
.\01-querydb.exe -db computing.txt -sid a12345
evalOutput("An error message indicating that the student ID must be an integer")

Clear-Host
Write-Host ("THE FOLLOWING TESTS FAIL WITH THE STARTER CODE`nTHIS IS THE POINT WHERE STUDENTS SHOULD IMPLEMENT MISSING FEATURES")
Read-Host

banner "TEST 11: -n flag" $true
.\01-querydb.exe -db computing.txt -sid 12345 -n
evalOutput("Only the name tag 'NAME:' and the name 'Jo Kingly Blunt' should appear")

banner "TEST 12: -p flag" $true
.\01-querydb.exe -db computing.txt -sid 12345 -p
evalOutput("Only the 'PHONE:' tag and number '44-1234-567890' should appear")

banner "TEST 13: -g flag" $true
.\01-querydb.exe -db computing.txt -sid 12345 -p
$expop = "`nENROLLMENTS:`n   COMP101 COMP102 COMP105 COMP110 COMP150`nGRADES:`n   54 67.5 33.1 78.4 47.1`n"
evalOutput("The output $expop should appear")

banner "TEST 14: -n -p flag" $true
.\01-querydb.exe -db computing.txt -sid 12345 -n -p
evalOutput("Only the 'NAME:' and 'PHONE:' tag and number '44-1234-567890' should appear")

Pop-Location




