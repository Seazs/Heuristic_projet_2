import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt



# Get the last file in the directory (alphabetical order)
results_dir = "./results"
result_file = os.path.join(results_dir, sorted(os.listdir(results_dir))[-1])
#result_file = os.path.join(results_dir, "results_20250514_125905.csv")

print(f"Analyzing results from: {result_file}")

# Read the CSV file into a DataFrame
df = pd.read_csv(result_file)

# Group by Algorithm and Instance Size, then calculate the average deviation
average_deviation = df.groupby(['Algorithm', 'Instance Size'])['Deviation'].mean()

# Print the results
print(average_deviation)

# Compute the average deviation per instance and per algorithm
average_deviation_per_instance = df.groupby(['Instance', 'Algorithm'])['Deviation'].mean().unstack()

# Create a bar plot for each instance and algorithm
average_deviation_per_instance.plot(kind='bar', figsize=(12, 6))

plt.xlabel('Instance')
plt.ylabel('Average Deviation')
plt.title('Average Deviation per Instance for Each Algorithm')
plt.legend(title='Algorithm')
plt.grid(axis='y')
plt.tight_layout()
plt.show()




# Compute the average deviation per algorithm and per instance
average_deviation_per_algorithm = df.groupby(['Algorithm', 'Instance', 'Instance Size'])['Deviation'].mean().unstack('Algorithm')

# Create a scatter plot with different colors for different instance sizes
plt.figure(figsize=(10, 6))
for instance_size, group in average_deviation_per_algorithm.groupby(level='Instance Size'):
    plt.scatter(group['ils'], group['genetic'], label=f'Instance Size: {instance_size}')

# Set the same axis limits starting at 0
plt.xlim(0, max(average_deviation_per_algorithm['ils'].max(), average_deviation_per_algorithm['genetic'].max()) * 1.1)
plt.ylim(0, max(average_deviation_per_algorithm['ils'].max(), average_deviation_per_algorithm['genetic'].max()) * 1.1)

# Draw the line y = x
x = np.linspace(0, plt.xlim()[1], 100)
plt.plot(x, x, color='red', linestyle='--', label='y = x')

plt.xlabel('Average Deviation ILS')
plt.ylabel('Average Deviation Genetic')
plt.title('Correlation between ILS and Genetic Average Deviation by Instance Size')
plt.legend()
plt.grid()
plt.show()





