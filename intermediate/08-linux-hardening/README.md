# Linux System Hardening

**Level:** Intermediate · **Suggested time:** 3–4 hours · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Project index](../../README.md)

## Objective

Complete a reproducible linux system hardening exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Disposable Ubuntu VM with console access and snapshot. Complete project 01 and 06 first. Do not apply these changes to your main computer or a remote production server.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Inventory the baseline

```bash
cat /etc/os-release
sudo ss -lntup
sudo ufw status verbose
systemctl --type=service --state=running
```
Save output locally and note which services are actually needed. Take a VM snapshot and verify you can open the hypervisor console.

**Screenshot checkpoint:** 01-baseline.png: OS, listening ports, and firewall state.

### 2. Update the VM

```bash
sudo apt update
sudo apt upgrade
```
Read prompts before accepting package changes. Reboot if required, then record updated package state. A package version alone does not establish whether a vendor backported a fix.

**Screenshot checkpoint:** 02-updates.png: completed update summary.

### 3. Prepare and validate SSH

If SSH is needed for this lab:
```bash
sudo apt install openssh-server
sudo cp -a /etc/ssh/sshd_config /etc/ssh/sshd_config.portfolio-backup
sudo mkdir -p /etc/ssh/sshd_config.d
printf 'PermitRootLogin no\n' | sudo tee /etc/ssh/sshd_config.d/00-portfolio-lab.conf
sudo sshd -t
sudo sshd -T | grep permitrootlogin
sudo systemctl reload ssh
```
Expected effective setting: permitrootlogin no. If `sshd -t` fails, fix the configuration before reloading. Keep password authentication unchanged until a separate key-based login is verified. Distribution snippets may take precedence; inspect effective output rather than assuming file content wins.

**Screenshot checkpoint:** 03-ssh-policy.png: syntax validation and effective setting.

### 4. Enable a minimal firewall

Do this from the VM console, not a remote-only session:
```bash
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow OpenSSH
sudo ufw enable
sudo ufw status verbose
```
Allow OpenSSH only when SSH is part of the lab. Record the rule and why it is needed. Do not expose the VM using router port forwarding.

**Screenshot checkpoint:** 04-firewall.png: active firewall and minimal allow rule.

### 5. Validate allowed and denied connections

On a host-only adapter, identify the VM address with `ip -br addr`. From a second owned host-only VM test the single target with `nmap -sT -p 22,8000 LAB_VM_IP` (replace LAB_VM_IP with the recorded address). For a meaningful deny test, start a temporary HTTP service bound to that lab IP on port 8000. Verify port 22 if enabled; verify 8000 is blocked externally while listening locally. Stop HTTP with Ctrl+C.

**Screenshot checkpoint:** 05-validation.png: listener, firewall result, and allowed/blocked outcomes.

### 6. Record improvements and restore

Compare baseline and final attack surface. Explain service necessity and residual risk. Record effective SSH setting, firewall rules, and test results. Revert the snapshot after reporting if you do not want the changes retained. Do not disable protection on your host computer to make a lab connection work.

**Screenshot checkpoint:** 06-comparison.png: before/after hardening table.

## Troubleshooting

If locked out, use the VM console or snapshot. A closed port may mean no listener; it is not proof the firewall blocked traffic. Validate service state, exact IP, and host-only network configuration.

## Questions to answer in your report

- Why validate effective SSH configuration?
- How does least privilege apply to listening services?
- What evidence proves a firewall block rather than an absent service?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://ubuntu.com/server/docs/how-to/security/openssh-server/
- https://documentation.ubuntu.com/server/how-to/security/firewalls/index.html
