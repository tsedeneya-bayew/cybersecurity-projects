# File Integrity with SHA-256 — Findings Report

**Status:** In progress  
**Started:** October 9, 2026 (first screenshot submitted)  
**Completed:** Not yet completed

> Partial report based on Steps 1–2 evidence. Steps 3–6 remain pending.

## Progress summary

The synthetic invoice was created and its visible contents verified. Step 2 records the original SHA-256 digest and a passing baseline check (`invoice.txt: OK`). Tamper detection, restoration, and baseline trust tests remain pending.

## Environment and authorized scope

| Item | Observed value |
|---|---|
| Platform | Ubuntu VM shown running in Oracle VirtualBox; release and tool versions not shown in this screenshot |
| Prompt | tsedeneya-bayew@VM; customized display |
| Working directory | Commands target ~/portfolio-labs/integrity; prompt shows an abbreviated path ending in integrity |
| Target | invoice.txt, a fictional lab invoice |
| Tools used so far | mkdir, cd, printf, cat, sha256sum, tee; versions not captured |
| Snapshot / recovery plan | Not documented for this project |
| Timezone | Not shown |

## Evidence log

| Step | Command or action | Actual result | Evidence | Interpretation |
|---|---|---|---|---|
| 1 | mkdir -p ~/portfolio-labs/integrity; cd into that directory | No visible errors; prompt ends in integrity | [Original invoice screenshot](../screenshots/01-original.png) | Lab working directory established. |
| 1 | printf writes the fictional invoice to invoice.txt | No visible error | [Original invoice screenshot](../screenshots/01-original.png) | File creation command recorded. |
| 1 | cat invoice.txt | Invoice ID: LAB-001 and Amount: 100 on separate lines | [Original invoice screenshot](../screenshots/01-original.png) | Visible contents match the intended synthetic starting data. |
| 2 | `sha256sum invoice.txt` piped to `tee baseline.sha256` | Original digest recorded below | [Baseline screenshot](../screenshots/02-baseline.png) | Baseline generation and displayed digest recorded. |
| 2 | `sha256sum --check baseline.sha256` | invoice.txt: OK | [Baseline screenshot](../screenshots/02-baseline.png) | Current file matches the stored baseline digest. |

![Step 1 synthetic invoice evidence](../screenshots/01-original.png)

![Step 2 baseline verification evidence](../screenshots/02-baseline.png)

## Original SHA-256 digest

```text
1894293b52f6cdc4cbbb539c88c43531f5aace2f574baa302deaf0810627b2ed
```

## Observations

The file contains the expected fictional invoice identifier and amount. The shown `printf` command specifies newline separators. Step 2 records a 64-character SHA-256 digest and a passing check against `baseline.sha256`. No independent byte count is shown. The matching check supports consistency with the recorded baseline at that moment; it does not establish authorship or independently trusted baseline storage.

## Deviations and troubleshooting

No visible command errors or deviations from Steps 1–2. Exit codes were not captured.

## Validation

- **Documented:** Synthetic file creation, visible content inspection, original digest, and passing baseline comparison.
- **Pending:** Modified-file digest and failed check, restoration, and the baseline trust demonstration.

## Limitations

This evidence establishes visible starting content and a match to the locally generated digest baseline. It does not establish authorship, independent baseline trust, or any detected modification. OS release and tool versions were not captured in this screenshot.

## Cleanup / restoration

The synthetic file and lab directory are present in Step 1; baseline.sha256 is generated in Step 2. No cleanup or restoration has been reported for this project.

## Lessons learned so far

- `printf` creates controlled synthetic content and `cat` checks the visible text.
- `sha256sum --check` compares the current file against the digest recorded in the baseline.
- A matching local baseline does not prove authorship or that the baseline is protected.

## Remaining work

- [x] Step 1: create synthetic data
- [x] Step 2: establish a baseline
- [ ] Step 3: change a value and detect the mismatch
- [ ] Step 4: restore and recheck
- [ ] Step 5: demonstrate baseline trust limitations
- [ ] Step 6: complete the report and final verification
