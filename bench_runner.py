#!/usr/bin/env python3
"""
Simple benchmark runner for the pthreads-sync-bench-c17 programs.
Generates a CSV with columns: program,threads,ops_per_thread,trial,time_us,raw_output

Usage examples:
  ./bench_runner.py --threads 1 2 4 --ops 1000 --trials 3 --out results.csv
"""
import subprocess
import argparse
import csv
import re
import shlex
from pathlib import Path

DEFAULT_PROGRAMS = ["./counter_mutex", "./approx_counter", "./list_hh", "./hash_single", "./hash_buckets"]
TIME_RE = re.compile(r"time_us=([0-9]+)")

def run_one(cmd, timeout=60):
    try:
        p = subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=timeout)
        out = p.stdout + p.stderr
        return p.returncode, out
    except subprocess.TimeoutExpired as e:
        return -1, f"TIMEOUT after {timeout}s"

def parse_time(output):
    m = TIME_RE.search(output)
    if m:
        return int(m.group(1))
    return None

def main():
    p = argparse.ArgumentParser()
    p.add_argument("--programs", nargs="*", default=DEFAULT_PROGRAMS)
    p.add_argument("--threads", nargs="*", type=int, default=[1,2,4,8])
    p.add_argument("--ops", type=int, default=50000, help="ops per thread (or iters for counters)")
    p.add_argument("--trials", type=int, default=3)
    p.add_argument("--out", default="results.csv")
    p.add_argument("--timeout", type=int, default=60)
    args = p.parse_args()

    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    with out_path.open("w", newline="") as csvf:
        writer = csv.writer(csvf)
        writer.writerow(["program","threads","ops_per_thread","trial","time_us","raw_output"])

        for prog in args.programs:
            if not Path(prog).exists():
                print(f"Warning: program {prog} not found, skipping")
                continue
            for threads in args.threads:
                for trial in range(1, args.trials+1):
                    cmd = f"{shlex.quote(prog)} {threads} {args.ops}"
                    print(f"Running: {cmd} (trial {trial})")
                    rc, out = run_one(cmd, timeout=args.timeout)
                    time_us = parse_time(out)
                    if time_us is None:
                        print(f"Could not parse time from output of {prog}:\n{out}")
                    writer.writerow([prog, threads, args.ops, trial, time_us if time_us is not None else '', out.replace('\n', ' | ')[:1000]])

    print(f"Results written to {out_path}")

if __name__ == '__main__':
    main()
