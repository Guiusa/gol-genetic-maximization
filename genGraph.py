import sys
import matplotlib.pyplot as plt

# Read input
lines = sys.stdin.read().strip().splitlines()

N = int(lines[0])

if len(lines) != N + 1:
    raise ValueError(f"Expected {N} data lines, but received {len(lines) - 1}.")

# Parse the three values from each line
data = []

for i in range(1, N + 1):
    values = list(map(int, lines[i].split()))

    if len(values) != 3:
        raise ValueError(f"Line {i} must contain exactly 3 integers.")

    data.append(values)

# Extract the three series
series1 = [row[0] for row in data]
series2 = [row[1] for row in data]
series3 = [row[2] for row in data]

# X-axis: data point index
x = range(1, N + 1)

# Create the plot
plt.figure(figsize=(10, 5))

plt.plot(x, series1, marker='o', label='Melhor indivíduo')
plt.plot(x, series2, marker='o', label='Pior indivíduo')
plt.plot(x, series3, marker='o', label='Média')

plt.xlabel("Geração")
plt.ylabel("Células vivas")
plt.title("Evolução do número de células vivas ao final da simulação")

plt.legend()
plt.grid(True)

# Save the graph
plt.savefig("graph.png", dpi=300, bbox_inches="tight")

print("Graph saved as graph.png")
