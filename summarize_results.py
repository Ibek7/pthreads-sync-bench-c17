#!/usr/bin/env python3
"""
Summarize benchmark CSV results into median/mean/stddev per program and thread count.
Produces an output CSV with columns: program, threads, count, median_us, mean_us, stdev_us
"""
import csv
import argparse
from collections import defaultdict
import statistics

p = argparse.ArgumentParser()
p.add_argument('--in', dest='infile', default='results_full.csv')
p.add_argument('--out', dest='outfile', default='results_full_summary.csv')
args = p.parse_args()

data = defaultdict(list)
with open(args.infile) as f:
    r = csv.DictReader(f)
    for row in r:
        t = row.get('time_us','').strip()
        if not t:
            continue
        key = (row['program'], int(row['threads']))
        data[key].append(int(t))

with open(args.outfile, 'w', newline='') as out:
    w = csv.writer(out)
    w.writerow(['program','threads','n','median_us','mean_us','stdev_us'])
    for (prog, threads), vals in sorted(data.items()):
        n = len(vals)
        median = int(statistics.median(vals))
        mean = int(statistics.mean(vals))
        stdev = int(statistics.pstdev(vals)) if n>1 else 0
        w.writerow([prog, threads, n, median, mean, stdev])

print('Wrote summary to', args.outfile)
