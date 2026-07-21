# ChargeLab

ChargeLab is an interactive charged-particle simulator written in C++ and compiled to WebAssembly for the browser. It lets users add, select, drag, edit, and fix charged particles while visualizing electric-force interactions in real time.

The project began as a C++/SFML desktop physics simulator and was later refactored so the core simulation engine could run on the web through Emscripten and WebAssembly.

## Features

- Real-time charged-particle motion
- Positive and negative charge interactions
- Coulomb-force based attraction and repulsion
- Force-vector visualization for selected particles
- Click-to-place positive and negative particles
- Select and drag particles
- Pause, clear, and reset demo controls
- Fixed particles that stay in place while still affecting other particles
- Selected-particle editor for:
  - position
  - charge
  - mass
  - radius
  - velocity
- Live selected-particle stats panel

## Tech Stack

- C++
- SFML for the desktop version
- Emscripten
- WebAssembly
- JavaScript
- HTML Canvas
- HTML/CSS

## Project Structure

```txt
chargeLab/
├── src/
│   ├── Particle.h
│   ├── Particle.cpp
│   ├── Simulation.h
│   ├── Simulation.cpp
│   ├── Vector2D.h
│   ├── Vector2D.cpp
│   ├── SFMLRenderer.h
│   └── SFMLRenderer.cpp
├── web/
│   ├── index.html
│   ├── style.css
│   ├── app.js
│   ├── bindings.cpp
│   ├── chargeLab.js
│   └── chargeLab.wasm
├── main.cpp
├── Makefile
└── README.md