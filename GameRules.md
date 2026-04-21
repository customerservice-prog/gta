---
# Machine-friendly block (parsed by Scripts/SelfUpdateManager.py)
engine:
  default_gravity_z: -980
  view_distance_quality: 2
scalability:
  overall_cap: 3
world_director:
  enabled: true
  min_env_touch_seconds: 2.0
custom_training:
  # GTAWorld-native stack (AGTATrainingDirector / UGTACustomTrainingPolicy) — not a third-party plugin
  default_step_seconds: 0.1
  default_observation_dim: 8
  default_action_dim: 3
neural_rendering:
  bridge_enabled: false
---

## Human notes

- Policy and observation layout are defined in code (`GTAWorldTrainingLayout.h`, `ABaseAgent::AppendTrainingObservation`).
- Tweak **world_director** while profiling combat pacing.
