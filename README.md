# Conway's Game of Life 🎮

![Game of Life Screenshot](./screenshot.png)

Implementation of John Conway's classic cellular automaton in **C++** and **Python**.

## What is it?

A "game" based on 4 simple rules that generate complex patterns:
- A live cell with 2-3 live neighbors survives
- A dead cell with exactly 3 live neighbors is reborn
- A live cell with fewer than 2 live neighbors dies (underpopulation)
- A live cell with more than 3 live neighbors dies (overpopulation)

## C++ (Interactive Version)

Complete implementation with real-time controls.

### Compilation and usage:
```bash
make
./conway <cols> <rows>
# Example: ./conway 100 40
```

### Controls:
- `+/-` → Speed
- `p` → Pause
- `w` → Toggle wrapping edges
- `r` → Restart
- `q` → Quit

## Python (Educational Version)

Simplified code **without interactive controls** to understand the logic without threading/input complexity. Perfect for learning the base algorithms.

### Usage:
```python
python3 conway.py
```

## Academic Purpose

Compare implementations and understand how the same algorithms adapt based on language and requirements.
