# Python Password Strength Checker — Findings Report

**Status:** In progress  
**Started:** October 9, 2026  
**Completed:** Not yet completed

## Progress summary

An initial setup attempt is documented. The terminal navigates out of the earlier traffic lab and opens nano from /home. The intended project folder, Python version, saved implementation, and test results have not yet been verified.

## Environment and scope

| Item | Observed value |
|---|---|
| Environment | Existing lab VM; exact OS version not shown in this screenshot |
| Prompt | tsedeneya-bayew@VM, customized display |
| Directory at editor launch | /home |
| Editor command | nano checker.py |
| Python version | Not yet recorded |
| Scope | Educational local checker using invented test strings only |

## Evidence log

| Step | Action | Actual result | Evidence | Interpretation |
|---|---|---|---|---|
| 1 initial attempt | Run cd .. three times, then nano checker.py | Terminal reaches /home; editor command returns to shell | [Setup attempt](../screenshots/01-setup-attempt.png) | Editor invocation documented; file creation and saved contents not established |

## Deviations and correction

The initial attempt uses /home, the parent directory containing user homes. The guide now provides explicit commands to create and enter ~/portfolio-labs/password-checker, print the directory, and check Python's version before editing checker.py. No elevated privileges are needed. No permission error appears in the supplied screenshot, so none is claimed.

## Validation and remaining work

- [x] Record initial setup attempt
- [ ] Step 1: verify project directory and Python version
- [ ] Step 2: save and inspect implementation
- [ ] Step 3: run controlled examples using invented strings
- [ ] Step 4: run automated checks
- [ ] Step 5: document heuristic limitations
- [ ] Step 6: publish actual executed code and finalize report

## Limitations and cleanup

The screenshot does not establish whether checker.py was saved or what it contains. No execution, test success, or password-security conclusion is claimed. No sensitive information requiring redaction was visible. The VM is retained for learning; no service or system change is shown in this attempt.
