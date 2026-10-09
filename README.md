# Cybersecurity Projects

### 18 hands-on labs · Beginner → Intermediate → Advanced

A public learning portfolio with six projects at each level. Each project has a detailed guide, screenshot checkpoints, troubleshooting, and a findings report template.

> **Status:** Prepared lab guides. Actual screenshots and findings will be added as the labs are completed.

**[Start here: lab setup and screenshot workflow](START-HERE.md)**

| Category | Projects | Focus |
|---|---:|---|
| [Beginner](beginner/README.md) | 6 | Linux, file integrity, network traffic, Windows logs, Python, service discovery |
| [Intermediate](intermediate/README.md) | 6 | Log analysis, hardening, phishing, application security, assessment, forensics |
| [Advanced](advanced/README.md) | 6 | SIEM, incident response, Active Directory, CI/CD, AI security, OT/ICS |

## Beginner

| # | Project | Estimated time | Status |
|---|---|---|---|
| 01 | [Linux Users & File Permissions](beginner/01-linux-permissions/README.md) | 1–2 hours | Not started |
| 02 | [File Integrity with SHA-256](beginner/02-file-integrity/README.md) | 45–90 minutes | Not started |
| 03 | [Wireshark Traffic Analysis](beginner/03-wireshark-analysis/README.md) | 1–2 hours | Not started |
| 04 | [Windows Security Event Logs](beginner/04-windows-event-logs/README.md) | 1–2 hours | Not started |
| 05 | [Python Password Strength Checker](beginner/05-password-strength-checker/README.md) | 1–2 hours | Not started |
| 06 | [Local Network Discovery with Nmap](beginner/06-nmap-network-discovery/README.md) | 1–2 hours | Not started |

## Intermediate

| # | Project | Estimated time | Status |
|---|---|---|---|
| 07 | [Python Security Log Analyzer](intermediate/07-python-log-analyzer/README.md) | 2–3 hours | Not started |
| 08 | [Linux System Hardening](intermediate/08-linux-hardening/README.md) | 3–4 hours | Not started |
| 09 | [Phishing Email Investigation](intermediate/09-phishing-email-analysis/README.md) | 2–3 hours | Not started |
| 10 | [Local Web Vulnerability Lab](intermediate/10-web-vulnerability-lab/README.md) | 3–5 hours | Not started |
| 11 | [Network Vulnerability Assessment](intermediate/11-vulnerability-assessment/README.md) | 3–5 hours | Not started |
| 12 | [Digital Forensics Investigation](intermediate/12-digital-forensics/README.md) | 3–5 hours | Not started |

## Advanced

| # | Project | Estimated time | Status |
|---|---|---|---|
| 13 | [SIEM Deployment & Detection](advanced/13-siem-detection-lab/README.md) | 1–2 days | Not started |
| 14 | [Incident Response Investigation](advanced/14-incident-response/README.md) | 1–2 days | Not started |
| 15 | [Active Directory Security Lab](advanced/15-active-directory-security/README.md) | 2–3 days | Not started |
| 16 | [Secure CI/CD Pipeline](advanced/16-secure-cicd-pipeline/README.md) | 1–2 days | Not started |
| 17 | [AI Prompt Injection Evaluation](advanced/17-ai-security-evaluation/README.md) | 1–2 days | Not started |
| 18 | [OT/ICS Threat Modeling](advanced/18-ot-ics-threat-model/README.md) | 1–2 days | Not started |

## Inside each project

- `README.md`: objective, prerequisites, numbered steps, expected behavior, explanations, screenshot checkpoints, troubleshooting, and references
- `screenshots/`: upload your sanitized screenshots using the specified filenames
- `reports/findings.md`: document your real observations, validation, limitations, cleanup, and lessons learned
- `src/`: add code you executed or adapted when the project involves programming

## Working through the collection

Begin with project 01, then proceed through 01–06. Read each project's prerequisites before moving to higher levels. Advanced SIEM and Active Directory exercises need additional VM resources; the OT/ICS exercise is a theoretical tabletop.

Follow the upload workflow in START-HERE.md. Add screenshots under the matching guide step with a relative link such as `![Lab evidence](screenshots/01-environment.png)`. Edit the matching project report and update status here and in its README.

| Status | Meaning |
|---|---|
| Not started | Guide prepared; execution not documented |
| In progress | Lab underway; actual evidence being collected |
| Completed | Screenshots, findings, validation, and cleanup included |

## Scope and evidence

Use owned or explicitly authorized disposable lab systems and synthetic data. Keep expected output separate from actual observations. Review screenshots and artifacts before publishing; exclude passwords, tokens, private account information, unrelated logs, and personal traffic.

## Collection history

This repository consolidates all 18 guides into one collection. Earlier standalone project repositories point to the current folders. The profile README links here. No further repository creation is needed for these projects.
