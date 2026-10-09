# SIEM Deployment & Detection

**Level:** Advanced · **Suggested time:** 1–2 days · **Status:** Not started

> Prepared lab guide. No execution, screenshots, or findings are claimed yet.

[← Portfolio index](https://github.com/tsedeneya-bayew/cybersecurity-portfolio)

## Objective

Complete a reproducible siem deployment & detection exercise, validate the behavior, and explain the evidence and limitations in a professional report.

## Environment and prerequisites

Wazuh all-in-one server VM and a separate Ubuntu endpoint VM. Check current vendor requirements before allocating RAM/disk; the central server needs substantially more resources than beginner labs. Complete projects 07 and 08. Use only owned VMs and private lab networking.

## How to use this repository

1. Read the environment and scope before starting. Use a disposable VM for administrative changes.
2. Follow steps in order. Record actual output; expected results are predictions, not completed evidence.
3. Save screenshots using the exact filenames shown below in `screenshots/`.
4. Add each image under its step using `![Description](screenshots/filename.png)`. No images are included yet.
5. Complete [your findings report](reports/findings.md). Record deviations and failed checks honestly.
6. Change **Not started** to **In progress** when you begin. Mark **Completed** only after your evidence and report are committed.

For browser uploads: open the destination folder → Add file → Upload files → select your sanitized files → Commit changes. To edit Markdown: open the file → pencil icon → edit → preview → commit with a descriptive message. Do not upload passwords, tokens, personal identifiers, or unrelated logs.

## Step-by-step lab

### 1. Plan deployment and resources

Create a table of server IP, endpoint IP, CPU, RAM, disk, OS, and snapshot names. Review the current Wazuh Quickstart and supported OS list linked below. Choose a supported stable version; record it rather than assuming an older command is current. Permit internet access temporarily for official packages, then keep the endpoint-to-server path private.

**Screenshot checkpoint:** 01-topology.png: lab diagram and resource table.

### 2. Install central components

Follow the vendor Quickstart sections in order: verify system requirements; obtain the installation assistant from the official Wazuh source; review its instructions; run the documented all-in-one install command in the server VM. Record the exact command and version privately. Keep generated admin passwords and archives out of screenshots and GitHub. Use the vendor's certificate guidance; do not bypass a browser certificate warning as a lab step.

**Screenshot checkpoint:** 02-dashboard.png: authenticated dashboard with secrets hidden.

### 3. Enroll the endpoint

In the dashboard choose the agent deployment/enrollment interface. Select the endpoint OS and architecture, supply the private manager IP, and use the generated vendor commands on the endpoint. Start the agent as instructed. Verify the agent appears active; compare its displayed identity with your endpoint inventory.

**Screenshot checkpoint:** 03-agent.png: active owned endpoint and software version.

### 4. Configure synthetic event collection

On the endpoint:
```bash
sudo mkdir -p /var/log/portfolio-lab
printf 'PORTFOLIO_LOGIN_FAIL user=lab_user source=192.0.2.10\n' | sudo tee /var/log/portfolio-lab/auth.log
```
Back up `/var/ossec/etc/ossec.conf`. Add inside its existing `<ossec_config>` element:
```xml
<localfile>
  <log_format>syslog</log_format>
  <location>/var/log/portfolio-lab/auth.log</location>
</localfile>
```
Validate XML structure and restart the endpoint agent with `sudo systemctl restart wazuh-agent`. Do not replace the entire configuration.

**Screenshot checkpoint:** 04-collection.png: sanitized localfile configuration.

### 5. Create and test a custom rule

On the server, back up `/var/ossec/etc/rules/local_rules.xml`. Add this group alongside existing groups, with an unused custom rule ID:
```xml
<group name="portfolio_lab,">
  <rule id="100100" level="5">
    <match>PORTFOLIO_LOGIN_FAIL</match>
    <description>Portfolio synthetic login failure</description>
  </rule>
</group>
```
Run `sudo /var/ossec/bin/wazuh-logtest` and paste the synthetic log line; confirm the expected rule. Check current vendor custom-rule ID guidance and avoid collisions. Restart the manager only after validation. Append a fresh endpoint event:
```bash
printf 'PORTFOLIO_LOGIN_FAIL user=lab_user source=192.0.2.10\n' | sudo tee -a /var/log/portfolio-lab/auth.log
```
In the dashboard search for the rule ID within a suitable time range. This single-event detection does not establish brute-force behavior.

**Screenshot checkpoint:** 05-alert.png: matching synthetic alert and rule ID.

### 6. Measure and evaluate

Compare event generation and alert timestamps, considering clock sync and ingestion delay. Append a harmless line such as `PORTFOLIO_HEALTH_OK`; it should not trigger rule 100100. Record positive and negative outcomes, missed events, data retention, and synthetic-only limits. Restore backups or revert snapshots after capturing the report.

**Screenshot checkpoint:** 06-validation.png: positive/negative validation table and latency.

## Troubleshooting

An active agent does not prove your custom file is collected. Check exact path, file permissions, agent service logs, time range, manager rule load, and clock sync. Avoid repeated full reinstalls; isolate collection, parsing, and rule matching separately.

## Questions to answer in your report

- How do collection, parsing, and alerting differ?
- What does a negative test establish?
- How would a repeated-failure rule need a time window and grouping fields?

## Completion checklist

- [ ] Environment and exact scope recorded
- [ ] All lab steps attempted and actual outcomes documented
- [ ] Screenshots uploaded and linked under their steps
- [ ] Findings distinguish observation from interpretation
- [ ] Limitations and remediation explained
- [ ] Cleanup or restoration completed
- [ ] Findings report completed; status updated in this repository and portfolio index

## Official references

- https://documentation.wazuh.com/current/quickstart.html
- https://documentation.wazuh.com/current/user-manual/ruleset/rules/custom.html
- https://documentation.wazuh.com/current/user-manual/capabilities/log-data-collection/index.html
