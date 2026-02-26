# AeroTelemetry-Core

A multithreaded, high-performance telemetry data server built with C++ for handling real-time rocket/avionics sensor data. The system concurrently processes TCP and UDP network streams, utilizing a thread-safe priority queue to manage critical flight commands.

## Features

* **Dual-Protocol Networking:** * **TCP Server:** Reliable connection for critical commands (e.g., Engine Ignition, Abort Sequence, Parachute Deployment).
  * **UDP Server:** High-speed, connectionless stream for continuous sensor data (e.g., Pitot Dynamic Pressure, Fin Servo Angles).
* **Multithreaded Architecture:** Utilizes `std::thread` to handle network listening, user input, and data processing simultaneously without blocking.
* **Thread-Safe Priority Queue:** Implements the Producer-Consumer pattern using `std::mutex` and `std::condition_variable` to safely manage concurrent data insertion and extraction.
* **Graceful Shutdown:** Intercepts OS signals (`SIGINT`, `SIGTERM`) to securely close file descriptors, unblock listening sockets, and terminate threads safely preventing memory leaks.
* **Blackbox Logging:** Automatically logs "Critical" priority events to a persistent `.txt` file for post-flight analysis.

## System Architecture

The project is built on a robust Object-Oriented structure:
* **BaseServer (Abstract):** Defines the core networking interface and polymorphic `stop()` capabilities.
* **TCP/UDP Derived Servers:** Specialized networking implementations overriding the `run()` loop.
* **RocketControl:** The central processing hub that encapsulates the priority queue, handles thread synchronization, and manages I/O operations.

## Build and Run Instructions

This project is built for Linux/Unix environments.

### Compilation
A `Makefile` is provided for easy compilation. Run the following command in the project directory:

```bash
make all
Execution
Run the compiled system using:

Bash

make run
(Alternatively, run ./RocketSystem directly)

Usage
Once the server is running, it listens on:

Port 8080 (TCP)

Port 8081 (UDP)

You can use the provided client application (or any standard network utility like netcat) to send telemetry data in the following format:
[ID] [Task_Name] [Priority_Level]

Priority Levels:

0 : Low

1 : Mid

2 : High

3 : Critical (Logged to disk)

Press Ctrl+C in the terminal to trigger a graceful shutdown sequence.

Future Improvements (Roadmap)
Implementing std::unique_ptr for dynamic, scalable server instantiation.

Decoupling the logging system into an independent TelemetryLogger class (Single Responsibility Principle).

Adding highly granular POSIX timestamps to the telemetry log files.
