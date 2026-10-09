# File Integrity with SHA-256 — Findings Report

**Status:** In progress  
**Started:** October 9, 2026 (first screenshot submitted)  
**Completed:** Not yet completed

> Partial report based on Step 1 evidence. Steps 2–6 remain pending.

## Progress summary

The synthetic invoice was created and its visible contents verified. No hash, baseline, tamper-detection result, or restoration result has yet been captured.

## Environment and authorized scope

| Item | Observed value |
|---|---|
| Platform | Ubuntu VM shown running in Oracle VirtualBox; release and tool versions not shown in this screenshot |
| Prompt | tsedeneya-bayew@VM; customized display |
| Working directory | Commands target ~/portfolio-labs/integrity; prompt shows an abbreviated path ending in integrity |
| Target | invoice.txt, a fictional lab invoice |
| Tools used so far | mkdir, cd, printf, cat; versions not captured |
| Snapshot / recovery plan | Not documented for this project |
| Timezone | Not shown |

## Evidence log

| Step | Command or action | Actual result | Evidence | Interpretation |
|---|---|---|---|---|
| 1 | mkdir -p ~/portfolio-labs/integrity; cd into that directory | No visible errors; prompt ends in integrity | [Original invoice screenshot](../screenshots/01-original.png) | Lab working directory established. |
| 1 | printf writes the fictional invoice to invoice.txt | No visible error | [Original invoice screenshot](../screenshots/01-original.png) | File creation command recorded. |
| 1 | cat invoice.txt | Invoice ID: LAB-001 and Amount: 100 on separate lines | [Original invoice screenshot](../screenshots/01-original.png) | Visible contents match the intended synthetic starting data. |

![Step 1 synthetic invoice evidence](../screenshots/01-original.png)

## Observations

The file contains the expected fictional invoice identifier and amount. The shown `printf` command specifies newline separators. The screenshot confirms visible text, but no independent byte count or digest has been captured. All subsequent integrity tests remain pending.

## Deviations and troubleshooting

No visible command errors or deviations from Step 1. Exit codes were not captured.

## Validation

- **Documented:** Synthetic file creation and visible content inspection.
- **Pending:** Original SHA-256 digest, baseline check, modified-file digest and failed check, restoration, and the baseline trust demonstration.

## Limitations

This evidence establishes visible file contents only. It does not prove file integrity, authorship, a protected baseline, or any detected modification. OS release and tool versions were not captured in this screenshot.

## Cleanup / restoration

The synthetic file and lab directory are present at the end of Step 1. No cleanup or restoration has been reported for this project.

## Lessons learned so far

- `printf` creates controlled synthetic content and `cat` checks the visible text.
- File-integrity testing needs a recorded digest and comparison; viewing a file alone is not an integrity test.

## Remaining work

- [x] Step 1: create synthetic data
- [ ] Step 2: establish a baseline
- [ ] Step 3: change a value and detect the mismatch
- [ ] Step 4: restore and recheck
- [ ] Step 5: demonstrate baseline trust limitations
- [ ] Step 6: complete the report and final verification
