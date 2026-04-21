# GTAWorld — phased roadmap to launch

This repository uses **Epic’s engine only** (no Unreal Learning Agents plugin). All training / policy code lives in the **`GTAWorld`** module: `AGTATrainingDirector`, `UGTACustomTrainingPolicy`, `ABaseAgent`, `UAgentBrainComponent`.

---

## Folder layout (training)

```
Source/GTAWorld/Public/
├── GTAWorldTrainingLayout.h       # Observation/action dimensions (single source of truth)
├── GTACustomTrainingPolicy.h      # Your policy — subclass in BP/C++ and plug into director
├── GTATrainingDirector.h          # Level actor: obs → policy → movement
├── AgentBrainComponent.h
└── BaseAgent.h
```

## Class hierarchy (training)

- `AGTATrainingDirector` — ticks on `StepSeconds`; calls `AppendTrainingObservation` → `UGTACustomTrainingPolicy::EvaluatePolicy` → `ApplyPolicyActionVector`.
- `UGTACustomTrainingPolicy` — default planar pursuit heuristic; **replace** with your net / rules.
- `ABaseAgent` — packs **8** floats (see `GTAWorldTraining::DefaultObservationDim`); applies **3** floats (strafe, forward, yaw °).
- `UAgentBrainComponent` — reward EMA; optional bind to the same `AGTATrainingDirector` you register with.

---

## Phase 2 — Your own training loop (in project)

1. Place **`AGTATrainingDirector`** in the level (or spawn at runtime).
2. Set **`PolicyClass`** to a Blueprint child of `UGTACustomTrainingPolicy` if you want custom logic; otherwise the default C++ heuristic is used.
3. Call **`RegisterAgent`** for each `ABaseAgent` you want driven (or bind from BeginPlay on the NPC).
4. Tune **`StepSeconds`** (e.g. `0.08`–`0.15`). Disable the director in shipping if NPCs use full scripted AI instead.

See `Content/Training/README.txt`.

---

## Phase 0 / 1 / 3

Unchanged from before: open project in UE 5.4, build editor target, create startup map, PIE gameplay (movement, shoot, vehicle, World Director, dynamic view distance).

---

## GameRules / Python

`Scripts/SelfUpdateManager.py` only patches config markers from `GameRules.md` — unrelated to external ML plugins.

---

## Notes

- **ChaosVehicles** + **EnhancedInput** remain optional engine plugins; no third-party AI plugins are required for training.
- To train with gradients / files, add **your** inference library or file format inside a **subclass of `UGTACustomTrainingPolicy`** and keep `GTAWorldTrainingLayout` in sync with `ABaseAgent::AppendTrainingObservation`.
