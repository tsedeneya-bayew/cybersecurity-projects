# Python Password Strength Checker — Findings Report

**Status:** Completed  
**Started:** October 9, 2026  
**Completed:** October 20, 2026

## Summary and completion scope

The educational checker was prepared in a local VM, inspected through saved-source readback, exercised interactively, and tested with a four-test unittest suite. The observed suite passed. At the owner's request, the project is closed with the available evidence. A separate learner-written limitations response, additional screenshots, full source collection, and publication of the executed source files were omitted. Completion does not imply those activities occurred.

## Environment

| Item | Observed value |
|---|---|
| Environment | Ubuntu lab VM; exact OS release not independently recorded for this project |
| Python | 3.8.5 |
| Directory | /home/seed/portfolio-labs/password-checker |
| Editor | nano |
| Prompt | tsedeneya-bayew@VM, customized display |
| Scope | Local educational checker; invented test strings only |

## Evidence and results

| Step | Evidence | Actual result |
|---|---|---|
| Initial setup attempt | [Setup attempt](../screenshots/01-setup-attempt.png) | nano checker.py invoked from /home; user reported Permission denied when saving |
| Corrected setup | [Environment](../screenshots/01-environment.png) | Correct project directory printed; Python 3.8.5 verified |
| Source inspection | [Code](../screenshots/02-code.png) | Saved import, COMMON set, evaluate function, and getpass main block visible; final output lines clipped |
| Interactive examples | [Output](../screenshots/03-output.png) | No-issue message; minimum-length warning; length plus repetition warnings |
| Empty input | [Empty input](../screenshots/03-empty.png) | Length warning and Input is empty. |
| Automated tests | [Tests](../screenshots/04-tests.png) | Four tests each report ok; Ran 4 tests in 0.000s; OK |

## Implementation observations

The visible source uses a four-entry common-password set, a minimum length of 15 for this lab heuristic, a repeated-character check, and an empty-input check. casefold supports case-insensitive matching. evaluate returns feedback messages. getpass requests terminal input without displaying it; the execution screenshots show prompts and feedback without entered values.

The final source lines are outside the code screenshot. Subsequent successful interactive runs and tests demonstrate that the executed file parses and runs. The standalone compile check was skipped. Full source files were not collected, so the guide's reference code must not be represented as a verified byte-for-byte copy of the VM files.

## Interactive validation

Three initial runs show the no-issue fallback, a length warning, and combined length/repetition warnings. Inputs are hidden, so their exact values cannot be independently recovered from the screenshots. The combined warnings are consistent with a repeated string shorter than 15 characters, rather than the suggested 20-character example.

An additional empty-input run shows both the minimum-length warning and Input is empty. Both are expected because an empty string also has length below 15. The no-issue result means only that the limited rules found no issue; it is not proof of strength.

## Automated validation

The observed unittest output reports these tests as passing:

- test_common_is_flagged
- test_empty_is_rejected
- test_long_phrase_has_no_heuristic_issue
- test_repeated_is_flagged

The summary reports four tests and OK. No numeric exit code is shown. Test source was not collected, so the names and output establish the observed suite result without independently verifying its precise assertions or coverage.

## Troubleshooting

The initial editor invocation occurred in /home, the parent directory containing user home directories. The user reported a Permission denied error; the error text is not visible in the initial screenshot. Creating and entering ~/portfolio-labs/password-checker corrected the working location without sudo. The customized prompt does not change the actual home directory /home/seed.

## Technical limitations and lessons

This section documents the tool's limitations; it is not a claimed learner-authored reflection.

Character count does not measure how a password was chosen or how predictable it is. A long familiar phrase or pattern can remain guessable. A four-entry blocklist misses most common or compromised passwords and does not detect reuse, personal information, or credential stuffing.

The checker is an educational heuristic, not an entropy estimator or a production password policy. Passing four tests verifies limited observed behavior, not resistance to attacks. Passwords should not appear in logs because logs can be copied, retained, or accessed by others.

A production design would need a carefully maintained compromised-password screening approach, secure handling of input, broader validation, and protection against credential attacks. Raw passwords should not be sent to a third-party checking service. Password managers help create and store unique credentials, while MFA provides an additional layer of account protection. Neither was implemented or tested in this lab.

## Retention and completion

The VM and educational files are retained for learning. This lab does not document starting a network service or changing system security settings, so no service shutdown or configuration rollback is required by the recorded work. No sensitive information requiring redaction was visible in the published screenshots.

- [x] Setup and working-directory correction documented
- [x] Saved-source screenshot reviewed
- [x] Interactive feedback and empty-input case recorded
- [x] Four-test suite result recorded
- [x] Limitations and evidence boundaries stated
- [x] Omitted work identified without claiming execution
- [x] Available report finalized and project marked completed
