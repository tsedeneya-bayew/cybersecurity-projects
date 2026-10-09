# Digital Forensics Investigation

**Level:** Intermediate · **Suggested time:** 3–5 hours · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Portfolio index](https://github.com/tsedeneya-bayew/cybersecurity-portfolio)

## Objective

Complete a reproducible digital forensics investigation exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Ubuntu VM, Python 3, zip/unzip, sha256sum. Uses synthetic logical files; no disk imaging or recovered real user data.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Generate a fictional evidence set

```bash
sudo apt update
sudo apt install zip unzip python3
mkdir -p ~/portfolio-labs/forensics/source
cd ~/portfolio-labs/forensics
printf '2026-01-01T10:00:00Z login lab_user\n2026-01-01T10:03:00Z download report.txt\n' > source/activity.log
printf 'Synthetic report: lab-only content\n' > source/report.txt
printf 'meeting note\n' > source/notes.txt
zip -r evidence.zip source
sha256sum evidence.zip | tee evidence.sha256
```
Identify acquisition as a self-generated synthetic fixture. Do not describe it as a seized device or forensic disk image.

**Screenshot checkpoint:** 01-acquisition.png: file list and evidence digest.

### 2. Preserve an original and working copy

```bash
mkdir -p original working
cp evidence.zip evidence.sha256 original/
chmod a-w original/evidence.zip
cp original/evidence.zip working/
sha256sum original/evidence.zip working/evidence.zip
```
Both copies should match. A read-only mode is an organizational precaution, not a forensic write blocker. Record an evidence ledger with filename, digest, timestamp, analyst role, and each copy operation.

**Screenshot checkpoint:** 02-preservation.png: matching hashes.

### 3. Extract only the working copy

```bash
cd working
unzip evidence.zip -d extracted
find extracted -type f -print
find extracted -type f -exec sha256sum {} \;
```
Inspect extracted synthetic files. ZIP timestamps and current filesystem timestamps may differ in timezone and precision; do not treat extraction time as original creation time.

**Screenshot checkpoint:** 03-inventory.png: extracted files and hashes.

### 4. Build a timeline

Use explicit UTC timestamps from activity.log to create an event table. Link each row to its file and line number. Distinguish recorded event time from ZIP metadata and analysis time. Describe the download event as a log claim, not proof of external exfiltration.

**Screenshot checkpoint:** 04-timeline.png: two-event timeline with evidence references.

### 5. Analyze content and uncertainty

Compare report.txt content with the activity log. Identify what the fixture supports and what it cannot establish: no deleted-file recovery, process telemetry, authenticated identity, or network evidence. Explain that live file metadata can be altered and a hash proves byte identity, not truth of contents.

**Screenshot checkpoint:** 05-analysis.png: observations and limitations.

### 6. Validate original integrity and report

```bash
cd ~/portfolio-labs/forensics/original
sha256sum --check evidence.sha256
```
Expected: evidence.zip: OK for the preserved original. The manifest references a relative filename, so run from the original directory. Include evidence ledger, inventory, timeline, findings, limitations, and final original digest. Retain synthetic artifacts only.

**Screenshot checkpoint:** 06-final-integrity.png: passing original integrity check.

## Troubleshooting

Work from the expected directory because checksum manifests store relative filenames. Extraction timestamps are not original event times. If hashes differ, stop and document the discrepancy rather than silently recreating the baseline.

## Questions to answer in your report

- What does a hash prove about evidence?
- How do logical-file analysis and disk forensics differ?
- Why separate observations from event hypotheses?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://www.gnu.org/software/coreutils/manual/html_node/sha2-utilities.html
- https://docs.python.org/3/library/zipfile.html
