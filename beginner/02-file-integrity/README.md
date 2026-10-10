# File Integrity with SHA-256

**Level:** Beginner · **Suggested time:** 45–90 minutes · **Status:** In progress

> Lab in progress. Steps 1–4 are evidenced below; Steps 5–6 remain pending.

[← Project index](../../README.md)

## Objective

Complete a reproducible file integrity with sha-256 exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Ubuntu VM with sha256sum (coreutils). No privileged access required.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. Steps 1–4 evidence is included below; add later screenshots as you complete the lab.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Create synthetic data

```bash
mkdir -p ~/portfolio-labs/integrity
cd ~/portfolio-labs/integrity
printf 'Invoice ID: LAB-001\nAmount: 100\n' > invoice.txt
cat invoice.txt
```
Use this fictional invoice only. Hashes describe bytes, so spaces and line endings matter.

**Screenshot checkpoint:** 01-original.png: synthetic invoice contents.

![Step 1: creation and inspection of the synthetic invoice](screenshots/01-original.png)

**Observed result:** The screenshot shows creation of `~/portfolio-labs/integrity`, changing into that directory, and writing `invoice.txt` with `printf`. `cat invoice.txt` displays `Invoice ID: LAB-001` and `Amount: 100` on separate lines. No errors are visible. Step 1 is complete; no SHA-256 digest or baseline verification has yet been captured.

### 2. Establish a trusted baseline

```bash
sha256sum invoice.txt | tee baseline.sha256
sha256sum --check baseline.sha256
```
A SHA-256 digest is 64 hexadecimal characters. `--check` recomputes the hash and compares it with the stored digest. Expected result is `invoice.txt: OK`. The baseline must be protected separately from the monitored data.

**Screenshot checkpoint:** 02-baseline.png: digest and successful verification.

![Step 2: SHA-256 baseline and successful verification](screenshots/02-baseline.png)

**Observed result:** `sha256sum invoice.txt | tee baseline.sha256` displays the digest below and writes the baseline. `sha256sum --check baseline.sha256` returns `invoice.txt: OK`. No errors or exit codes are shown. This establishes a matching local baseline at this stage; separate protection or authenticity of that baseline has not been demonstrated.

```text
1894293b52f6cdc4cbbb539c88c43531f5aace2f574baa302deaf0810627b2ed  invoice.txt
```

### 3. Change one value

```bash
cp invoice.txt invoice-original.txt
sed -i 's/Amount: 100/Amount: 101/' invoice.txt
sha256sum invoice.txt
sha256sum --check baseline.sha256
```
The comparison should fail because the file has changed. Record the exit code immediately with `echo $?`. This detects modification, but does not identify who changed the file.

**Screenshot checkpoint:** 03-tamper-detected.png: changed invoice and failed check.

![Step 3: modified invoice digest and expected baseline mismatch](screenshots/03-tamper-detected.png)

**Observed result:** The screenshot shows copying the original invoice and running the substitution from `Amount: 100` to `Amount: 101`. The new SHA-256 digest is shown below. Checking the original baseline returns `invoice.txt: FAILED` and a warning that one computed checksum did not match. This is the expected modification-detection result. Changed file contents and the exit code are not separately displayed.

```text
16d7cb06a3aae288ff23562281cf17383b665826937b69579a04af5f4b711d98  invoice.txt
```

### 4. Restore and recheck

```bash
cp invoice-original.txt invoice.txt
sha256sum --check baseline.sha256
```
Expected: OK again. Do not overwrite the baseline just to make an unexpected change disappear.

**Screenshot checkpoint:** 04-restored.png: restored contents and passing check.

![Step 4: restore original invoice and pass baseline check](screenshots/04-restored.png)

**Observed result:** The screenshot shows copying `invoice-original.txt` back to `invoice.txt`, then checking the original `baseline.sha256`. The result is `invoice.txt: OK`, confirming that the restored file matches the recorded original digest. File contents and the numeric exit code are not displayed in this screenshot.

### 5. Demonstrate the trust limitation

```bash
printf 'Amount: 999\n' >> invoice.txt
sha256sum invoice.txt > compromised-baseline.sha256
sha256sum --check compromised-baseline.sha256
sha256sum --check baseline.sha256
```
The newly generated baseline passes while the original baseline fails. An attacker able to alter both data and baseline can defeat this simple comparison. Explain protected baselines, digital signatures, or authenticated hashes as stronger trust mechanisms.

**Screenshot checkpoint:** 05-trust-limit.png: contrasting checks.

### 6. Write the report

Record original digest, changed digest, commands, exit codes, restoration outcome, and the baseline trust limitation. Leave the original baseline intact. Retain only synthetic artifacts; no real invoices.

**Screenshot checkpoint:** 06-summary.png: final verification and report excerpt.

## Troubleshooting

Run commands from the same directory used to create the baseline. A missing file is different from a digest mismatch. Windows CRLF line endings can change a digest without changing visible text.

## Questions to answer in your report

- Does a matching hash prove authorship?
- Where should the baseline be stored?
- Why does a small byte change alter the digest?

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
