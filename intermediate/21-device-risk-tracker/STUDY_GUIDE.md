# Code walkthrough and presentation practice

Use this guide alongside `main.cpp`. Before submission, explain each function in your own words and make your own changes. The source is about 340 lines including comments and blank lines; confirm that this fits your professor's approximate 300-line target.

## Requirement mapping

| Requirement | Code to show | What to explain |
|---|---|---|
| Classes | `Device`, `Computer`, `Server` | A class describes an object's data and behavior. |
| Objects | `make_unique<Computer>` / `make_unique<Server>` | These expressions construct actual devices. |
| Pointers | `unique_ptr<Device>`, `Device*` | Smart pointers own objects; raw pointers borrow them during search/update. |
| Inheritance | `class Server : public Device` | A server reuses common device state and behavior. |
| Overloading | Both `addVulnerability` methods | Compiler chooses by the number/type of arguments. |
| Overriding | `calculateRisk() const override` | Runtime dispatch selects the derived implementation. |
| Encapsulation | Private members in `Device` | Callers cannot directly change ID, name, or count. |
| Templates | `displayValue<T>` | Same function handles strings and integers with type checking. |
| Static | `Device::totalCreated` | One counter is shared across all device instances. |
| Vectors | `DeviceList` | A vector holds a variable number of owning pointers. |
| One main | `int main()` | Starts the program and dispatches menu choices. |
| Detailed comments | Throughout `main.cpp` | Explain intent, ownership, algorithms, and assumptions. |

## Read the code in this order

1. **Includes:** console I/O, files, string parsing, strings, vectors, smart pointers, moving/swapping, exceptions.
2. **displayValue:** `T` is inferred from the argument. `ostream&` lets console and file reports share code. A reference avoids copying the stream.
3. **readNumber:** reads one line, parses exactly one integer, rejects extra characters and out-of-range values, and exits cleanly at end-of-input.
4. **Device:** constructor initializes private state. Getters expose read-only values. `const` methods promise not to modify the object. Both overloads enforce a maximum of 1000 vulnerabilities.
5. **Virtual functions:** `= 0` makes Device abstract. You cannot instantiate Device directly. Its virtual destructor supports deletion through a base-class pointer.
6. **Derived classes:** public inheritance represents an 'is a Device' relationship. Each class supplies its type name and scoring formula. Server with three vulnerabilities scores 60; Computer with three scores 30.
7. **DeviceList:** `vector<unique_ptr<Device>>` can contain both concrete types. Smart pointers prevent manual memory leaks, and preserve derived types without slicing.
8. **bubbleSort:** compare adjacent elements, swap out-of-order pointers, shrink the unsorted section, and stop if a pass makes no swaps. `byId` chooses which ordering to apply.
9. **findDevice:** sort ascending by ID, repeatedly inspect the middle, and discard half the interval. `right` is excluded. A failed search returns `nullptr`.
10. **Menu helpers:** add checks duplicate IDs, update finds a borrowed pointer and exercises both overloads, report invokes virtual methods, export checks open and flush failures.
11. **loadSamples:** read the text dataset, validate it into a temporary vector, and move the vector into the tracker only when the full file is valid.
12. **main:** owns the collection, repeats the menu, and routes choices through a switch. Local smart pointers clean up when the collection is destroyed.

## Five-minute presentation demo

1. Show `Device` and its derived classes. Point to private fields, overloaded methods, and overridden virtual functions.
2. Run the program and choose **8**. Explain that eight sample records are fictional and imported from the text file.
3. Choose **4**. Compare Student Laptop (30, Medium) with Lab Server (60, High). Both have three vulnerabilities. Show the shared static counter.
4. Choose **5**. Test Web Server should appear first with score 100. Explain the adjacent-swap algorithm.
5. Choose **6**, then **106**. Show Lab Server. Explain why binary search first orders by ID. Try **999** to show a missing result.
6. Choose **3**, ID **102**, count **1**. This calls the zero-argument overload; Reception PC now scores 10.
7. Choose **3**, ID **102**, count **2**. This calls the integer overload; Reception PC now scores 30 (Medium).
8. Choose **1** and ID **102** to demonstrate duplicate rejection. Enter letters at the next menu prompt to demonstrate input validation.
9. Choose **7** and open `risk_reports.txt`. Explain the export replaces the prior report and is not a database.
10. Choose **0** and explain memory cleanup and data lifetime.

## Questions to practice

- How does overloading differ from overriding?
- Why is Device abstract, and why is its destructor virtual?
- Why use pointers rather than `vector<Device>`?
- Who owns the pointer returned by findDevice? Why must you not delete it?
- Why is the static counter defined outside the class?
- Why must the vector be sorted before binary search?
- Why is the entire find operation slower than O(log n) here?
- What happens on an empty vector, a duplicate ID, invalid input, or a missing file?
- What does `std::move` do when importing the temporary vector?
- Why does the risk score stop at 100, and why is it not a real risk standard?

## Limits to understand

The static counter counts successful object constructions over the process lifetime. If a sample import fails after constructing some temporary objects, those discarded objects still count as created. It is not an active-device count. There is no deletion feature, severity data, network scanning, or automatic vulnerability discovery.

A useful personal extension is adding a field such as device location, then updating the constructor, getters, dataset, validation, and report yourself. Keep the final program simple enough to explain every line.
