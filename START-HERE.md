# Start Here

## Your first project

Begin with [01 — Linux Users & File Permissions](beginner/01-linux-permissions/README.md). Do not try to complete all 18 projects at once. Finish the guide, collect evidence, and explain what you learned before moving on.

## Prepare an Ubuntu VM on your Windows computer

1. Check Windows Task Manager → Performance → CPU for virtualization support and available RAM. Keep enough RAM for your host; if resources are limited, begin with the small Python projects instead.
2. Use a reputable hypervisor obtained from its official source, such as VirtualBox. Download an Ubuntu LTS Desktop ISO from the official Ubuntu website.
3. Create a new Linux/Ubuntu VM. Suggested beginner allocation: 2 CPU cores, 4 GB RAM, and a 30 GB dynamically allocated disk, if your host can support it.
4. Attach the ISO as the virtual optical disk and install Ubuntu inside the VM. The installer's erase-disk choice must apply to the VM's virtual disk only; do not perform these instructions on your host computer.
5. Create a VM-only administrator account and unique password. Do not reuse a personal account password.
6. Start with NAT networking for official package updates. Do not choose bridged networking for the security labs. Two-VM exercises use a separate host-only or internal network, with no router port forwarding.
7. Update the guest with `sudo apt update` and `sudo apt upgrade`. Reboot if requested.
8. Shut down the guest and take a hypervisor snapshot named `clean-ubuntu-baseline`. Confirm the VM can boot before beginning.
9. Open the project README and read its environment and scope. Project-specific requirements take precedence over these general allocations.

Official starting points:

- https://ubuntu.com/download/desktop
- https://ubuntu.com/tutorials/install-ubuntu-desktop
- https://www.virtualbox.org/wiki/Downloads

## Evidence workflow

1. Read a step, run its commands, and compare actual output with expected behavior.
2. Capture the terminal and relevant output together. Use Windows Snipping Tool or the VM's screenshot utility.
3. Save using the exact filename in the guide, such as `01-environment.png`.
4. Apply opaque redaction to identifying or sensitive details. Inspect the exported image.
5. Open the project `screenshots/` folder on GitHub. Choose Add file → Upload files → select your image → Commit changes.
6. Edit the project README using the pencil icon. Add the image under its step:

```markdown
![Ubuntu environment and lab user](screenshots/01-environment.png)
```

7. Preview before committing. Record deviations under the relevant step.
8. Fill `reports/findings.md` with your real results; do not copy expected results as if they were observed.
9. Change the project status to In progress when you begin, and Completed only after evidence, findings, validation, and cleanup are recorded. Update the portfolio roadmap too.

For every project, open its category folder (`beginner/`, `intermediate/`, or `advanced/`), then its numbered project folder. Upload evidence inside that project's `screenshots/` folder and write results in its `reports/findings.md` file.

## Useful commit messages

- `Add Linux environment screenshot`
- `Document permission-denied validation`
- `Add findings and cleanup notes`
- `Complete Linux permissions lab`

## Writing your final report

State what you did, what you observed, why it matters, and what remains uncertain. Use first person only for steps you actually performed. Include tool versions, exact scope, evidence references, failed checks, and limitations. Report templates are intentionally unfinished until you add your work.

## Advanced lab resources

SIEM and Active Directory labs require additional VMs, RAM, and disk. Check current vendor requirements before installing. The OT/ICS project is a tabletop model; it requires no real industrial hardware. The AI project uses synthetic inputs and requires no publication of API keys. The CI/CD guide contains inactive example configuration until you review and add it yourself.
