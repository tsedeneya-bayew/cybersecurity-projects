// Harmless file-activity generator. No encryption, deletion, or process control.
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
namespace fs = std::filesystem;
const std::string marker = "CXX_FILE_ACTIVITY_LAB_V1";
void write_text(const fs::path& path, const std::string& text) {
    std::ofstream out(path, std::ios::binary);
    out << text;
    out.close();
    if (!out) throw std::runtime_error("Cannot write lab file");
}
std::string filename(int n) { return "sample_" + std::to_string(n) + ".txt"; }
void validate(const fs::path& root) {
    if (fs::is_symlink(root) || !fs::is_directory(root)) throw std::runtime_error("Invalid sandbox");
    std::set<std::string> expected{".lab-marker", "CANARY_DO_NOT_EDIT.txt"};
    for (int i = 0; i < 40; ++i) expected.insert(filename(i));
    std::set<std::string> actual;
    for (const auto& item : fs::directory_iterator(root)) {
        if (item.is_symlink() || !item.is_regular_file()) throw std::runtime_error("Links/subfolders refused");
        if (fs::hard_link_count(item.path()) != 1) throw std::runtime_error("Hard links refused");
        actual.insert(item.path().filename().string());
    }
    if (actual != expected) throw std::runtime_error("Sandbox contents changed; use a fresh lab sandbox");
    std::ifstream in(root / ".lab-marker");
    std::string value; std::getline(in, value);
    if (value != marker) throw std::runtime_error("Missing lab marker");
}
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: simulator init|normal|burst PATH/ransomware-lab-sandbox\n";
        return 1;
    }
    try {
        auto root = fs::absolute(argv[2]).lexically_normal();
        if (root.filename() != "ransomware-lab-sandbox")
            throw std::runtime_error("Sandbox name must be ransomware-lab-sandbox");
        const std::string mode = argv[1];
        if (mode == "init") {
            if (!fs::create_directory(root)) throw std::runtime_error("Init requires a NEW sandbox");
            write_text(root / ".lab-marker", marker + "\n");
            write_text(root / "CANARY_DO_NOT_EDIT.txt", "Decoy for this disposable lab only.\n");
            for (int i = 0; i < 40; ++i)
                write_text(root / filename(i), "Synthetic training document " + std::to_string(i) + "\n");
        } else if (mode == "normal" || mode == "burst") {
            validate(root);
            if (mode == "normal") write_text(root / filename(0), "A single ordinary edit to a synthetic document.\n");
            else {
                for (int i = 0; i < 30; ++i) {
                    // Plain text only; .demo is an explicitly lab-specific indicator.
                    write_text(root / filename(i), "SIMULATED BURST ACTIVITY - PLAIN TEXT - " + std::to_string(i) + "\n");
                    fs::rename(root / filename(i), root / (filename(i) + ".demo"));
                }
                write_text(root / "CANARY_DO_NOT_EDIT.txt", "SIMULATED CANARY CHANGE\n");
            }
        } else throw std::runtime_error("Unknown simulator mode");
        std::cout << "Simulator completed: " << mode << '\n';
        return 0;
    } catch (const std::exception& e) { std::cerr << "ERROR: " << e.what() << '\n'; return 2; }
}
