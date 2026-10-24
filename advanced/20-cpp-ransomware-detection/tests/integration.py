"""End-to-end tests using disposable directories and real monitor subprocesses."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import time

monitor, simulator = map(lambda s: str(Path(s).resolve()), sys.argv[1:3])

def run(*args, ok=True):
    result = subprocess.run(args, text=True, capture_output=True, timeout=15)
    if (result.returncode == 0) != ok:
        raise RuntimeError(f"Unexpected exit: {args}: {result.stdout} {result.stderr}")
    return result

def scenario(parent, mode):
    root = parent / 'ransomware-lab-sandbox'
    report = parent / 'reports'
    run(simulator, 'init', str(root))
    with tempfile.TemporaryFile(mode='w+') as output:
        proc = subprocess.Popen([monitor, str(root), str(report), '3', '50'],
                                stdout=output, stderr=subprocess.PIPE, text=True)
        try:
            ready = False
            for _ in range(100):
                output.seek(0)
                if 'BASELINE READY' in output.read():
                    ready = True
                    break
                if proc.poll() is not None:
                    break
                time.sleep(.02)
            if not ready:
                raise RuntimeError('Baseline not ready')
            run(simulator, mode, str(root))
            _, err = proc.communicate(timeout=10)
            if proc.returncode:
                raise RuntimeError(err)
        finally:
            if proc.poll() is None:
                proc.kill()
                proc.communicate()
    events = [json.loads(x) for x in (report / 'events.jsonl').read_text().splitlines()]
    incidents = [json.loads(x) for x in (report / 'incidents.jsonl').read_text().splitlines()]
    summary = json.loads((report / 'summary.json').read_text())
    assert summary['events'] == len(events)
    assert summary['incidents'] == len(incidents)
    assert summary['status'] == 'complete'
    if mode == 'normal':
        assert len(events) == 1 and not incidents, (events, incidents)
    else:
        assert len(events) >= 61, len(events)
        signals = {s for i in incidents for s in i['signals']}
        assert 'canary_changed' in signals
        assert 'many_distinct_files_changed' in signals
        assert 'demo_extension_delete_create_pairs' in signals
        assert any(i['score'] >= 4 for i in incidents)
    # Existing evidence directories and nested output paths are refused.
    run(monitor, str(root), str(report), '1', '50', ok=False)
    run(monitor, str(root), str(root / 'nested'), '1', '50', ok=False)
    print(f'PASS: {mode}: events={len(events)}, incidents={len(incidents)}')

with tempfile.TemporaryDirectory(prefix='cpp-detection-tests-') as temp:
    parent = Path(temp)
    for mode in ('normal', 'burst'):
        folder = parent / mode
        folder.mkdir()
        scenario(folder, mode)
    safety = parent / 'safety'
    safety.mkdir()
    root = safety / 'ransomware-lab-sandbox'
    run(simulator, 'init', str(root))
    run(simulator, 'init', str(root), ok=False)
    run(simulator, 'burst', str(parent), ok=False)
    (root / 'unexpected.txt').write_text('must survive')
    run(simulator, 'burst', str(root), ok=False)
    assert (root / 'unexpected.txt').read_text() == 'must survive'
    (root / 'unexpected.txt').unlink()
    # Verify write confinement by refusing a hard-linked sample.
    outside = safety / 'outside.txt'
    outside.write_text('must survive')
    (root / 'sample_0.txt').unlink()
    os.link(outside, root / 'sample_0.txt')
    run(simulator, 'normal', str(root), ok=False)
    assert outside.read_text() == 'must survive'
    (root / 'sample_0.txt').unlink()
    (root / 'sample_0.txt').write_text('synthetic')
    # File symlinks are refused by the simulator and ignored by the monitor.
    try:
        (root / 'sample_0.txt').unlink()
        (root / 'sample_0.txt').symlink_to(outside)
    except OSError:
        print('SKIP: symlink test requires OS permission')
    else:
        run(simulator, 'normal', str(root), ok=False)
        assert outside.read_text() == 'must survive'
        report = safety / 'symlink-report'
        result = run(monitor, str(root), str(report), '1', '50')
        assert 'files=41' in result.stdout
    print('PASS: refusal of reused sandboxes, unexpected files, hard links, nested/reused reports')
