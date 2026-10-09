# Phishing Email Investigation

**Level:** Intermediate · **Suggested time:** 2–3 hours · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Project index](../../README.md)

## Objective

Complete a reproducible phishing email investigation exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Python 3 and a text editor. Uses a handcrafted email with reserved .invalid domains. No live malicious links, attachments, or external uploads.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Create the evidence file

Save as `sample.eml` with a blank line between headers and body:
```text
From: Help Desk <support@helpdesk.invalid>
To: Lab User <user@example.invalid>
Reply-To: verify@account-check.invalid
Date: Thu, 1 Jan 2026 10:00:00 +0000
Subject: Urgent: account expires today
Message-ID: <lab-001@helpdesk.invalid>
Authentication-Results: lab-gateway.invalid; spf=fail; dkim=none; dmarc=fail
MIME-Version: 1.0
Content-Type: text/plain; charset=utf-8

Your account expires in 30 minutes.
Verify your password at https://account-check.invalid/login
```
Everything here is synthetic. Header authentication results are fictional, not a live gateway verdict.

**Screenshot checkpoint:** 01-email.png: synthetic message with full headers.

### 2. Preserve and fingerprint

On Ubuntu run `sha256sum sample.eml`; on Windows use `Get-FileHash .\sample.eml -Algorithm SHA256`. Record digest, filename, acquisition method (created synthetic fixture), and analysis time. Work on a copy.

**Screenshot checkpoint:** 02-hash.png: evidence digest.

### 3. Parse the headers safely

Create `inspect_email.py`:
```python
from email import policy
from email.parser import BytesParser

with open('sample.eml', 'rb') as f:
    msg = BytesParser(policy=policy.default).parse(f)
for key in ['From', 'To', 'Reply-To', 'Date', 'Subject',
            'Message-ID', 'Authentication-Results']:
    print(f'{key}: {msg.get(key, "MISSING")}')
print('Multipart:', msg.is_multipart())
```
Run it with Python. This parses text only; it does not open URLs or execute attachments.

**Screenshot checkpoint:** 03-parser.png: extracted headers.

### 4. Evaluate indicators

Compare From and Reply-To domains, urgency, credential request, and URL destination. Record URLs in defanged form such as `hxxps://account-check[.]invalid/login` in your report. Treat a domain mismatch as a signal requiring context, not standalone proof. In real cases, Authentication-Results is only trustworthy when inserted by a trusted mail gateway; attackers can add fake headers.

**Screenshot checkpoint:** 04-indicators.png: indicator table and confidence.

### 5. Assign a scoped verdict

Write a verdict: this constructed sample is suspicious because it combines urgency, a password request, and differing identities. Separate observed fields from your interpretation. Explain why this fixture cannot validate SPF/DKIM/DMARC configuration or sender attribution.

**Screenshot checkpoint:** 05-verdict.png: verdict, evidence, and limitations.

### 6. Recommend response

Draft internal containment guidance in the report: do not interact, preserve original, report through the approved channel, assess recipients and credential exposure, and coordinate blocking only after verification. Do not send messages or perform real blocking from this lab. Publish only the synthetic fixture and your report.

**Screenshot checkpoint:** 06-response.png: response recommendations.

## Troubleshooting

Missing fields often mean the blank header/body separator is wrong. Do not trust a copied or rendered email to preserve original transport headers. If converting files between systems, rehash the original bytes.

## Questions to answer in your report

- Why is Reply-To mismatch only one indicator?
- When can Authentication-Results be trusted?
- What additional evidence would be needed for a real incident?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://docs.python.org/3/library/email.parser.html
- https://www.rfc-editor.org/rfc/rfc8601
