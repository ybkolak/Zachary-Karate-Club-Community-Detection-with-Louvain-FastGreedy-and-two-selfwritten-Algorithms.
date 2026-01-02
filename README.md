# Graph Community Detection

This project is a C program that implements graph community detection algorithms. It uses four different algorithms to detect communities in social network graphs such as the Karate Club dataset.

## 📋 Contents

- [Features](#features)
- [Algorithms Used](#algorithms-used)
- [Installation](#installation)
- [Usage](#usage)
- [Data Format](#data-format)
- [Sample Output](#sample-output)
- [Modularity (Q) Value](#modularity-q-value)

## 🎯 Features

- **Louvain Algorithm**: Two-phase hierarchical community detection
- **Fast Greedy Algorithm**: Greedy community merging approach
- **My Algorithm (Common Neighbor Based)**: Community assignment based on the number of common neighbors
- **My Algorithm 2 (Modularity Based)**: Community optimization based on the number of neighboring communities
- **Modularity Calculation**: Calculates Q value to measure the success of each algorithm

## 🔧 Algorithms Used

### 1. Louvain Algorithm
- **Phase 1**: Assigns communities to each node to maximize modularity increase
- **Phase 2**: Merges communities into single nodes to create a new graph
- This process repeats until the modularity value stabilizes

### 2. Fast Greedy Algorithm
- Merges the pair of communities that provides the highest modularity increase
- Continues until no further increase is possible

### 3. My Algorithm (Common Neighbor Based)
- Each node joins the community of its neighbor with whom it shares the most common neighbors
- Iterates until communities stabilize

### 4. My Algorithm 2 (Modularity Based)
- Counts neighboring communities for each node
- Selects the community with the most neighboring nodes
- Changes community if modularity increases
- Continues until no node changes

## 📦 Installation

### Requirements
- GCC or any C compiler
- Windows, Linux, or macOS operating system

### Compilation

**Windows (GCC/MinGW):**
```bash
gcc proje.c -o a.exe
```

**Linux/macOS:**
```bash
gcc proje.c -o a.out
```

## 🚀 Usage

1. Place the `data.txt` file in the project directory
2. Compile the program
3. Run the program:

**Windows:**
```bash
.\a.exe
```

**Linux/macOS:**
```bash
./a.out
```

## 📄 Data Format

The data file (`data.txt`) should be in the following format:

```
[2 1]
[3 1] [3 2]
[4 1] [4 2] [4 3]
[5 1]
...
```

- Each line contains one or more edges
- Format: `[node1 node2]`
- Nodes start from 1
- The graph is undirected

## 📊 Sample Output

```
Node : 34, Edge : 78

test 1: Q : 0.381166, Number of Communities= 3
Louvain node-community:
Community 0 : 1 2 3 4 5 6 7 8 10 11 12 13 14 17 18 20 22
Community 1 : 9 15 16 19 21 23 24 27 28 29 30 31 32 33 34
Community 2 : 25 26

 Fast Greedy 
Q : 0.380671, commun= 3
Fast Greedy node-community:
Community 0 : 1 5 6 7 11 12 17 20
Community 1 : 2 3 4 8 10 13 14 18 22
Community 8 : 9 15 16 19 21 23 24 25 26 27 28 29 30 31 32 33 34

 My Algorithm 
Q : 0.356016, commun= 3
My Algorithm node-community:
Community 1 : 1 2 3 4 5 6 7 8 10 11 12 13 14 17 18 20 22
Community 25 : 25 26
Community 33 : 9 15 16 19 21 23 24 27 28 29 30 31 32 33 34

 My Algorithm 2 
Q : 0.412, commun= 4
My Algorithm 2 node-community:
...
```

## 📈 Modularity (Q) Value

Modularity (Q) is a metric that measures the quality of the community structure:
- **Value range**: between -0.5 and 1.0
- **High Q value**: Strong community structure
- **Low Q value**: Weak or random community structure
- **Q ≈ 0.3-0.7**: Good values for typical social networks

### Formula:
$$Q = \frac{1}{2m} \sum_{i,j} \left[ A_{ij} - \frac{k_i k_j}{2m} \right] \delta(c_i, c_j)$$

Where:
- $m$: Total number of edges
- $A_{ij}$: Adjacency matrix
- $k_i, k_j$: Degrees of nodes
- $\delta(c_i, c_j)$: 1 if in the same community, 0 otherwise

## 🏗️ Project Structure

```
graph/
├── proje.c       # Main source code
├── data.txt      # Data file (graph edges)
├── README.md     # This file
└── a.exe         # Compiled program (Windows)
```

## 📚 Data Structure

```c
typedef struct graph{
    int node_number;      // Number of nodes
    int edge_number;      // Number of edges
    int *community;       // Community ID for each node
    int **matrix;         // Adjacency matrix
} graph;
```

## 🔍 Functions

### Basic Functions
- `connected_edge()`: Checks for an edge between two nodes
- `expected_value()`: Calculates expected edge value
- `modularity_network()`: Calculates modularity for the entire network
- `modularity_singular()`: Calculates modularity for a single community

## 📝 Notes

- The program tests the same dataset with four different algorithms
- Each algorithm uses a separate copy of the graph
- Results can be compared using the modularity value
- The Louvain algorithm generally gives the highest Q value

