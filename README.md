# HLA Car Sim 

A C++17 car simulation with real-time WebSocket telemetry. The simulation accepts keyboard commands to control a vehicle (start engine, accelerate, steer, brake) and streams vehicle state (latitude, longitude, heading, speed) to a browser-based map client via WebSocket.

Built with a **multi-threaded architecture**: the main thread handles user input while a dedicated simulation thread runs at 20 Hz (50 ms tick), processing commands from a thread-safe queue, updating vehicle movement each tick, and broadcasting state updates over WebSocket.

## Architecture

The application uses a **producer-consumer pattern** with two threads:

- **Main Thread (Input Producer)**: Blocks on `std::cin`, reads user commands, and enqueues them to a thread-safe queue protected by `std::mutex`
- **Simulation Thread (Consumer)**: Runs a fixed 20 Hz update loop (50 ms timestep), dequeues one command per tick, executes it on the `Car` object, advances position from speed and heading, and broadcasts state via WebSocket

**Synchronization:**
- `std::queue<char>` + `std::mutex` — protects command queue from concurrent read/write
- `std::atomic<bool> running` — allows graceful shutdown without mutex overhead
- `std::thread::join()` — ensures sim thread completes before server shutdown

**Thread Safety:**
- The `Car` object is owned exclusively by the sim thread (no shared access)
- Only the command queue is shared between threads (protected by mutex)
- Main thread reads `running` atomically without locking

## Requirements

- CMake 3.10+
- A C++ compiler with C++17 support (clang++ or g++)
- [ixwebsocket](https://github.com/machinezone/IXWebSocket) (included in `external/`)

## Build

**First time / after changing CMakeLists.txt:**
```bash
./build.sh
```

**Incremental builds (day-to-day development):**
```bash
cmake --build build/
```

The executable is placed in `bin/`.

## Run

**1. Start the simulation:**
```bash
./bin/HLA_Personal_Project
```
The WebSocket server starts on `ws://127.0.0.1:8080` and the sim thread begins its 20 Hz update loop.

**2. Control the car** (in the terminal running the sim):
```
Enter command (h for help): f
Enter command (h for help): w
Enter command (h for help): d
Enter command (h for help): v
Enter command (h for help): q
```

**3. Open the map client** (in a separate terminal or browser):
```bash
open data/map.html
```
The map connects to the WebSocket server and updates continuously from streamed state.

## Execution Flow

1. User enters a command (e.g., `f` for start engine) → main thread enqueues to `commandQueue`
2. At the next 50 ms simulation tick, the sim thread dequeues the command and executes it on the `Car`
3. On every simulation tick, the car advances based on current speed and heading, then the latest state is broadcast over WebSocket
4. When user types `q` → main thread enqueues, then waits on `join()` for sim thread to finish
5. Sim thread processes `q`, sets `running = false`, exits its loop
6. `join()` returns, main thread stops the server and exits

## Commands

| Key | Action |
|-----|--------|
| `f` | Start engine |
| `w` | Accelerate (increase speed by 10 m/s) |
| `a` | Steer left (decrease heading by 10 degrees) |
| `d` | Steer right (increase heading by 10 degrees) |
| `b` | Brake (stop immediately) |
| `i` | Display current speed |
| `v` | Display vehicle state in terminal |
| `o` | Reserved (shown in menu, not currently implemented) |
| `e` | Reserved (shown in menu, not currently implemented) |
| `h` | Show commands |
| `q` | Quit |

## Map Client

`data/map.html` is a Leaflet.js map that connects to the WebSocket server. The simulation thread pushes JSON state at 20 Hz, so the map updates in real time:

- The car is rendered as a **directional SVG icon** that rotates with the vehicle's heading
- The map pans to keep the car in view
- A popup shows current speed and heading

The JSON payload format is:
```json
{ "lat": 53.3498, "lon": -6.2603, "heading": 0.0, "speed": 10.0 }
```

## Project Structure

```
HLA_Personal_Project/
├── CMakeLists.txt        # Build configuration
├── build.sh              # Full clean build script
├── include/              # Header files (Car.h)
├── src/                  # Source files (main.cpp, Car.cpp)
├── external/ixwebsocket/ # WebSocket library
├── bin/                  # Built executable
├── build/                # CMake-generated build files
├── data/                 # Map client (map.html)
├── doc/                  # Documentation
└── lib/                  # Libraries
```
