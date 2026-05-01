# Distributed Systems Lab 4

**Environment:** Linux (WSL) using `gcc` compiler

------------------------------------------------------------------------

## Question 1: UDP Client--Server with Vector Clocks

### Objective

To modify an existing UDP client and server to demonstrate causal
ordering using vector clocks and to observe their behavior with multiple
concurrent clients.

------------------------------------------------------------------------

### Modifications

#### Server

-   Uses UDP on port **5001**
-   After `recvfrom()`:
    -   Prints the received message
    -   Updates and prints its vector clock
-   Sends an echo reply containing the updated server vector clock
-   Processes a finite number of messages (15 = 3 clients × 5 messages)

#### Client

-   Executes a loop **5 times**
-   In each iteration:
    -   Increments its vector clock
    -   Sends its **PID** and vector clock to the server
    -   Waits for the server echo
    -   Prints its PID and updated vector clock
    -   Sleeps for **5 seconds**

------------------------------------------------------------------------

### Experiment

-   Server is started first
-   Three UDP clients are run in separate terminals
-   Each client sends 5 messages
-   Server processes 15 messages and exits gracefully

------------------------------------------------------------------------

### Output

The execution output of the UDP server and three UDP clients is captured
and attached.

-   The server output shows messages received from different clients
    along with updated vector clocks.
-   Each client output shows sent messages, echoed responses, and
    updated vector clock values.

The screenshot of the output is included in the ZIP file as:

-   **q1.png**

------------------------------------------------------------------------

### Justification

Vector clocks are updated on every send and receive event.\
The server merges the received vector clock with its own and increments
its entry, preserving causal ordering.\
Multiple clients demonstrate interleaving and causal dependency among
distributed processes.

------------------------------------------------------------------------

# Question 2: TCP Server -- Zombie Process Elimination

## Objective

To modify a concurrent TCP server so that terminated child processes do
not remain as zombie processes, and to verify this behavior using Linux
process monitoring commands.

------------------------------------------------------------------------

## Problem Description

In a fork-based concurrent TCP server, each client connection creates a
child process using `fork()`. When the child process terminates, it
becomes a zombie if the parent process does not collect its exit status
using `wait()` or `waitpid()`.

Zombie processes occupy entries in the process table and are undesirable
in long-running servers.

------------------------------------------------------------------------

## Modification to Server Code

The server was modified to handle the `SIGCHLD` signal by ignoring it:

``` c
signal(SIGCHLD, SIG_IGN);
```

Ignoring `SIGCHLD` instructs the operating system to automatically reap
child processes when they terminate, thereby preventing zombie
processes.

No changes were required in the client code.

------------------------------------------------------------------------

## Experimental Procedure

1.  The modified TCP server was started.
2.  The TCP client was executed multiple times (n times) to create
    multiple child processes on the server.
3.  After client executions completed, the process table was examined
    using the following commands:

``` bash
ps -el | grep Z
ps -ef | grep defunct
```

------------------------------------------------------------------------

## Output of ps Commands

### Command:

``` bash
ps -el | grep Z
```

### Output:

``` text
F S   UID     PID    PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
```

No process is shown in the zombie (Z) state.

------------------------------------------------------------------------

### Command:

``` bash
ps -ef | grep defunct
```

### Output:

``` text
jagrav   1037   897  0 14:27 pts/2    00:00:00 grep --color=auto defunct
```

The output corresponds only to the `grep` command itself. No `<defunct>`
processes are present.

------------------------------------------------------------------------

## Observation

-   No processes were found in the zombie (`Z`) state.
-   No `<defunct>` child processes existed after multiple client
    executions.
-   This confirms that child processes were successfully reaped.

------------------------------------------------------------------------

## Conclusion

The absence of zombie or defunct processes in the `ps` command output
confirms that the modified TCP server correctly handles child process
termination. By ignoring the `SIGCHLD` signal, the operating system
automatically reaps child processes, ensuring that the server remains
free of zombie processes even after handling multiple client
connections.

This experiment successfully demonstrates zombie process elimination in
a concurrent TCP server.


## Question 3: TCP `shutdown()` vs `close()`

### Objective

To observe TCP connection termination behavior when using
`shutdown(SHUT_WR)` instead of `exit()` or `close()`.

------------------------------------------------------------------------

### Modification

-   Client calls:

``` c
shutdown(sockfd, SHUT_WR);
```

-   Client then calls `recv()` twice
-   Second `recv()` returns **0**, indicating that FIN from server is
    received

------------------------------------------------------------------------

### Explanation

-   `close()` closes the socket for both reading and writing
-   `shutdown(SHUT_WR)` closes only the write side
-   Client can still receive data after sending FIN
-   Enables **half-close** behavior

------------------------------------------------------------------------

### Experiment & Output

-   Server and client are run on a Linux system
-   TCP states are observed using:

``` bash
ss -tanp
```

-   States such as `FIN-WAIT` and `TIME-WAIT` are observed

Screenshots of the output are included in the ZIP file.

------------------------------------------------------------------------

### When Is This Needed?

-   When a client finishes sending data but still expects a response
-   Used in protocols like HTTP where request and response phases are
    separate
-   Ensures graceful connection termination without data loss

------------------------------------------------------------------------

## Conclusion

All three experiments successfully demonstrate: - Vector clock--based
causal ordering using UDP - Zombie-free TCP server operation -
Controlled TCP connection termination using `shutdown()`

The implementations conform to the problem requirements and were
verified using execution outputs.
