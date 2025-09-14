# Aptomodo — C++ Roguelike Game Engine

Aptomodo is a custom **game engine written in C++**. It bundles everything you need to build a **classic roguelike** with modern tooling: an **ECS** core (powered by **EnTT**), behavior‑tree AI, OpenGL/Raylib rendering and animation, audio, combat, robust pathfinding with shadow‑casting visibility, in‑engine editors, and real‑time diagnostics via Dear ImGui.

<p align="center">
  <img alt="Aptomodo" src="demos/demo.gif" width="1080"/>
  </p>

  ---

## ✨ Highlights

* **Entity‑Component‑System (ECS)** architecture using **EnTT** for data‑oriented design and high performance.
* **AI with Behavior Trees** for readable, extensible decision‑making.
* **Rendering + Animation** via **OpenGL**/**Raylib**, with timeline/sprite/flipbook control.
* **Audio** system for music and SFX management.
* **Combat** framework with hitboxes/hurtboxes, damage, invulnerability frames, and events.
* **Pathfinding** using **A\*** ("A‑star") and **shadow‑casting** for field‑of‑view & wall occlusion.
* **Attribute system** with multiple subttributes mutually influenced, that can be modified with items and drops
* **Dear ImGui** tooling: real‑time diagnostics and live configuration.
* **Built‑in Editors**:

  * 🎞️ **Animation Editor** — import from **Blender** and author playback.
  * 👾 **Enemy Editor** — define enemies and behaviors quickly.
  * 🗡️ **Attack Editor** — author attack bounding boxes visually.

      ---

## 🧱 Architecture at a Glance

Aptomodo is organized around **ECS** with EnTT:

```
[ Systems ]  ──►  operate on  ──►  [ Entities + Components ]
     ▲                                   ▲
          │                                   │
           [ Events ]  ◄────────────── publish/subscribe ──────────────┘
           ```

           * **Entities**: lightweight IDs.
           * **Components**: plain data (e.g., `Position`, `Velocity`, `Sprite`, `Health`, `AIState`, `HitBox`).
           * **Systems**: pure logic, stateless where possible (`PhysicsSystem`, `RenderSystem`, `AISystem`, `CombatSystem`, `AudioSystem`, `PathSystem`).
           * **Events**: decouple subsystems (`AttackLanded`, ÈntityDied`, `Footstep`, `ConfigChanged`).

### Minimal ECS example (C++)

```cpp
#include <entt/entt.hpp>

struct Position { float x{}, y{}; };
struct Velocity { float dx{}, dy{}; };

void physics(entt::registry& r, float dt) {
      auto view = r.view<Position, Velocity>();
        for (auto e : view) {
                auto& p = view.get<Position>(e);
                    const auto& v = view.get<Velocity>(e);
                        p.x += v.dx * dt; p.y += v.dy * dt;
                          }
}
```

---

## 🧠 AI — Behavior Trees

Behavior Trees keep AI logic modular and testable. Typical nodes include **Selector**, **Sequence**, **Decorator**, and **Leaf** actions.

* Author trees in data (TOML) or code; reuse leaf actions across enemies.
* Integrates with the **Enemy Editor** to preview decisions and tweak parameters live.

---

## 🎨 Rendering & Animation (OpenGL / Raylib)

* 2D rendering pipeline built on **Raylib** with **OpenGL** backend.
* Sprite, tileset, and flipbook animation control (play, pause, blend, speed, events).
* Camera controls, layers, and debug overlays.


---

## 🔊 Audio

* Music/SFX with channel groups and volume buses.
* Event‑driven triggers (e.g., `Footstep`, `AttackLanded`).
* Optional ducking during dialog/critical cues.

---

## ⚔️ Combat

* **Hitbox/Hurtbox** components with spatial queries.
* Damage resolution, iframes, knockback, and status effects hooks.
* Author attacks quickly with the **Attack Editor**; export to data files for reuse.

---

## 🧭 Pathfinding & Visibility

* **A\*** grid/path graph with heuristic tuning and dynamic costs.
* **Shadow‑casting** FOV for true roguelike visibility and wall occlusion.
* Integrates with AI line‑of‑sight checks and ranged combat.

---

## 🧰 Tooling — Dear ImGui

* **Diagnostics**: frame time, ECS counts, memory, logger, and hot‑reload counters.
* **Live Configuration**: tweak gameplay, rendering, and AI params at runtime.
* Panels for **Editors** (Animation/Enemy/Attack) with undo/redo and export.

---

## 🎛️ Editors

### Animation Editor (Blender → Engine)

* Import animations authored in **Blender**.
* Supported flows: sprite sheet strips, image sequences, or glTF/FBX baked exports (replace with your exact flow).
* Set frame ranges, playback rate, events (e.g., footstep), and export as JSON.

### Enemy Editor

* Compose enemies from components: stats, behavior tree, sprite set, drops.
* Live preview of patrol/chase/attack logic.

### Attack Editor

* Visual authoring of **attack bounding boxes** over frames.
* Define damage, hitstun, knockback, and tags.

---

### Credits

* **C++20** toolchain (GCC, CMake)
* **Raylib** (OpenGL)
* **EnTT** (ECS)
* **Dear ImGui**

