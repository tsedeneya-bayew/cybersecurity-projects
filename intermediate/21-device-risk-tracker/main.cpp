// Cybersecurity Device Risk Tracker - OOP final project learning scaffold.
// AI-assisted starter: review, adapt, and understand every line before submission.
// Uses synthetic entries only; this program does not scan real devices.
// REQUIREMENT MAP FOR THE PRESENTATION
// Classes: Device, Computer, Server model the tracked assets.
// Objects: make_unique constructs a Computer or Server on the heap.
// Pointers: unique_ptr owns each object; Device* temporarily borrows it.
// Inheritance: both concrete types publicly inherit Device.
// Overloading: addVulnerability() and addVulnerability(int).
// Overriding: calculateRisk() and getType() vary by device type.
// Encapsulation: id, name, and vulnerabilities are private.
// Templates: displayValue<T> accepts different printable value types.
// Static: totalCreated belongs to Device rather than each object.
// Vectors: DeviceList is a growable collection of owning pointers.
// Extra credit: bubbleSort, findDevice, and saveReports.
//
// SCORING RULES (classroom assumptions, not CVSS)
// Computer score = vulnerability count * 10, capped at 100.
// Server score = vulnerability count * 20, capped at 100.
// Low = 0-29; Medium = 30-59; High = 60-100.
// Zero vulnerabilities means zero score in this simplified model.
// Real risk assessment would also consider severity and exposure.
//
// DATA LIFETIME
// Entries remain in memory until exit; exports do not reload automatically.
// IDs must be unique; names may repeat because IDs identify devices.
// Searching sorts by ID, so it changes the displayed collection order.
// Select menu option 5 again whenever descending risk order is desired.
// All resources are released automatically when main exits.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>

// A function template works with both integer and string values.
// The stream parameter lets the same function print to console or file.
template <typename T>
void displayValue(std::ostream& out, const std::string& label, const T& value)
{
    out << label << ": " << value << '\n';
}

// Read an entire line so invalid input cannot leave characters in cin.
// EOF throws an exception that main catches to exit cleanly.
int readNumber(const std::string& prompt, int minimum, int maximum)
{
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line))
            throw std::runtime_error("Input closed.");
        std::istringstream input(line);
        int value;
        char extra;
        if ((input >> value) && !(input >> extra)
            && value >= minimum && value <= maximum)
            return value;
        std::cout << "Enter a whole number from " << minimum
                  << " to " << maximum << ".\n";
    }
}

// Abstract base class: common state is hidden behind a public interface.
class Device {
private:
    int id;
    std::string name;
    int vulnerabilities = 0;
    static int totalCreated; // One shared counter, not one per object.
public:
    Device(int deviceId, const std::string& deviceName)
        : id(deviceId), name(deviceName)
    {
        ++totalCreated;
    }
    // A virtual destructor permits safe deletion through a base pointer.
    virtual ~Device() = default;
    int getId() const { return id; }
    int getVulnerabilities() const { return vulnerabilities; }
    static int getTotalCreated() { return totalCreated; }

    // Overloading: same method name, different parameter lists.
    void addVulnerability() { addVulnerability(1); }
    void addVulnerability(int count)
    {
        // Keep the internal count within a safe, documented range.
        if (count < 1 || count > 1000 - vulnerabilities)
            throw std::invalid_argument("Total must not exceed 1000.");
        vulnerabilities += count;
    }

    // Pure virtual functions force derived classes to supply behavior.
    virtual int calculateRisk() const = 0;
    virtual std::string getType() const = 0;
    std::string riskLevel() const
    {
        int score = calculateRisk();
        if (score >= 60) return "High";
        if (score >= 30) return "Medium";
        return "Low";
    }
    void printReport(std::ostream& out) const
    {
        displayValue(out, "Device ID", id);
        displayValue(out, "Name", name);
        displayValue(out, "Type", getType());
        displayValue(out, "Vulnerabilities", vulnerabilities);
        displayValue(out, "Risk Score", calculateRisk());
        displayValue(out, "Risk Level", riskLevel());
        out << "------------------------------\n";
    }
};
// Define storage for the static member outside the class.
int Device::totalCreated = 0;

// Inheritance reuses the base class state and report function.
class Computer : public Device {
public:
    Computer(int id, const std::string& name) : Device(id, name) {}
    std::string getType() const override { return "Computer"; }
    int calculateRisk() const override
    {
        int score = getVulnerabilities() * 10;
        return score > 100 ? 100 : score;
    }
};

class Server : public Device {
public:
    Server(int id, const std::string& name) : Device(id, name) {}
    std::string getType() const override { return "Server"; }
    int calculateRisk() const override
    {
        // Servers receive more points per vulnerability in this toy model.
        int score = getVulnerabilities() * 20;
        return score > 100 ? 100 : score;
    }
};

// The vector owns devices through smart pointers; no manual delete is needed.
using DeviceList = std::vector<std::unique_ptr<Device>>;

