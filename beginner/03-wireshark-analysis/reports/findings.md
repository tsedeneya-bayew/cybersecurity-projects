# Wireshark Traffic Analysis — Findings Report

**Status:** In progress  
**Started:** October 9, 2026 (first screenshot submitted)  
**Completed:** Not yet completed

> Partial setup report. Step 1 interface verification and Steps 2–6 remain pending.

## Progress summary

The supplied screenshot documents dnsutils installation and synthetic page preparation. It does not show Wireshark interfaces, packet capture, or protocol analysis.

## Environment and authorized scope

| Item | Observed value |
|---|---|
| Environment | User's Ubuntu lab VM; exact OS release not shown in this screenshot |
| Package | dnsutils 1:9.18.30-0ubuntu0.20.04.2 |
| Prompt | tsedeneya-bayew@VM, customized display |
| Working directory | ~/portfolio-labs/traffic specified by the commands |
| Synthetic artifact | index.html, created using printf with Synthetic HTTP lab page and a newline |
| Scope | Planned local loopback HTTP and example.com DNS lab; no capture shown yet |
| Other tool versions | Not captured |

## Evidence log

| Step | Action | Actual result | Evidence | Interpretation |
|---|---|---|---|---|
| 1 setup | Package installation output | dnsutils unpacking and Setting up messages; prompt returns | [Setup screenshot](../screenshots/01-setup.png) | dnsutils setup shown completing without a visible error. |
| 1 setup | mkdir -p and cd to traffic directory | No visible errors; prompt ends in traffic | [Setup screenshot](../screenshots/01-setup.png) | Lab directory preparation recorded. |
| 1 setup | printf redirected to index.html | No visible error | [Setup screenshot](../screenshots/01-setup.png) | Synthetic page creation command recorded; contents not separately read back. |

![Step 1 setup evidence](../screenshots/01-setup.png)

## Observations and limitations

Only dnsutils setup is visible in the installation excerpt; installation or availability of Wireshark, curl, and Python 3 is not established by this screenshot. The package summary lists 570 packages not upgraded, which alone does not establish vulnerability or patch status. No Wireshark interfaces, capture privileges, running HTTP server, packets, or DNS responses have been verified. Exact exit codes are not shown.

## Deviations and troubleshooting

No visible error. The screenshot documents preparation rather than the guide's requested interface checkpoint; capture an additional screenshot of Wireshark's available interfaces to finish Step 1.

## Validation and remaining work

- [x] Document dnsutils setup and synthetic page creation commands
- [ ] Step 1: verify Wireshark interfaces and capture access
- [ ] Step 2: start loopback HTTP server
- [ ] Step 3: capture and inspect HTTP
- [ ] Step 4: reconstruct TCP conversation
- [ ] Step 5: inspect DNS query and response
- [ ] Step 6: summarize results and stop server

## Cleanup / restoration

No server or capture is shown running. No cleanup has been reported. The user retains the lab VM for future projects.
