# Active Directory Security Lab

**Level:** Advanced · **Suggested time:** 2–3 days · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Project index](../../README.md)

## Objective

Complete a reproducible active directory security lab exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Disposable Windows Server evaluation VM plus a Windows Pro/Enterprise client VM that supports domain join. Host-only network, legitimate evaluation media, local console access, and snapshots. Allow roughly 4 GB RAM per VM or more according to current vendor requirements; Windows Home cannot join an AD domain. Complete Windows log analysis first.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Build an isolated lab

Use vendor evaluation media and read licensing terms yourself. Create host-only adapters; avoid bridged connections and public DNS registration. Configure static lab addresses: DC 192.168.56.10/24 and client 192.168.56.20/24 only if this subnet does not overlap another network. Set client DNS to the DC. Rename server to LAB-DC and client to LAB-PC, reboot, and snapshot both.

**Screenshot checkpoint:** 01-topology.png: private addresses and VM roles.

### 2. Install a new forest

On the server use Server Manager → Add Roles and Features → Role-based installation → Active Directory Domain Services → Add Features → Install. Select Promote this server to a domain controller → Add a new forest → root domain `ad.example.test`. Set a unique Directory Services Restore Mode password without recording it publicly. Keep DNS selected; review prerequisites and install. A delegation warning for this isolated test domain is expected, but resolve errors before proceeding. Reboot and verify the domain.

**Screenshot checkpoint:** 02-forest.png: domain and domain-controller state, no credentials.

### 3. Create minimal identities and join client

Open Active Directory Users and Computers; create OUs `LabUsers`, `LabGroups`, and `LabComputers`. Create standard user `lab_analyst` with a unique lab-only password. Create global security group `LabReaders` and add the user. Join the client to `ad.example.test` using lab admin credentials through Windows settings, then reboot. Do not put the standard user in Domain Admins.

**Screenshot checkpoint:** 03-membership.png: standard user and LabReaders membership.

### 4. Audit policy and privileged membership

In an elevated server PowerShell:
```powershell
Get-ADDomain
Get-ADDefaultDomainPasswordPolicy
Get-ADGroupMember -Identity 'Domain Admins'
Get-ADGroupMember -Identity 'LabReaders'
```
Record actual policy and members, redacting identifiers where necessary. Describe administrative group risk without pretending that every member is improper. Domain password policy is not set by arbitrary OU links; document its domain-level location.

**Screenshot checkpoint:** 04-audit.png: sanitized policy and group inventory.

### 5. Validate group-based file access

On the server create a lab folder with a fictional text file. Share it using Advanced Sharing as `LabShare`. Grant LabReaders Read at both share permissions and NTFS Security permissions, while retaining Administrators and SYSTEM control. Use the Advanced Security view to inspect inherited access; do not remove entries from unrelated folders. On client, sign in as lab_analyst and read `\\LAB-DC\LabShare\sample.txt`. Try creating a file in the share; it should fail. Record both permission layers and effective behavior.

**Screenshot checkpoint:** 05-access.png: successful read and denied write.

### 6. Monitor and conclude

Use the logon audit procedure from project 04 on the relevant lab systems and generate at most two controlled failures for the standard account. Correlate relevant local logon events; domain authentication may additionally involve DC events depending on protocol. Report least privilege, DNS dependency, policy baseline, audit coverage, and permission validation. Revert snapshots or preserve the isolated lab offline.

**Screenshot checkpoint:** 06-summary.png: audit and access findings.

## Troubleshooting

Domain join commonly fails when client DNS points to the internet instead of the DC or clocks differ. Use `Resolve-DnsName LAB-DC.ad.example.test` and `w32tm /query /status`. Do not disable Windows Firewall or endpoint protection as a blanket fix; verify required services and scoped rules.

## Questions to answer in your report

- Why is DNS essential for domain discovery?
- How do share and NTFS permissions combine?
- How can group membership support least privilege?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/deploy/install-active-directory-domain-services--level-100-
- https://learn.microsoft.com/en-us/powershell/module/activedirectory/
