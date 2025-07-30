import os
import pandas as pd
from scipy.stats import wilcoxon

# Function to perform Wilcoxon test and print results
def perform_wilcoxon_test(group1, group2, description):
    stat, p_value = wilcoxon(group1, group2)
    print(f"{description}")
    print(f"Wilcoxon test statistic: {stat}, p-value: {p_value}")
    if p_value < 0.05:
        print("=> Statistically significant difference")
        if group1.mean() < group2.mean():
            print("ILS has better performance on average.")
        else:
            print("Genetic has better performance on average.")
        print("\n")
    else:
        print("=> No statistically significant difference\n")

# Path to the results directory
results_dir = "./results"

# Get the latest result file
result_file = os.path.join(results_dir, sorted(os.listdir(results_dir))[-1])
print(f"Analyzing results from: {result_file}")

# Read the CSV file into a DataFrame
df = pd.read_csv(result_file)

# Ensure the data contains both algorithms for each instance size
instance_sizes = df['Instance Size'].unique()

print("Wilcoxon Test Results for Each Instance Size:")
print("--" * 50)

for size in instance_sizes:
    # Filter data for the current instance size
    size_data = df[df['Instance Size'] == size]
    
    # Ensure both algorithms have data for all runs
    ils_deviation = size_data[size_data['Algorithm'] == 'ils']['Deviation']
    genetic_deviation = size_data[size_data['Algorithm'] == 'genetic']['Deviation']
    
    # Perform the Wilcoxon signed-rank test
    if len(ils_deviation) == len(genetic_deviation):  # Ensure equal lengths
        description = f"Instance Size: {size} - Comparing ILS vs Genetic (All Runs)"
        # print(ils_deviation)
        # print(genetic_deviation)
        perform_wilcoxon_test(ils_deviation, genetic_deviation, description)
    else:
        print(f"Skipping instance size {size} due to mismatched data lengths.")

print("--" * 50)
print("Analysis complete.")