## Socket Code Output Justification

### Observed Output

```bash
socket fd: 3
file fd: 4
```

### Explanation


This program demonstrates **file descriptor allocation** in an operating system.


1. **`socket fd: 3`**
- When the program starts, the operating system already reserves:
- `0` → standard input (stdin)
- `1` → standard output (stdout)
- `2` → standard error (stderr)
- The `socket()` system call requests a new TCP socket.
- The OS assigns the **lowest available file descriptor**, which is `3`.
- Hence, the socket is assigned file descriptor `3`.


2. **`file fd: 4`**
- The `open("data", O_RDONLY)` system call opens a regular file named `data`.
- Since file descriptor `3` is already in use by the socket, the OS assigns the **next available descriptor**, which is `4`.
- This confirms that both sockets and files are managed using the same file descriptor table.


---


### Key Observation


- The operating system treats **sockets and regular files uniformly** as file descriptors.
- File descriptor numbers are allocated sequentially based on availability.
- The file name `data` may refer to a text file or any other file type; the OS does not distinguish based on file extensions.


---


### Conclusion


The output verifies that:
- Sockets and files share a common file descriptor namespace.
- The OS assigns descriptors starting from the lowest unused value.
- This behavior supports the UNIX design principle that **everything is a file**.

---

# Vector Clock Simulation in C++

This program simulates **message passing between multiple processes** using **Vector Clocks**.
It models core **distributed systems concepts** such as **causal ordering of events**, **concurrency detection**, **synchronization**, **deadlock detection**, and **message delivery tracking**.

---

## Features

- `send`, `recv`, and `print` operations
- Vector clock maintenance for each process
- Causal ordering of distributed events
- Detection of concurrent events
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

- `vector_clock.cpp` – source code
- `input.txt` – input file describing processes and events
- `README.md` – project documentation
- `testcase.md` – sample test case
- `output.md` – output of the sample test case

---

## Compilation and Execution (Windows)

### Step 1: Open Command Prompt and Navigate to Directory

Open **Command Prompt** and move to the directory containing `vector_clock.cpp`.

### Step 2: Compilation

Compile the program using:
```bash
g++ vector_clock.cpp -o vector_clock.exe
```

### Step 3: Execute the Program

Execute the program using:
```bash
vector_clock.exe < input.txt
```

The program reads all input from input.txt and prints execution logs, Vector clocks, deadlock status, and undelivered messages.