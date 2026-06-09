# Button Debounce State Machine

## 1. Architectural Overview

This module implements a deterministic software debounce algorithm using a finite state machine (FSM).

Mechanical push-buttons often generate unstable transitions (bounce) when pressed or released. Instead of immediately trusting the raw GPIO signal, the state machine validates transitions before accepting them as legitimate button events.

The implementation consists of three states:

- STATE_IDLE
- STATE_PRESS_DETECTED
- STATE_PRESSED

The debounce routine is intended to execute periodically (every 5 ms) from an ISR or RTOS task.

### State Flow

STATE_IDLE
→ detects potential press
→ STATE_PRESS_DETECTED

STATE_PRESS_DETECTED
→ confirms stable press
→ STATE_PRESSED

STATE_PRESSED
→ detects release
→ STATE_IDLE

This design filters unstable button transitions while maintaining deterministic behavior.

---

## 2. Assumptions and Trade-offs

Assumptions:

- GPIO input uses active-low logic.
- 1 = released
- 0 = pressed
- Function executes periodically every 5 ms.
- No hardware debounce circuitry is present.

Trade-offs:

- Simple and deterministic implementation.
- Minimal memory footprint.
- Very low CPU overhead.
- Fixed debounce behavior based on periodic sampling.
- Less configurable than timer-based debounce implementations.

---

## 3. Verification Steps

1. Build project:

make

2. Simulate button input by calling:

debounce_button(raw_pin_state);

with sequences containing bounce patterns.

Example:

1, 1, 0, 1, 0, 0, 0

Expected result:

- Temporary bounce transitions are ignored.
- Stable press is accepted only after confirmation.
- Release correctly returns the state machine to IDLE.

---

## 4. Build Instructions

Build:

make

Clean:

make clean

Compiler flags:

-std=c11 -Wall -Wextra -Werror -O2