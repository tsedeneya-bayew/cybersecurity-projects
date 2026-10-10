# Wireshark Traffic Analysis — Findings Report

**Status:** In progress  
**Started:** October 9, 2026 (first screenshot submitted)  
**Completed:** Not yet completed

> Partial setup report. Steps 1–2 are documented; Steps 3–6 remain pending.

## Progress summary

The supplied screenshot documents dnsutils installation and synthetic page preparation. An additional screenshot shows Wireshark 3.2.3 and available interfaces, including loopback. Step 2 shows the Python HTTP server running on 127.0.0.1:8000. Packet capture and protocol analysis remain pending.

## Environment and authorized scope

| Item | Observed value |
|---|---|
| Environment | User's Ubuntu lab VM; exact OS release not shown in this screenshot |
| Package | dnsutils 1:9.18.30-0ubuntu0.20.04.2 |
| Prompt | tsedeneya-bayew@VM, customized display |
| Working directory | ~/portfolio-labs/traffic specified by the commands |
| Synthetic artifact | index.html, created using printf with Synthetic HTTP lab page and a newline |
| Scope | Planned local loopback HTTP and example.com DNS lab; no capture shown yet |
| Wireshark | 3.2.3, Git v3.2.3 packaged as 3.2.3-1 |
| Interfaces shown | enp0s3, Loopback: lo, any, docker0, bluetooth-monitor, nflog, nfqueue; remote capture entries also visible |
| Other tool versions | Not captured |

## Evidence log

| Step | Action | Actual result | Evidence | Interpretation |
|---|---|---|---|---|
| 1 setup | Package installation output | dnsutils unpacking and Setting up messages; prompt returns | [Setup screenshot](../screenshots/01-setup.png) | dnsutils setup shown completing without a visible error. |
| 1 setup | mkdir -p and cd to traffic directory | No visible errors; prompt ends in traffic | [Setup screenshot](../screenshots/01-setup.png) | Lab directory preparation recorded. |
| 1 setup | printf redirected to index.html | No visible error | [Setup screenshot](../screenshots/01-setup.png) | Synthetic page creation command recorded; contents not separately read back. |
| 1 interfaces | Open Wireshark and inspect welcome screen | Version 3.2.3; enp0s3 and Loopback: lo listed; No Packets | [Interface screenshot](../screenshots/01-interfaces.png) | Interface availability verified; capture success remains untested. |
| 2 | cd to traffic directory; python3 -m http.server 8000 --bind 127.0.0.1 | Serving HTTP on 127.0.0.1 port 8000 | [Server screenshot](../screenshots/02-server.png) | Server startup on loopback recorded; requests not yet shown. |

![Step 1 setup evidence](../screenshots/01-setup.png)

![Step 1 Wireshark interface evidence](../screenshots/01-interfaces.png)

![Step 2 loopback HTTP server evidence](../screenshots/02-server.png)

## Observations and limitations

Only dnsutils setup is visible in the installation excerpt; the interface screenshot separately establishes Wireshark availability. Python 3 successfully starts its HTTP server in Step 2; its exact version and curl availability are not shown. The package summary lists 570 packages not upgraded, which alone does not establish vulnerability or patch status. Wireshark interfaces are visible, including loopback. The Python server reports startup on loopback port 8000. Capture privileges, successful HTTP requests, packets, and DNS responses have not been verified. Interface visibility alone does not prove a successful capture. Exact exit codes are not shown.

## Deviations and troubleshooting

No visible error. The first screenshot documented preparation; the additional interface screenshot now satisfies the interface checkpoint. enp0s3 is highlighted, but the local HTTP exercise requires Loopback: lo.

## Validation and remaining work

- [x] Document dnsutils setup and synthetic page creation commands
- [x] Step 1: prepare the lab and inspect Wireshark interfaces
- [ ] Verify successful capture access when beginning Step 3
- [x] Step 2: start loopback HTTP server
- [ ] Step 3: capture and inspect HTTP
- [ ] Step 4: reconstruct TCP conversation
- [ ] Step 5: inspect DNS query and response
- [ ] Step 6: summarize results and stop server

## Cleanup / restoration

The Python server is running in the Step 2 screenshot; no capture is shown. Server shutdown remains pending for Step 6. The user retains the lab VM for future projects.
