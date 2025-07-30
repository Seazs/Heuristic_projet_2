import subprocess
import matplotlib.pyplot as plt
import re
import os
from concurrent.futures import ThreadPoolExecutor

# Define the instances and their best-known values
instances = ["ta101"]#"ta051", "ta081",
best_known_file = "./data/bestKnownTCT.txt"
executable = "./Heuristic_PFSP_SLS"
algorithms = ["--ils", "--genetic"]

# Read best-known values
best_known = {}
with open(best_known_file, "r") as f:
    next(f)  # Skip header
    for line in f:
        instance, value = line.strip().split(",")
        best_known[instance.strip()] = int(value.strip())

# Function to run the program and capture output
def run_instance(instance, algorithm):
    command = [executable, f"./data/Benchmarks/{instance}", algorithm]
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if result.returncode != 0:
        print(f"Error running instance {instance} with {algorithm}: {result.stderr}")
        return None
    return result.stdout

# Function to parse output and extract time and scores
def parse_output(output):
    time_scores = []
    for line in output.splitlines():
        match = re.search(r"([\d.]+)\s+(\d+)", line)
        if match:
            time = float(match.group(1))
            score = int(match.group(2))
            time_scores.append((time, score))
    return time_scores

# Plot the evolution of scores
def plot_comparison(instance, results, best_value):
    plt.figure(figsize=(10, 6))
    for algorithm, time_scores in results.items():
        times, scores = zip(*time_scores)
        deviations = [(score - best_value) / best_value * 100 for score in scores]  # Calculate deviation in %
        plt.plot(times, deviations, label=f"{algorithm} Deviation", marker="o")
    plt.title(f"Comparison of Algorithms for {instance}")
    plt.xlabel("Time (s)")
    plt.ylabel("Deviation from Best Known Solution (%)")
    plt.legend()
    plt.grid()
    plt.savefig(f"./fig/comparison_{instance}.png")  # Save the plot to a file
    plt.close()  # Close the plot to free memory

# Function to process a single instance
def process_instance(instance):
    print(f"Running instance: {instance}")
    best_value = best_known.get(instance.upper().replace("TA0", "TA"), None)
    if best_value is None:
        print(f"Best known value for {instance} not found.")
        return

    results = {}
    for algorithm in algorithms:
        print(f"Running {algorithm} for {instance}")
        output = run_instance(instance, algorithm)
        if output:
            time_scores = parse_output(output)
            results[algorithm] = time_scores

    if results:
        plot_comparison(instance, results, best_value)

# Main script
if __name__ == "__main__":
    # Use ThreadPoolExecutor to run instances in parallel
    with ThreadPoolExecutor() as executor:
        executor.map(process_instance, instances)