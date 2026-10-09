# Secure CI/CD Pipeline

**Level:** Advanced · **Suggested time:** 1–2 days · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Portfolio index](https://github.com/tsedeneya-bayew/cybersecurity-portfolio)

## Objective

Complete a reproducible secure ci/cd pipeline exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

A public lab repository, GitHub Actions hosted runner, Git, Python 3 locally. Complete the Python and secure-query projects. This guide prepares CI only; no deployment credentials or live release.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Create a small tested module

In your project folder create `src/__init__.py` (empty), `src/calculator.py`:
```python
def add(a, b):
    if not isinstance(a, (int, float)) or not isinstance(b, (int, float)):
        raise TypeError('numbers required')
    return a + b
```
Create `tests/test_calculator.py`:
```python
import unittest
from src.calculator import add

class Tests(unittest.TestCase):
    def test_add(self):
        self.assertEqual(add(2, 3), 5)
    def test_bad_input(self):
        with self.assertRaises(TypeError):
            add('2', 3)
```
Run `python3 -m unittest discover -s tests -v`. Record actual local results.

**Screenshot checkpoint:** 01-tests.png: successful local tests.

### 2. Review workflow trust boundaries

Define CI goals: tests and static analysis on push and pull request, read-only repository token, no secrets, hosted runner. Do not use `pull_request_target` to execute untrusted contributor code. Review official Actions documentation. Resolve full commit SHAs for official checkout and setup-python actions from their trusted repositories and record those SHAs for reproducible pinning.

**Screenshot checkpoint:** 02-design.png: workflow threat model and selected pinned SHAs.

### 3. Create the workflow

Save this as `.github/workflows/security.yml` AFTER replacing both placeholder SHAs with verified full commit SHAs:
```yaml
name: security-checks
on: [push, pull_request]
permissions:
  contents: read
jobs:
  checks:
    runs-on: ubuntu-latest
    timeout-minutes: 10
    steps:
      - uses: actions/checkout@REPLACE_WITH_VERIFIED_FULL_SHA
        with:
          persist-credentials: false
      - uses: actions/setup-python@REPLACE_WITH_VERIFIED_FULL_SHA
        with:
          python-version: '3.12'
      - run: python -m pip install bandit
      - run: python -m unittest discover -s tests -v
      - run: bandit -r src
```
Do not commit active YAML with placeholders. This baseline installs the current Bandit package; an extension pins audit tool dependencies after checking releases. No workflow is installed by the prepared guide itself.

**Screenshot checkpoint:** 03-workflow.png: completed configuration without placeholders.

### 4. Validate clean and broken builds

Commit your reviewed workflow and sample code to the lab repository. Open Actions and inspect the job. Record commit SHA and actual status. On a lab branch deliberately change the expected result to 6; the test job should fail. Fix it and confirm a passing run. Do not disable the test to obtain green status.

**Screenshot checkpoint:** 04-builds.png: failing then passing test check.

### 5. Validate a security finding

On a disposable lab branch add an unused function with `subprocess.run(command, shell=True)` to src. Do not call it or supply untrusted input. Bandit should report the unsafe shell invocation; record its exact finding and severity. Replace the example with a fixed list of arguments and `shell=False`, then rerun. Static analysis does not prove all vulnerabilities are absent.

**Screenshot checkpoint:** 05-static-analysis.png: finding and corrected passing run.

### 6. Report and extend

Include trust boundaries, action pins, token permissions, checks, deliberate failures, remediation, and limitations. If adding application dependencies, use pip-audit on a pinned application requirements file and distinguish application from tooling dependencies. Add dependency review and branch protection only after understanding permissions; do not publish secrets or add deployment steps.

**Screenshot checkpoint:** 06-report.png: pipeline validation matrix.

## Troubleshooting

If no workflow runs, verify the exact `.github/workflows/` path, valid YAML, event trigger, and Actions availability. Pin SHAs must be real official action commits. An invalid placeholder is a configuration error, not a security-test success.

## Questions to answer in your report

- Why pin actions to full commits?
- Why is untrusted PR code dangerous with write tokens or secrets?
- What can tests and static analysis each miss?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://docs.github.com/en/actions/reference/security/secure-use
- https://github.com/actions/setup-python
- https://bandit.readthedocs.io/
