# Implementation validation

## Scope

Project 20 was implemented and exercised in a Linux development container. These are implementation checks, not personal Windows execution evidence or a malware evaluation. No real ransomware was used.

## Observed results

- GCC C++17 compiled the monitor, simulator, and core tests with `-Wall -Wextra -Wpedantic -Werror`; monitor and tests used `-pthread`.
- Deterministic tests passed for distinct-path thresholds, repeated edits to a single file, window expiry, cooldown, severity escalation, canary creation/deletion, known entropy distributions, entropy-only suppression, lab extension correlation, JSON escaping, queue capacity/drain/concurrency, invalid configuration, and event ordering.
- The ordinary single-file edit produced 1 event and 0 incidents in the automated integration run.
- The synthetic burst produced 61 events and 3 incidents in the automated integration run. Alerts included canary changes, bulk path changes, and correlated `.demo` creations/deletions. Counts can vary with polling timing.
- Tests confirmed refusal of sandbox reinitialization, unexpected sandbox files, hard-linked simulator files, simulator symlinks, reused report directories, and nested report paths. Collector symlinks were skipped.
- Integration tests parsed every generated event and incident as JSON and reconciled log counts with the final summary.

See `test-output.txt` for captured output. `example-incident.json` is a real incident from a separate synthetic run; `example-summary.json` is that run's summary. No user documents or absolute sandbox paths are included in these artifacts.

## Interpretation

These checks establish that the implemented rules and end-to-end lab workflow work for the exercised scenarios. They do not establish a real-world detection rate, a general false-positive rate, resistance to log tampering, safe operation in hostile directory trees, or production readiness.

## Windows setup checkpoint

The user's supplied terminal screenshot confirms MSVC 19.51.36260, CMake 4.3.1-msvc1, successful repository cloning, and successful x64 CMake configuration with Visual Studio 18 2026. See [Windows setup observations](windows-setup.md). Later user-supplied evidence verifies successful Windows Debug compilation, a passing direct test_detector.exe invocation, and a completed guided synthetic run with 62 events and 3 high-severity incidents. One interrupted scan recovered through the bounded retry. See [Windows runtime checkpoint](windows-runtime-checkpoint.md). Windows CTest/Python integration execution remains unverified.

## Pending work

- Run the full Python integration suite on Windows if desired; the guided Windows build, core tests and runtime demonstration are verified.
- Capture personal lab screenshots and findings.
- Evaluate legitimate bulk copying, archives, and larger workloads.
- Quantify detection delay, CPU/memory consumption, and missed events.
- Implement a native Windows watcher and process attribution as separate extensions.
