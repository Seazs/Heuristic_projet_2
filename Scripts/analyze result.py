import os
import pandas as pd


# Get the last file in the directory (alphabetical order)
results_dir = "./results"
result_file = os.path.join(results_dir, sorted(os.listdir(results_dir))[-1])

print(f"Analyzing results from: {result_file}")

# Read the CSV file into a DataFrame
df = pd.read_csv(result_file)

# Group by Algorithm and Instance Size, then calculate the average deviation
average_deviation = df.groupby(['Algorithm', 'Instance Size'])['Deviation'].mean()

# Print the results
print(average_deviation)