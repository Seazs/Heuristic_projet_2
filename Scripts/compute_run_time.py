import subprocess
import time

def compute_average_runtime(executable_path, args, runs=10):
    total_time = 0.0

    for i in range(runs):
        start_time = time.time()
        subprocess.run([executable_path] + args, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        end_time = time.time()

        run_time = end_time - start_time
        total_time += run_time
        print(f"Run {i + 1}: {run_time:.4f} seconds")

    average_time = total_time / runs
    print(f"\nAverage runtime over {runs} runs: {average_time:.4f} seconds")
    return average_time

if __name__ == "__main__":
    executable_path = "./Heuristic_PFSP"
    args = ["data/Benchmarks/ta101", "--first", "--vnd1", "--srz"]
    compute_average_runtime(executable_path, args)