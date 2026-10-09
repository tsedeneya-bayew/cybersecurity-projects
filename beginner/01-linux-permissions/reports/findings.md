# Linux Users & File Permissions — Findings Report

**Status:** In progress  
**Started:** October 9, 2026 (first screenshot submitted)  
**Completed:** Not yet completed

> Partial report based on the supplied Step 1 screenshot. Steps 2–6 have not yet been documented.

## Progress summary

The initial environment inspection is complete. The screenshot records the Ubuntu release, actual current account, UID/GID, and group membership. No lab users, file-permission changes, or access-control outcomes are established by this evidence.

## Environment and authorized scope

| Item | Observed value |
|---|---|
| OS and version | Ubuntu 20.04.1 LTS (Focal Fossa); VERSION_ID 20.04 |
| Actual account | seed |
| UID / primary GID | 1000 / 1000 (seed) |
| Supplementary groups shown | adm (4), cdrom (24), sudo (27), dip (30), plugdev (46), lpadmin (120), lxd (131), sambashare (132), docker (136) |
| Terminal prompt | tsedeneya-bayew@VM; customized display |
| Scope | User-provided Ubuntu VM for this lab |
| Tools used so far | cat, whoami, id; versions not captured |
| Snapshot / recovery plan | Not yet documented |
| VM CPU / RAM / disk | Not shown in screenshot |
| Timezone | Not shown in screenshot |

## Evidence log

| Step | Command | Actual result | Evidence | Interpretation |
|---|---|---|---|---|
| 1 | `cat /etc/os-release` | Ubuntu 20.04.1 LTS; codename focal | [Environment screenshot](../screenshots/01-environment.png) | Establishes the release label reported by the VM. |
| 1 | `whoami` | seed | [Environment screenshot](../screenshots/01-environment.png) | The effective account name is seed, despite the customized prompt. |
| 1 | `id` | UID/GID 1000; groups listed above | [Environment screenshot](../screenshots/01-environment.png) | Records current identity and group membership. |

![Step 1 environment evidence](../screenshots/01-environment.png)

## Observations

### OS baseline recorded

The release information identifies Ubuntu 20.04.1 LTS (Focal Fossa). This screenshot does not establish installed package versions, patch status, support entitlement, or whether updates have been applied.

### Account identity differs from the displayed prompt

The prompt displays `tsedeneya-bayew@VM`, while `whoami` returns `seed`. The prompt was customized for the portfolio; it is not evidence of an account rename. Account-related commands should use the actual account where required.

### Group membership recorded

The account belongs to `sudo`, along with the other groups in the environment table. Membership is observed, but no `sudo` command or effective privilege test is shown. No claim of successfully exercised administrative access is made at this stage.

## Deviations and troubleshooting

The terminal prompt is customized; the report retains the actual account identity from command output. No errors are visible in the three environment commands. Exit codes were not captured.

## Validation

- **Documented:** OS release inspection, current account inspection, UID/GID and group inspection.
- **Pending:** Create lab users/group, apply directory and file permissions, verify allowed and denied access, demonstrate controlled group-write, and restore settings.

## Limitations

Only Step 1 is evidenced. The screenshot does not prove hypervisor configuration, snapshot creation, resource allocation, successful privilege elevation, file permissions, or access-control behavior. No vulnerability or remediation finding is claimed.

## Cleanup / restoration

No cleanup is documented yet. The shown commands inspect the environment; no lab-specific permission change is visible.

## Lessons learned so far

- The visible shell prompt can differ from the actual account name.
- `whoami` and `id` provide account identity and group evidence.
- OS identification is a baseline, not proof of patch status.

## Remaining work

- [x] Step 1: record environment
- [ ] Step 2: create two lab users and one group
- [ ] Step 3: create least-privilege directory and file
- [ ] Step 4: verify allowed and denied access
- [ ] Step 5: demonstrate a controlled permission change
- [ ] Step 6: complete findings and document restoration
