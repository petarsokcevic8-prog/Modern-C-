# Section 5: Embedded C & RTOS Fundamentals

This section contains low-level embedded C implementations focused on deterministic execution, hardware-level bit manipulation, and real-time state handling.

The implementation avoids dynamic memory allocation and follows strict embedded constraints (C99/C11 compliance, no heap usage, predictable runtime behavior).

---

# Task 1: Memory-Mapped Hardware Bit Manipulation

## 1. Architectural Overview

This task demonstrates direct interaction with hardware through memory-mapped registers.

A fixed memory address (0x40021004) is treated as a 32-bit hardware register using a volatile pointer. The volatile keyword ensures the compiler does not optimize away reads/writes, as the value may change outside program control (hardware-driven updates).

Bit manipulation is performed using standard bitwise operators:
- OR (|) for setting bits
- AND with negation (& ~) for clearing bits
- XOR (^) for toggling bits

This approach ensures that only targeted bits are modified while preserving all other register configurations.

---

## 2. Assumptions and Trade-offs

- Assumes a 32-bit memory-mapped hardware register
- No validation of hardware address validity (typical in embedded systems)
- Uses direct bitwise operations for deterministic performance
- Trade-off: low-level control over safety abstractions

---

## 3. Verification Steps

1. Compile using a C11-compliant compiler: