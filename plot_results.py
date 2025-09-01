#!/usr/bin/env python3
import csv
import argparse
from collections import defaultdict
import sys

try:
    import matplotlib.pyplot as plt
except Exception as e:
    print('matplotlib not available:', e)
    sys.exit(2)

p = argparse.ArgumentParser()
p.add_argument('--in', dest='infile', default='results_full.csv')
p.add_argument('--out', dest='outfile', default='results.png')
args = p.parse_args()

data = defaultdict(lambda: defaultdict(list))
with open(args.infile) as f:
    r = csv.DictReader(f)
    for row in r:
        prog = row['program']
        threads = int(row['threads'])
        time = row['time_us'] and row['time_us'].strip()
        if time:
            data[prog][threads].append(int(time))

# plot medians
import statistics
for prog, per_threads in data.items():
    xs = sorted(per_threads.keys())
    ys = [int(statistics.median(per_threads[x])) for x in xs]
    plt.plot(xs, ys, marker='o', label=prog)

plt.xlabel('threads')
plt.ylabel('median time (us)')
plt.legend()
plt.xscale('linear')
plt.grid(True)
plt.savefig(args.outfile)
print('Saved plot to', args.outfile)