// Handwritten bubble sort. byId selects ascending IDs or descending risk.
// Swapping smart pointers changes order without copying device objects.
void bubbleSort(DeviceList& devices, bool byId)
{
    for (std::size_t end = devices.size(); end > 1; --end) {
        bool swapped = false;
        for (std::size_t j = 0; j + 1 < end; ++j) {
            bool wrongOrder = byId
                ? devices[j]->getId() > devices[j + 1]->getId()
                : devices[j]->calculateRisk() < devices[j + 1]->calculateRisk();
            if (wrongOrder) {
                std::swap(devices[j], devices[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Already ordered: stop early.
    }
}

// Binary search requires ascending IDs, so establish that order first.
// [left, right) is a half-open interval; it also handles an empty vector.
Device* findDevice(DeviceList& devices, int id)
{
    bubbleSort(devices, true);
    std::size_t left = 0;
    std::size_t right = devices.size();
    while (left < right) {
        std::size_t middle = left + (right - left) / 2;
        if (devices[middle]->getId() == id)
            return devices[middle].get(); // Borrowed pointer; do not delete it.
        if (devices[middle]->getId() < id)
            left = middle + 1;
        else
            right = middle;
    }
    return nullptr;
}

void showAll(const DeviceList& devices, std::ostream& out)
{
    if (devices.empty()) out << "No devices yet.\n";
    for (const auto& device : devices)
        device->printReport(out); // Calls the appropriate virtual methods.
}

void addDevice(DeviceList& devices)
{
    int id = readNumber("Device ID (1-999999): ", 1, 999999);
    if (findDevice(devices, id)) {
        std::cout << "That ID already exists.\n";
        return;
    }
    int type = readNumber("Type (1=Computer, 2=Server): ", 1, 2);
    std::string name;
    do {
        std::cout << "Device name (1-60 characters): ";
        if (!std::getline(std::cin, name))
            throw std::runtime_error("Input closed.");
    } while (name.size() > 60 || name.find_first_not_of(" \t\r") == std::string::npos);
    if (type == 1)
        devices.push_back(std::make_unique<Computer>(id, name));
    else
        devices.push_back(std::make_unique<Server>(id, name));
    std::cout << "Device added.\n";
}

void updateDevice(DeviceList& devices)
{
    int id = readNumber("Device ID: ", 1, 999999);
    Device* device = findDevice(devices, id);
    if (!device) {
        std::cout << "Device not found.\n";
        return;
    }
    int remaining = 1000 - device->getVulnerabilities();
    if (remaining == 0) {
        std::cout << "Vulnerability limit reached.\n";
        return;
    }
    int count = readNumber("How many vulnerabilities to add? ", 1, remaining);
    // Exercise both overloads during normal menu use.
    if (count == 1) device->addVulnerability();
    else device->addVulnerability(count);
    device->printReport(std::cout);
}

void searchDevice(DeviceList& devices)
{
    int id = readNumber("Search for ID: ", 1, 999999);
    Device* result = findDevice(devices, id);
    if (result) result->printReport(std::cout);
    else std::cout << "Device not found.\n";
}

// File access: export a human-readable snapshot; this is not a load function.
void saveReports(const DeviceList& devices)
{
    std::ofstream file("risk_reports.txt"); // Replaces the previous export.
    if (!file) {
        std::cout << "Could not open risk_reports.txt for writing.\n";
        return;
    }
    file << "DEVICE RISK REPORT - educational scoring only\n";
    showAll(devices, file);
    file.close(); // Check errors that can occur while flushing buffered data.
    if (file.fail()) std::cout << "Could not finish writing the report.\n";
    else std::cout << "Saved risk_reports.txt in the current folder.\n";
}

// Import the simple text dataset: ID, type (C/S), count, then name.
// Stage all rows first so an invalid file cannot partially change the tracker.
void loadSamples(DeviceList& devices)
{
    if (!devices.empty()) {
        std::cout << "Load samples into an empty tracker. Restart first.\n";
        return;
    }
    std::ifstream file("sample_devices.txt");
    if (!file) {
        std::cout << "Cannot open sample_devices.txt in the current folder.\n";
        return;
    }
    DeviceList pending;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream row(line);
        int id, count;
        char type;
        std::string name;
        if (!(row >> id >> type >> count) || !std::getline(row >> std::ws, name)
            || id < 1 || id > 999999 || count < 0 || count > 1000
            || (type != 'C' && type != 'S') || name.size() > 60
            || name.find_first_not_of(" \t\r") == std::string::npos
            || findDevice(pending, id)) {
            std::cout << "Invalid or duplicate sample row. Nothing loaded.\n";
            return;
        }
        std::unique_ptr<Device> device;
        if (type == 'C') device = std::make_unique<Computer>(id, name);
        else device = std::make_unique<Server>(id, name);
        if (count > 0) device->addVulnerability(count);
        pending.push_back(std::move(device));
    }
    if (file.bad()) {
        std::cout << "Dataset read failed. Nothing loaded.\n";
        return;
    }
    devices = std::move(pending);
    std::cout << "Loaded " << devices.size() << " fictional sample devices.\n";
}

// The program contains exactly one main function.
int main()
{
    DeviceList devices;
    std::cout << "Educational tracker: enter sample devices manually.\n";
    try {
        while (true) {
            std::cout << "\nCYBERSECURITY DEVICE RISK TRACKER\n"
                      << "1. Add a computer or server\n"
                      << "2. View all devices\n"
                      << "3. Add vulnerabilities\n"
                      << "4. Display risk reports and total\n"
                      << "5. Sort devices by risk (highest first)\n"
                      << "6. Search by device ID\n"
                      << "7. Save reports to a file\n"
                      << "8. Load fictional sample dataset\n"
                      << "0. Exit\n";
            int choice = readNumber("Choice: ", 0, 8);
            if (choice == 0) break;
            switch (choice) {
            case 1: addDevice(devices); break;
            case 2: showAll(devices, std::cout); break;
            case 3: updateDevice(devices); break;
            case 4:
                showAll(devices, std::cout);
                displayValue(std::cout, "Devices created", Device::getTotalCreated());
                break;
            case 5:
                bubbleSort(devices, false);
                showAll(devices, std::cout);
                break;
            case 6: searchDevice(devices); break;
            case 7: saveReports(devices); break;
            case 8: loadSamples(devices); break;
            }
        }
    } catch (const std::exception& error) {
        std::cout << error.what() << '\n';
    }
    std::cout << "Goodbye. Unsaved entries are cleared on exit.\n";
    return 0;
}
