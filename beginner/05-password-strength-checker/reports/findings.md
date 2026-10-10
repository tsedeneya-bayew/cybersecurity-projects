# Python Password Strength Checker — Findings Report

**Status:** In progress  
**Started:** October 9, 2026  
**Completed:** Not yet completed

## Progress summary

Step 1 is verified after correcting the working directory. The new screenshot shows /home/seed/portfolio-labs/password-checker and Python 3.8.5. Saved implementation and test results remain pending.

## Environment and scope

| Item | Observed value |
|---|---|
| Environment | Existing lab VM; exact OS version not shown in this screenshot |
| Prompt | tsedeneya-bayew@VM, customized display |
| Corrected project directory | /home/seed/portfolio-labs/password-checker |
| Editor command | nano checker.py |
| Python version | 3.8.5 |
| Scope | Educational local checker using invented test strings only |

## Evidence log

| Step | Action | Actual result | Evidence | Interpretation |
|---|---|---|---|---|
| 1 initial attempt | Run cd .. three times, then nano checker.py | Terminal reaches /home; editor command returns to shell | [Setup attempt](../screenshots/01-setup-attempt.png) | Editor invocation documented; file creation and saved contents not established |

| 1 corrected | mkdir -p, cd, pwd, python3 --version, nano checker.py | Correct directory printed; Python 3.8.5; no visible errors | [Environment](../screenshots/01-environment.png) | Directory and interpreter checkpoint verified; saved code not yet shown |

## Deviations and correction

The initial attempt uses /home, the parent directory containing user homes. The guide now provides explicit commands to create and enter ~/portfolio-labs/password-checker, print the directory, and check Python's version before editing checker.py. No elevated privileges are needed. The user reported a Permission denied error when saving in /home; the error text is not visible in the initial screenshot. The corrected screenshot confirms use of the user's own home directory. The customized terminal prompt is a display choice; the actual home is /home/seed.

## Validation and remaining work

- [x] Record initial setup attempt
- [x] Step 1: verify project directory and Python version
- [ ] Step 2: save and inspect implementation
- [ ] Step 3: run controlled examples using invented strings
- [ ] Step 4: run automated checks
- [ ] Step 5: document heuristic limitations
- [ ] Step 6: publish actual executed code and finalize report

## Limitations and cleanup

The screenshot does not establish whether checker.py was saved or what it contains. No execution, test success, or password-security conclusion is claimed. No sensitive information requiring redaction was visible. The VM is retained for learning; no service or system change is shown in this attempt.
