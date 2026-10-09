# Local Network Discovery with Nmap

**Level:** Beginner · **Suggested time:** 1–2 hours · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Project index](../../README.md)

## Objective

Complete a reproducible local network discovery with nmap exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Ubuntu VM with Python 3 and Nmap. The required exercise scans 127.0.0.1 only. An optional second VM must be on an isolated host-only network.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Prepare local tools

```bash
sudo apt update
sudo apt install nmap python3
nmap --version
mkdir -p ~/portfolio-labs/discovery
cd ~/portfolio-labs/discovery
printf 'Local discovery lab\n' > index.html
```
Record tool version and define the authorized target as 127.0.0.1.

**Screenshot checkpoint:** 01-scope.png: version and scope statement.

### 2. Start one known service

```bash
python3 -m http.server 8000 --bind 127.0.0.1
```
Leave this terminal open. In a second terminal run `ss -lnt` and locate port 8000. This gives you ground truth to compare with the scanner.

**Screenshot checkpoint:** 02-listener.png: loopback listener on port 8000.

### 3. Scan a narrow port range

```bash
nmap -sT -p 7999-8001 127.0.0.1 -oN local-ports.txt
```
`-sT` uses TCP connect scanning without raw-packet privileges. `-p` limits ports. `-oN` writes readable output. Port 8000 should be open; neighboring ports are usually closed unless other local services exist. Record actual results.

**Screenshot checkpoint:** 03-port-scan.png: three-port result.

### 4. Identify the service

```bash
nmap -sT -sV -p 8000 127.0.0.1 -oN local-service.txt
```
`-sV` sends service probes. Distinguish a detected service/version from a confirmed vulnerability. A banner can be incomplete or misleading.

**Screenshot checkpoint:** 04-service.png: detected service and version, if returned.

### 5. Compare after shutdown

Stop the server with Ctrl+C. Repeat:
```bash
nmap -sT -p 8000 127.0.0.1 -oN after-shutdown.txt
```
Explain the before/after difference. If another program occupies port 8000, use `ss -lntp` to investigate within the VM.

**Screenshot checkpoint:** 05-after.png: port state after service shutdown.

### 6. Report an asset inventory

Create a table of target, port, protocol, service, evidence, and uncertainty. An optional extension starts the server on a second isolated VM, bound only to its host-only IP, then scans that single IP and port. Do not discover or scan campus, employer, internet, or home-router ranges as part of this exercise.

**Screenshot checkpoint:** 06-inventory.png: completed synthetic asset table.

## Troubleshooting

Connection refused often means the server stopped or the port is wrong. A filtered result is different from a closed result. Verify the service with `ss` before drawing conclusions.

## Questions to answer in your report

- How do open, closed, and filtered differ?
- Why does a service banner not prove a vulnerability?
- Why should scope specify exact targets?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://nmap.org/book/man.html
