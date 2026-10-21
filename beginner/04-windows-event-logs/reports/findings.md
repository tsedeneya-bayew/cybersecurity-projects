# Windows Security Event Logs — Findings Report

**Status:** In progress  
**Started:** October 20, 2026  
**Completed:** Not yet completed

## Summary

Initial baseline commands ran on the owner's Windows host. Logon auditing already shows Success and Failure. A repeated audit-policy backup failed because the destination file exists; its contents and suitability for restoration remain unverified. No new audit change is required based on the observed Logon setting.

## Environment and scope

| Item | Observed value |
|---|---|
| Execution environment | Owner's Windows host, by user choice |
| Recorded local time | October 20, 2026, 10:21:57 PM; timezone not printed |
| WindowsProductName | Windows 10 Home, as returned by Get-ComputerInfo |
| WindowsVersion | 2009, as returned |
| OsBuildNumber | 26200 |
| Lab directory | C:\PortfolioLab |
| Logon audit baseline | Success and Failure |
| Backup destination | C:\PortfolioLab\audit-before.csv; existing file not yet inspected |

The reported OS fields are recorded literally; they do not independently confirm the marketed Windows release. The screenshot does not show an elevation title, although the audit query succeeds.

## Evidence

[Step 1 baseline screenshot](../screenshots/01-baseline.png)

| Action | Actual result | Interpretation |
|---|---|---|
| Get-Date | Tuesday, October 20, 2026 10:21:57 PM | Local test date/time recorded |
| Get-ComputerInfo selection | Windows 10 Home / 2009 / 26200 | Reported OS fields recorded |
| New-Item directory command | PortfolioLab directory listed | Lab directory exists |
| auditpol backup | Failed to open file; error 0x00000050; The file exists. | Existing destination prevented this backup attempt; not proof of valid backup contents |
| auditpol Logon query | Success and Failure | Both audit outcomes already enabled |

An earlier success message is visible at the top without the originating command. It cannot alone establish that the existing backup is complete or predates any policy changes.

## Deviations and next verification

An earlier attempt used the Ubuntu terminal, where the Windows commands were unavailable. Execution now uses Windows PowerShell on the host rather than a disposable VM.

Preserve and inspect the existing audit-before.csv file before any policy modification. Since Logon success and failure auditing are already enabled, do not change that setting merely to repeat the guide. Avoid clearing Security logs or publishing unrelated host events. Cleanup should remove only the temporary lab account and reverse any actual lab changes.

## Progress

- [x] Record reported OS fields, local timestamp, and lab directory
- [x] Record current Logon auditing state
- [ ] Inspect existing baseline backup
- [ ] Create a standard local lab account
- [ ] Generate controlled login attempts and record times
- [ ] Inspect and query relevant Security events
- [ ] Document timeline and conclusions
- [ ] Clean up actual lab changes and finalize report

## Evidence limitations

No controlled login attempts, event IDs, test-account creation, backup readback, or restoration are shown yet. No sensitive data requiring redaction was visible in this screenshot.

## Additional backup inspection

The backup check shows `audit-before.csv` exists, has length `5955` bytes, and was last written October 20, 2026 at 10:21:27 PM. Reading its first five lines shows a CSV header and four audit-policy records. The computer name is blurred in the published privacy-edited copy. This verifies a nonempty, readable CSV excerpt, not full backup validity or successful restoration. The saved Logon row is not visible. Current Logon auditing was already Success and Failure; no additional change is required for that setting.

[Redacted backup check](../screenshots/01-backup-check-redacted.png)

## Step 2: local lab account

Settings → Accounts → Other Users shows `portfoliotest` as a Local account, with its password form closed. This differs from the guide's example name `portfolio_test`; use the actual name `portfoliotest` for subsequent commands and event filtering. The screenshot does not display the account type, so Standard User membership still needs verification. No password or unrelated personal information is visible.

[Account screenshot](../screenshots/02-account.png)

- [x] Local lab account creation documented
- [ ] Verify account type is Standard User
