#!/usr/bin/env python3
# This script is to try and find frequencies that are multiples of 12.288 MHz
# for the Raspberry Pi Pico to run at.  It is now unnecessary because Arduino-Pico 
# does this for us already.
import argparse
import sys

# Fixed hardware parameters - same as cocalc.py
fbdiv_range = range(16, 320 + 1)
postdiv_range = range(1, 7 + 1)
ref_min = 5
refdiv_min = 1
refdiv_max = 63
TARGET_FREQ = 12.288  # Our target frequency in MHz
MAX_FREQ = 400.0  # Maximum frequency to display (MHz)

def validRefdiv(string):
    if ((int(string) < refdiv_min) or (int(string) > refdiv_max)):
        raise ValueError("REFDIV must be in the range {} to {}".format(refdiv_min, refdiv_max))
    return int(string)

parser = argparse.ArgumentParser(description="Find achievable frequencies that are close to multiples of 12.288 MHz")
parser.add_argument("--input", "-i", default=12, help="Input (reference) frequency. Default 12 MHz", type=float)
parser.add_argument("--ref-min", default=5, help="Override minimum reference frequency. Default 5 MHz", type=float)
parser.add_argument("--vco-max", default=1600, help="Override maximum VCO frequency. Default 1600 MHz", type=float)
parser.add_argument("--vco-min", default=750, help="Override minimum VCO frequency. Default 750 MHz", type=float)
parser.add_argument("--max-freq", default=MAX_FREQ, help=f"Maximum output frequency to display. Default {MAX_FREQ} MHz", type=float)
parser.add_argument("--tolerance", default=1.0, help="Maximum percentage deviation from exact multiple. Default 1.0%%", type=float)
args = parser.parse_args()

# Calculate the range of REFDIV values using the same logic as cocalc.py
refdiv_range = range(refdiv_min, max(refdiv_min, min(refdiv_max, int(args.input / args.ref_min))) + 1)

# Store all valid achievable frequencies with their parameters
results = []

# Generate all achievable frequencies with the exact same constraints as cocalc.py
for refdiv in refdiv_range:
    for fbdiv in fbdiv_range:
        vco = args.input / refdiv * fbdiv
        if vco < args.vco_min or vco > args.vco_max:
            continue
        for pd2 in postdiv_range:
            for pd1 in postdiv_range:
                # This is the critical constraint in cocalc.py
                if ((vco * 1000) % (pd1 * pd2)) != 0:
                    continue
                    
                out = vco / pd1 / pd2
                
                # Skip if frequency exceeds our maximum
                if out > args.max_freq:
                    continue
                
                # Calculate how many times TARGET_FREQ fits into out
                multiple = out / TARGET_FREQ
                
                # Calculate percentage deviation from closest integer multiple
                closest_multiple = round(multiple)
                deviation = abs(multiple - closest_multiple) / closest_multiple * 100
                
                # Only include if it's within our tolerance of an integer multiple
                if deviation <= args.tolerance:
                    results.append((out, closest_multiple, deviation, refdiv, fbdiv, pd1, pd2, vco))

# Sort results by frequency
results.sort(key=lambda x: x[0])

# Print header
print(f"Achievable frequencies within {args.tolerance}% of a multiple of {TARGET_FREQ} MHz (up to {args.max_freq} MHz):")
print("┌─────────────────┬────────────┬───────────┬────────┬────────┬─────┬─────┬─────────────┐")
print("│  System Clock   │ Multiple   │ Deviation │ REFDIV │ FBDIV  │ PD1 │ PD2 │  VCO (MHz)  │")
print("│     (MHz)       │ of 12.288  │    (%)    │        │        │     │     │             │")
print("├─────────────────┼────────────┼───────────┼────────┼────────┼─────┼─────┼─────────────┤")

# Print results
for result in results:
    out, multiple, deviation, refdiv, fbdiv, pd1, pd2, vco = result
    print(f"│ {out:14.6f} │ {multiple:8d}x  │ {deviation:7.3f}% │ {refdiv:6d} │ {fbdiv:6d} │ {pd1:3d} │ {pd2:3d} │ {vco:11.3f} │")

print("└─────────────────┴────────────┴───────────┴────────┴────────┴─────┴─────┴─────────────┘")

# Print recommended frequencies
print("\nRecommended achievable frequencies close to multiples of 12.288 MHz:")

# Find frequencies closest to common multiples
common_multiples = [1, 2, 4, 8, 10, 12, 16, 20, 24, 32]
best_matches = []

for target_multiple in common_multiples:
    target_freq = TARGET_FREQ * target_multiple
    if target_freq > args.max_freq:
        continue
        
    # Find the closest achievable frequency to this target
    closest_results = sorted(results, key=lambda r: abs(r[0] - target_freq))
    if closest_results:
        best_match = closest_results[0]
        deviation_pct = abs(best_match[0] - target_freq) / target_freq * 100
        best_matches.append((target_multiple, best_match, deviation_pct))

# Sort recommended frequencies by closeness to target
best_matches.sort(key=lambda x: x[2])  # Sort by deviation percentage

for target_multiple, match, deviation_pct in best_matches:
    out, multiple, deviation, refdiv, fbdiv, pd1, pd2, vco = match
    target_freq = TARGET_FREQ * target_multiple
    
    print(f"- {out:.6f} MHz (target: {target_freq:.6f} MHz = {target_multiple}x{TARGET_FREQ})")
    print(f"  Deviation: {deviation_pct:.6f}%")
    print(f"  REFDIV={refdiv}, FBDIV={fbdiv}, PD1={pd1}, PD2={pd2}, VCO={vco:.3f} MHz")
    print(f"  SYS_CLK_HZ={int(out*1_000_000)}")
    
    # Print the compile definitions needed for this frequency
    print(f"  PLL_SYS_REFDIV={refdiv}")
    print(f"  PLL_SYS_VCO_FREQ_HZ={int((args.input * 1_000_000) / refdiv * fbdiv)}")
    print(f"  PLL_SYS_POSTDIV1={pd1}")
    print(f"  PLL_SYS_POSTDIV2={pd2}")
    print()

# Verify the exact frequencies using cocalc.py approach
print("\nTo verify any frequency, use: python cocalc.py <frequency>")
print("For example: python cocalc.py 123.0")