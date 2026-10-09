# Incident Response Investigation

**Level:** Advanced · **Suggested time:** 1–2 days · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Project index](../../README.md)

## Objective

Complete a reproducible incident response investigation exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Python 3 or a text editor; synthetic CSV evidence. Complete log analysis and forensics projects first. This is a tabletop investigation, with optional correlation in the SIEM.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Generate the incident fixture

Save `incident.csv`:
```csv
time_utc,host,user,event,source,detail
2026-01-01T10:00:00Z,LAB-PC,lab_user,login_failure,192.0.2.10,invalid password
2026-01-01T10:00:30Z,LAB-PC,lab_user,login_failure,192.0.2.10,invalid password
2026-01-01T10:01:00Z,LAB-PC,lab_user,login_success,192.0.2.10,interactive session
2026-01-01T10:03:00Z,LAB-PC,lab_user,process_start,local,report-export-tool
2026-01-01T10:04:00Z,LAB-PC,lab_user,file_create,local,synthetic-export.zip
2026-01-01T10:05:00Z,LAB-PC,lab_user,network_connection,198.51.100.20,443
```
All data is fictional. Hash the file and create a working copy. A process name and a connection do not establish maliciousness.

**Screenshot checkpoint:** 01-evidence.png: fixture and digest.

### 2. Define the incident question

Question: did an unauthorized login lead to export and transfer of data? List known facts, missing evidence, and initial competing hypotheses: legitimate user export; compromised account; inaccurate or incomplete telemetry. Record scope as one fictional endpoint and account.

**Screenshot checkpoint:** 02-hypotheses.png: question, scope, and competing explanations.

### 3. Build a correlated timeline

Sort UTC events and record delta between login, process, file, and connection. Use Python's csv and datetime libraries from project 07 or a manual table. Add a source reference for every event. Identify correlation without claiming causation. Optional SIEM import must be labeled synthetic.

**Screenshot checkpoint:** 03-timeline.png: sourced timeline.

### 4. Develop and test hypotheses

Create an evidence matrix with each hypothesis, supporting facts, conflicting facts, and missing evidence. Request (as a tabletop list only) authenticated user confirmation, process hashes and signature, command line, destination ownership, proxy logs, and file contents. With the provided fixture alone, label unauthorized access and exfiltration as unconfirmed.

**Screenshot checkpoint:** 04-matrix.png: confidence and evidence gaps.

### 5. Design proportionate response

Draft a tabletop response plan: preserve evidence; triage identity and endpoint; isolate if validated risk warrants it; coordinate account/session revocation; identify affected data; restore from known-good state; monitor recovery. State who approves each action and how to preserve business continuity. Do not actually isolate real devices, revoke accounts, or contact people.

**Screenshot checkpoint:** 05-response-plan.png: action, owner role, trigger, validation.

### 6. Produce a final incident report

Include executive summary, timeline, scope, observations, hypotheses, confidence, missing evidence, proposed containment, recovery criteria, and lessons learned. Hash the original again and verify it is unchanged. Clearly conclude what is known and unknown rather than inventing a confirmed breach.

**Screenshot checkpoint:** 06-report.png: final disposition and evidence limitations.

## Troubleshooting

Timezone mixing distorts timelines. Use UTC throughout this fixture. If a fact has no evidence reference, remove it or label it a hypothesis. Logs without byte counts or payload evidence cannot prove data exfiltration.

## Questions to answer in your report

- Which events support correlation but not causation?
- What evidence would confirm unauthorized access?
- How do containment priorities change with confidence and business impact?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://www.nist.gov/publications/incident-response-recommendations-and-considerations-cybersecurity-risk-management-csf
- https://docs.python.org/3/library/datetime.html
