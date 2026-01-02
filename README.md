# Graph Community Detection

This project is a C program that implements graph community detection algorithms. It uses four different algorithms to detect communities in social network graphs such as the Karate Club dataset.

## 📋 Table of Contents

- Features  
- Algorithms Used  
- Installation  
- Usage  
- Data Format  
- Sample Output  
- Modularity (Q) Value  

## 🎯 Features

- Louvain Algorithm: Two-phase hierarchical community detection
- Fast Greedy Algorithm: Greedy community merging approach
- My Algorithm (Common Neighbor Based): Community assignment based on common neighbors
- My Algorithm 2 (Modularity Based): Community optimization based on modularity improvement
- Modularity calculation to evaluate algorithm performance

## 🔧 Algorithms Used

### 1. Louvain Algorithm
- Phase 1: Each node is assigned to the community that maximizes modularity gain
- Phase 2: Communities are merged into single nodes to build a new graph
- The process is repeated until the modularity value converges

### 2. Fast Greedy Algorithm
- Merges the pair of communities that results in the highest modularity increase
- Continues until no further improvement is possible

### 3. My Algorithm (Common Neighbor Based)
- Each node moves to the community of the neighbor with whom it shares the highest number of common neighbors
- Iteration continues until the communities stabilize

### 4. My Algorithm 2 (Modularity Based)
- For each node, the number of neighboring nodes in each neighboring community is counted
- The community with the highest number of neighbors is selected
- The node changes its community only if modularity increases
- Iteration continues until no node changes its community

## 📦 Installation

### Requirements
- GCC or any C compiler
- Windows, Linux, or macOS

### Compilation

**Windows (GCC/MinGW):**
```bash
gcc proje.c -o a.exe
