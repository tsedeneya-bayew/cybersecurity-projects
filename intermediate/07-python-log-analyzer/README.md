# Python Security Log Analyzer

**Level:** Intermediate · **Suggested time:** 2–3 hours · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Project index](../../README.md)

## Objective

Complete a reproducible python security log analyzer exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Python 3; complete project 05 first. Uses a provided synthetic CSV, not production authentication logs.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Create a synthetic log

Create `auth.csv` with:
```csv
timestamp,user,source_ip,result
2026-01-01T10:00:00Z,lab_user,192.0.2.10,failure
2026-01-01T10:00:30Z,lab_user,192.0.2.10,failure
2026-01-01T10:01:00Z,lab_user,192.0.2.10,failure
2026-01-01T10:02:00Z,lab_user,192.0.2.10,success
2026-01-01T10:03:00Z,lab_other,192.0.2.20,success
```
These documentation-range IPs identify fictional sources. Save `auth.csv` and the script in the same folder.

**Screenshot checkpoint:** 01-dataset.png: five synthetic log rows.

### 2. Implement a basic analyzer

Create `analyzer.py`:
```python
import csv
import json
from collections import Counter

with open('auth.csv', newline='', encoding='utf-8') as f:
    rows = list(csv.DictReader(f))
required = {'timestamp', 'user', 'source_ip', 'result'}
if not rows or not required.issubset(rows[0]):
    raise ValueError('Missing rows or required columns')
unknown = [r for r in rows if r['result'] not in {'success', 'failure'}]
if unknown:
    raise ValueError('Unexpected result value')
failures = Counter(r['source_ip'] for r in rows if r['result'] == 'failure')
summary = {
    'total_events': len(rows),
    'failures_by_ip': dict(failures),
    'candidate_sources': [ip for ip, count in failures.items() if count >= 3]
}
print(json.dumps(summary, indent=2))
with open('summary.json', 'w', encoding='utf-8') as f:
    json.dump(summary, f, indent=2)
```
This threshold counts the entire file, not a rolling time window. It identifies candidates for investigation, not proven attackers.

**Screenshot checkpoint:** 02-code.png: parsing, validation, and counting logic.

### 3. Run and independently validate

Run `python3 analyzer.py` or `py analyzer.py`. Expected: total_events 5; three failures from 192.0.2.10; one candidate source. Count rows by hand and compare. Record how later success influences interpretation.

**Screenshot checkpoint:** 03-summary.png: actual JSON output.

### 4. Test boundary and malformed cases

Work on copies of `auth.csv`. Test two failures (no candidate), exactly three failures (one candidate), missing required header (error), and unexpected result value (error). Restore the original data after each test. Record pass/fail against these expected behaviors.

**Screenshot checkpoint:** 04-cases.png: boundary and invalid-input checks.

### 5. Improve temporal detection

Extend the script using `datetime.fromisoformat` after replacing Z with +00:00. Group by source, sort by time, and flag three failures within five minutes. Add a dataset with failures separated by hours; it should no longer trigger. Explain out-of-order events and timezone assumptions. Publish your own extended implementation in `src/`.

**Screenshot checkpoint:** 05-time-window.png: within-window alert and outside-window negative case.

### 6. Write an analyst report

Record the detection logic, event totals, candidate sources, false-positive possibilities, validation cases, and limitations. Preserve `summary.json` under `reports/` only if it contains the synthetic data. Suggest contextual fields that would make triage better.

**Screenshot checkpoint:** 06-report.png: synthetic detection summary.

## Troubleshooting

CSV filenames and headers must match exactly. Blank files deliberately fail validation. Do not silently skip malformed rows; count and report rejected events in any extension.

## Questions to answer in your report

- Why is a time window more useful than a whole-file count?
- What could cause legitimate repeated failures?
- How would malformed events affect detection coverage?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://docs.python.org/3/library/csv.html
- https://docs.python.org/3/library/datetime.html
