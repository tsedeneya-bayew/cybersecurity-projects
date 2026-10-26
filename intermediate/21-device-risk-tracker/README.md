# Project 21: Cybersecurity Device Risk Tracker

[Main portfolio](../../README.md) · **Intermediate** · C++17 · OOP final project

**Status:** AI-assisted starter implementation compiled and tested on Linux. Student adaptation, code walkthrough, Windows execution, and presentation practice are pending. The course requires student-written code and understanding of every submitted line: use this as a learning scaffold, adapt it, and disclose assistance according to your instructor's rules.

Track fictional computers and servers, record vulnerability counts, calculate educational risk scores, sort assets, search by ID, and export reports. This is a manually populated inventory exercise, not a vulnerability scanner. No external libraries or network connection are needed.

## Files

- `main.cpp`: single source file with exactly one `main()` and detailed comments.
- `sample_devices.txt`: eight fictional device records that the program can import.
- `expected_results.txt`: independently calculated reference scores and levels.
- `STUDY_GUIDE.md`: requirement mapping, walkthrough, demo, and practice questions.
- `validation.txt`: actual Linux compiler and behavior-check summary.

## Compile and run

Open a terminal **in this project folder**. Dataset import and export use the terminal's current folder.

### Windows with Visual Studio C++ tools

Open **Developer PowerShell for Visual Studio** (or Developer Command Prompt), then change to the downloaded project folder:

```powershell
cl /std:c++17 /EHsc /W4 main.cpp /Fe:risk_tracker.exe
.\risk_tracker.exe
```

The normal PowerShell window may not recognize `cl`. Use the Developer shell where the compiler is configured. Windows commands are provided but have not been executed in this environment.

### Linux / Ubuntu / Kali

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o risk_tracker
./risk_tracker
```

## Dataset and scoring

Choose **8** immediately after starting to load `sample_devices.txt`. Import requires an empty tracker and rejects invalid rows or duplicate IDs without adding partial records. Restart to reload the samples.

Each non-comment line has four fields: numeric ID, type (`C` or `S`), vulnerability count, and a name that can contain spaces. Lines starting with `#` are comments. IDs range from 1 to 999999, counts from 0 to 1000, and names from 1 to 60 characters. Sample rows are intentionally out of ID order to demonstrate sorting and binary search.

| ID | Device | Type | Count | Expected score | Level |
|---|---|---|---:|---:|---|
| 101 | Backup Server | Server | 0 | 0 | Low |
| 102 | Reception PC | Computer | 0 | 0 | Low |
| 103 | Student Laptop | Computer | 3 | 30 | Medium |
| 104 | File Server | Server | 2 | 40 | Medium |
| 105 | Training PC | Computer | 6 | 60 | High |
| 106 | Lab Server | Server | 3 | 60 | High |
| 107 | Test Web Server | Server | 8 | 100 | High |
| 108 | Library PC | Computer | 2 | 20 | Low |

Computer score = count × 10. Server score = count × 20. Both cap at 100. Low is 0–29, Medium is 30–59, and High is 60–100. These are classroom assumptions, not CVSS or measured real-world risk. All data is fictional and has no real IP addresses or credentials.

## Menu

1. Add a computer or server with a unique ID.
2. View every device.
3. Add one or more vulnerabilities to an existing device.
4. Display reports and the static total-created counter.
5. Bubble-sort devices by risk, highest first, and display them.
6. Binary-search a device ID (first bubble-sorts IDs ascending).
7. Export reports to `risk_reports.txt`, replacing the previous export.
8. Import the fictional sample dataset into an empty tracker.
0. Exit.

Entries live in memory and are cleared on exit. Exported reports can be read in a text editor but cannot be reloaded. Search changes the vector's order to ascending ID; use option 5 again to restore descending risk order. Options 2 and 4 intentionally reuse the same report function; option 4 also shows the static counter.

## Extra-credit features

- Handwritten bubble sort: adjacent comparisons and swaps, O(n²) worst case.
- Handwritten binary search: interval halving, O(log n) after ordering. Each search currently sorts first, so the complete operation has O(n²) worst-case cost.
- File access: dataset input through `ifstream` and report output through `ofstream`.

No linked list or binary search tree is claimed. Extra-credit awards depend on instructor review and student implementation requirements.
