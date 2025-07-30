from multiprocessing import Pool
import os
import subprocess

from datetime import datetime
from tqdm import tqdm

instances_dir = "data/Benchmarks"
best_known_file = "data/bestKnownTCT.txt"
executable = "./Heuristic_PFSP_SLS"

# Define the instances and algorithms
algorithms = ["ils" , "genetic"]  # Replace with actual algorithm types


if not os.path.exists(executable):
    raise FileNotFoundError(f"Executable {executable} not found. Please check the path.")

# Read best-known values
best_known = {}
with open(best_known_file, "r") as f:
    next(f)  # Skip header
    for line in f:
        instance, value = line.strip().split(",")
        best_known[instance.strip()] = int(value.strip())
        



# Output directory for logs
output_dir = "./results"
os.makedirs(output_dir, exist_ok=True)
# Generate a timestamped output directory
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
output_file = f"results_{timestamp}.csv"
os.makedirs(output_dir, exist_ok=True)

# Function to run the algorithm
def run_algorithm(args):
    instance, algorithm, best_value, instance_size, runs = args
    group_key = (algorithm, instance_size)
    total_time = 0.0   
    run_results = []

    
    for i in range(runs):
        commande = f"{executable} {instance} --{algorithm}"
        result = subprocess.run(commande, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if result.returncode != 0:
            print(f"Error running command: {commande}")
            print(result.stderr.decode())
            continue
        
        final_value = int(result.stdout.decode().strip().split()[-1])
        print(f"Instance: {instance}, Algorithm: {algorithm}, Run {i + 1}: {final_value}")
        
        deviation = ((final_value - best_value) / best_value) * 100
        print(f"Deviation: {deviation:.2f}%")
        
        run_results.append({
            "instance": instance,
            "algorithm": algorithm,
            "run": i + 1,
            "final_value": final_value,
            "deviation": deviation
        })
        
    return group_key, run_results
        
            
tasks = []

#instance_test_list = ["ta051"]

for instance_file in os.listdir(instances_dir):
    instance_path = os.path.join(instances_dir, instance_file)

    # Read instance size from first line
    with open(instance_path, "r") as f:
        first_line = f.readline().strip()
        instance_size = first_line.split()[0]
        if instance_size == "200":
            num_runs = 3
        else:
            num_runs = 10
            
    instance_key = instance_file.upper().replace("TA0", "TA") # fix the name diff between the file and the best known

    
    if instance_key not in best_known:
        continue
    
    best_value = best_known[instance_key]
    
    for algorithm in algorithms:
        # Check if the algorithm is valid
        if algorithm not in ["ils", "genetic"]: # Add other valid algorithms here
            print(f"Invalid algorithm: {algorithm}")
            continue
        
        tasks.append((instance_path, algorithm, best_value, instance_size, num_runs))
        
with Pool() as pool:
    results = list(tqdm(pool.imap_unordered(run_algorithm, tasks), total=len(tasks), desc="Processing Instances"))
    # Save results to CSV

import pandas as pd
from datetime import datetime
results_pd = []

for group_key, run_results in results:
    for result in run_results:
        results_pd.append({
            "Algorithm": result["algorithm"],
            "Instance": result["instance"],
            "Instance Size": group_key[1],
            "Run": result["run"],
            "Final Value": result["final_value"],
            "Deviation": result["deviation"]
        })

results_df = pd.DataFrame(results_pd, columns=["Algorithm", "Instance", "Instance Size", "Run", "Final Value", "Deviation"])
results_df.to_csv(os.path.join(output_dir, output_file), index=False)
print(f"Results saved to {output_file}")

# Save the parameters from the constant.hpp file
constants_file = "src/constant.hpp"
parameters_output_file = os.path.join(output_dir, f"parameters_{timestamp}.txt")

if os.path.exists(constants_file):
    with open(constants_file, "r") as src_file, open(parameters_output_file, "w") as dest_file:
        dest_file.write(f"Parameters extracted on {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
        dest_file.write("=" * 50 + "\n")
        dest_file.writelines(src_file.readlines())
    print(f"Parameters saved to {parameters_output_file}")
else:
    print(f"Warning: {constants_file} not found. Parameters file was not saved.")
