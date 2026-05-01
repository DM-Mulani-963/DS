# Lamport Clock Simulation in C++


This program simulates **message passing between multiple processes** using **Lamport Logical Clocks**.
It models fundamental distributed systems concepts such as event ordering, synchronization, deadlock detection, and message delivery.


---


## Features


- `send`, `recv`, and `print` operations
- Lamport logical clock maintenance
- Deadlock detection
- Reporting of undelivered messages


---


## Requirements


- C++ compiler supporting (e.g., `g++`)
- Operating System: Windows / Linux / macOS
- Command-line interface (Command Prompt / Terminal)


---


## Project Structure


The directory contains:


- `lamport.cpp` - source code
- `input.txt` - input file with process definitions
- `README.md` - this file
- `testcase.md` - sample test case
- `output.md` - output of sample test case

---


## Compilation and Execution (Windows)


### Step 1: Open Command Prompt and Navigate to Directory

Open **Command Prompt** and move to the directory containing `lamport.cpp`.

### Step 2: Compilation (Windows)

Compile the program using:
```bash
g++ lamport.cpp -o lamport.exe
```

### Step 3: Execute the Program
```bash 
lamport.exe < input.txt
```

The program reads all input from input.txt and prints execution logs, Lamport clocks, deadlock status, and undelivered messages.