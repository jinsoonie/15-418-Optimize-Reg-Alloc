# Python script to automate testing AND graph using matplotlib
# CHECK the commands, and change the "cases" to specify which testcases

import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np  # Import numpy for handling data conversions

# Define the parameters
num_threads = [1, 2, 4, 8]
execution_modes = ['-seq', '-openmpv1', '-openmpv2', '-openmpv3', '-openmpv4']
### CHANGE ME ###
cases = [(500, 15000), (500, 124750), (500, 80000)]

# Initialize DataFrame
columns = ['Test Case', 'Num Threads', 'Execution Mode', 'Elapsed Time MCS', 'Elapsed Time Coloring', 'Colors Used']
results = []  # Use a list to collect data

# Function to run command and parse output
def run_command(command):
    result = subprocess.run(command, stdout=subprocess.PIPE, text=True)
    output = result.stdout.split('\n')
    elapsed_mcs = float(output[3].split()[-2])
    elapsed_coloring = float(output[4].split()[-2])
    colors_used = int(output[5].split()[-2])
    return elapsed_mcs, elapsed_coloring, colors_used

# Clean and make
subprocess.run(["make", "clean"])
subprocess.run(["make"])

# Iterate over test cases
for mode in execution_modes:
    for n_threads in num_threads:
        for case in cases:
            command = ['./regalloc', str(case[0]), str(case[1]), str(n_threads), mode]
            elapsed_mcs, elapsed_coloring, colors_used = run_command(command)
            results.append({
                'Test Case': f'{case[0]} {case[1]}',
                'Num Threads': n_threads,
                'Execution Mode': mode,
                'Elapsed Time MCS': elapsed_mcs,
                'Elapsed Time Coloring': elapsed_coloring,
                'Colors Used': colors_used
            })

# Convert results to DataFrame
results_df = pd.DataFrame(results, columns=columns)

# Save results to a CSV file
results_df.to_csv('results.csv', index=False)

# Generate and save plots
for case in cases:
    case_str = f'{case[0]} {case[1]}'
    for metric in ['Elapsed Time MCS', 'Elapsed Time Coloring', 'Colors Used']:
        # Plot with normal scale
        plt.figure(figsize=(10, 6))
        for mode in execution_modes:
            subset_case = results_df[(results_df['Execution Mode'] == mode) & (results_df['Test Case'] == case_str)]
            if not subset_case.empty:
                plt.plot(subset_case['Num Threads'].to_numpy(), subset_case[metric].to_numpy(), label=f'{mode}')
        plt.title(f'{metric} for Test Case {case_str} - Normal Scale')
        plt.xlabel('Number of Threads')
        plt.ylabel(metric)
        plt.legend()
        plt.grid(True)
        plt.savefig(f'{case_str}_{metric}_normal_plot.png')  # Save the plot as PNG
        # plt.show()

        # Plot with log scale
        plt.figure(figsize=(10, 6))
        for mode in execution_modes:
            subset_case = results_df[(results_df['Execution Mode'] == mode) & (results_df['Test Case'] == case_str)]
            if not subset_case.empty:
                plt.plot(subset_case['Num Threads'].to_numpy(), subset_case[metric].to_numpy(), label=f'{mode}')
        plt.title(f'{metric} for Test Case {case_str} - Log Scale')
        plt.xlabel('Number of Threads')
        plt.ylabel(metric)
        plt.yscale('log')
        plt.legend()
        plt.grid(True)
        plt.savefig(f'{case_str}_{metric}_log_plot.png')  # Save the plot as PNG
        # plt.show()
