# MPICH Ring Communication Program

## Overview

This program implements ring communication using MPICH. Each process sends its rank to the next process and receives data from the previous process in a circular topology.

## Compilation

Use the following command to compile the program:

```bash
mpicc ring.c -o ring
```

## Execution

Run the program with more than one process:

```bash
mpirun -np 4 ./ring
```

You can replace 4 with any number of processes greater than 1.

## Program Logic

Each process:

* Sends its rank value to the next process
* Receives data from the previous process
* Repeats this process 10 times
* Adds the received value to a local counter

## Expected Output Behavior

Each process always receives the same value from its previous neighbor.

Formula:

```
Counter = 10 × (previous rank)
```

Example for 4 processes:

```
Rank 0: Counter = 30
Rank 1: Counter = 0
Rank 2: Counter = 10
Rank 3: Counter = 20
```

Note: Output order may vary due to parallel execution.

## Output Snapshot

The output snapshot is attached in the zip file named:

```
output.png
```

## Justification of Output

In a ring communication pattern:

* Each process receives data from (rank - 1 + p) % p
* Each sender always sends its own rank value
* The same value is received 10 times
* The counter accumulates this value over 10 iterations

Therefore:

```
counter = received_value × 10
```

This results in different counter values for each process based on its previous neighbor.

## Conclusion

The program correctly demonstrates ring communication in MPI and uses MPI_Sendrecv to avoid deadlock while ensuring consistent data exchange.
