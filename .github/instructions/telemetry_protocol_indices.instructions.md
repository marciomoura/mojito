### applyTo: `software/yeet_application/sfc_embedded/**`, `software/yeet/tools/monitor_client/**`

# AI Instructions: Telemetry Signal Index Mapping

This document explains the critical relationship between C++ telemetry builders and Python decoders. **Misaligned indices will cause boolean signals to display incorrect values in the monitor.**

## Overview

The telemetry system packs control signals into a binary packet:
- **C++ side**: Builds packets with raw signals (floats) and packed booleans (uint32)
- **Python side**: Decodes packets, unpacks booleans, and expands the signal array

## C++ Telemetry Builder Structure

### Signal Types
1. **Float signals**: Added via `packet.add_signal(index, value)` - each occupies one slot
2. **Boolean packs**: Added via `packet.add_bool_pack(index, {bool1, bool2, ...})` - packs up to 32 bools into one uint32 slot

### Index Assignment Rules
- Indices are **explicit** in C++ - each `add_signal()` or `add_bool_pack()` specifies its index
- Indices do NOT need to be sequential, but gaps waste bandwidth
- The packet header contains `signal_count` = highest index + 1

### Example (Grid-Side):
```cpp
// Raw signals at indices 0-41
packet.add_signal(0, voltage_a);
packet.add_signal(1, voltage_b);
// ... more signals ...
packet.add_signal(41, notch_duration);

// Boolean packs at indices 42-43
packet.add_bool_pack(42, {bool1, bool2, ...});  // 32 bools → bits 0-31
packet.add_bool_pack(43, {bool33, bool34, ...}); // More bools → bits 0-7

// More raw signals at indices 44-82
packet.add_signal(44, line_voltage_ab);
// ...
```

## Python Decoder Structure

### Raw Signal Reception
The decoder receives `signal_count` uint32 values and stores them in two arrays:
- `raw_signals[]`: Original uint32 values (needed for bool pack extraction)
- `signals[]`: Values converted to float via bit-cast

### Boolean Unpacking
Boolean packs are extracted using `get_raw_signal(pack_index)` and unpacked bit-by-bit:
```python
def _get_bool_pack_1(self) -> list[bool]:
    raw = self.get_raw_signal(BOOL_PACK_1_INDEX, 0)  # e.g., index 42 for grid
    return unpack_bools(raw, 32)  # Returns [bit0, bit1, ..., bit31]
```

### Signal Array Expansion
After decoding, `_expand_signals_with_bools()` appends unpacked booleans to `signals[]`:
```python
def _expand_signals_with_bools(self):
    bools1 = self._get_bool_pack_1()  # 32 bools
    bools2 = self._get_bool_pack_2()  # N bools
    all_bools = bools1 + bools2
    
    for b in all_bools:
        self.signals.append(1.0 if b else 0.0)
```

## Virtual Index Calculation

**CRITICAL**: Virtual indices for unpacked booleans = `raw_signal_count + bool_position`

### Grid-Side Example
- Raw signals: indices 0-82 (83 signals total)
- Bool pack 1 at index 42: 32 bools
- Bool pack 2 at index 43: 8 bools
- After expansion: bools appended at indices 83-122

```
Virtual Index = 83 + position_in_combined_bool_array

Bool pack 1, bit 0 (modulator release_pulses) → index 83
Bool pack 1, bit 24 (is_tripped)              → index 107
Bool pack 2, bit 0 (overvoltage_warning)      → index 115
```

### Machine-Side Example
- Raw signals: indices 0-71 (72 signals total)
- Bool pack 1 at index 51: 32 bools
- Bool pack 2 at index 52: 5 bools
- After expansion: bools appended at indices 72-108

```
Virtual Index = 72 + position_in_combined_bool_array

Bool pack 1, bit 0 (modulator release_pulses) → index 72
Bool pack 1, bit 22 (is_tripped)              → index 94
Bool pack 1, bit 23 (is_inhibited)            → index 95
Bool pack 2, bit 0 (comm_loss_warning)        → index 104
```

## Synchronization Checklist

When modifying telemetry, ensure ALL of these match:

### 1. C++ Builder (`*_telemetry_builder.cpp`)
- [ ] Signal indices in `add_signal()` calls
- [ ] Bool pack indices in `add_bool_pack()` calls
- [ ] Order of booleans within each pack (bit 0 first, bit 31 last)
- [ ] Comments documenting bit positions

### 2. Python Signal Index Enum (`*_telemetry_protocol.py`)
- [ ] Raw signal indices (e.g., `BOOL_PACK_1 = 51`)
- [ ] Virtual indices = `raw_signal_count + bool_position`
- [ ] Comments documenting which bit each virtual index represents

### 3. Python Decoder (`telemetry_protocol.py`)
- [ ] `_get_bool_pack_N()` reads correct raw signal index
- [ ] `_get_bool_pack_N()` docstring matches C++ bit layout
- [ ] Property getters (e.g., `protections`) use correct `bools[N]` indices
- [ ] `_expand_signals_with_bools()` appends bools in correct order

## Common Mistakes

### Wrong Virtual Index Base
```python
# WRONG - assumes bools start at 100
PROTECTION_IS_TRIPPED = 122  # Would miss by 28!

# CORRECT - calculate from actual signal count
PROTECTION_IS_TRIPPED = 94   # 72 raw signals + bit 22
```

### Mismatched Bit Positions
```cpp
// C++ packs in this order:
packet.add_bool_pack(51, {
    modulator_release,      // bit 0
    modulator_force_comm,   // bit 1
    // ...
    is_tripped,            // bit 22
    is_inhibited,          // bit 23
});
```
```python
# Python MUST match:
is_tripped = bools1[22]    # NOT bools1[20] or any other index!
is_inhibited = bools1[23]
```

### Forgetting to Update Both Sides
When adding a new boolean to C++, you MUST:
1. Add it to the bool pack at the correct bit position
2. Update the Python docstring for that pack
3. Update the Python property that reads it
4. Update the virtual index enum if UI needs direct access

## Debugging Tips

1. **Print raw bool pack value**: `print(f"BOOL_PACK_1 raw: 0x{raw:08X}")`
2. **Check individual bits**: `print(f"Bit 23: {(raw >> 23) & 1}")`
3. **Verify signal count**: Check `header.signal_count` matches expected
4. **Compare fiber optic**: If grid shows machine inhibited but machine shows FALSE, the machine-side unpacking is wrong

## File Locations

| Side | C++ Builder | Python Enum | Python Decoder |
|------|-------------|-------------|----------------|
| Grid | `sfc_embedded/src/grid_side_telemetry_builder.cpp` | `monitor_client/config/grid_side_telemetry_protocol.py` | `monitor_client/core/telemetry_protocol.py` (GridSideTelemetryPacket) |
| Machine | `sfc_embedded/src/machine_side_telemetry_builder.cpp` | `monitor_client/config/machine_side_telemetry_protocol.py` | `monitor_client/core/telemetry_protocol.py` (MachineSideTelemetryPacket) |
