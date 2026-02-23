# C Neural Network: Hyperparameter Analysis

**AGKI Project by Korbinian Hirsch and Nicolas Hildebrand**

This repository contains a complete, from-scratch implementation of an Artificial Neural Network in C. The primary goal of this project is to investigate and analyze the influence of various hyperparameters on the network's classification performance.

## ⚠️ Important Note on Pre-trained Weights
The file `network_parameters.csv` currently contains the saved weights of a highly optimized model achieving **98.14% accuracy**. 
**Warning:** Executing a new training run will overwrite this file. Please rename or back up this file if you wish to keep these specific weights!

## Compilation

To compile the project, you need a standard C compiler (like `gcc`). Run the following command in your terminal:

```bash
gcc main.c logic.c ui.c csv.c -o main