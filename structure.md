# Structure

Data
Pipes -> Functions that process data
Pipelines -> Functions that apply pipes
Levels -> Functions that apply pipelines

Pipes may have a state but it should be opaque

### Kinds of object functionalities

- Data: 
  - Structs to hold in vectors
  - Classes to hold in vectors (should I only hold structs for consistency?)
  - Standalone classes
  - Resources:
    - Assets that allow to build data

- Interactions with system:
  - Load
  - Save
  - Allocate memory
  - Free memory
  - Draw

- Processing
  - Factories;
    - Function to create data vectors
  - Pipes:
    - Functions to apply on vectors
  - Pipeline:
    - Update functions, to create pipelines out of pipes
  - Levels:
    - Level functions, to create levels out of pipelines
    - Interactions with system
    - Should define its specific controls (Listen function)
  - Game:
    - Highest level of abstraction, decides which level function to execute based on its own state, calls functions to interact with system
    - Interactions with system

- Systems:
  - A system defines its pipes and pipelines and if needed its data structs and system interactions
  - Systems should be as independent as possible
  - Examples (AI, Animation, Rendering, Status, Attacks, Audio)

- Editors:
  - An editor is just a level

### Classifications

- Core:
  - GameLoop

- Gameplay:
  - Levels:

- Editors

- Gui
- Animation
- Rendering
- Audio
- Status
- Attacks
  
- AI:
  - shadowCast (pipe)
  - astar (pipe)
  - behaviors (components)
  - behaviorTree (components)
  - loop (pipeline)
  - strategies (components)

- Shared:
  - helpers
  - math
  - pch

### Data flow

1. Initialize managers (window, rendering, audio, physics, space)
2. Load components from assets
3. Use assets to build vectors (data)
4. Feed data to pipelines (systems)
