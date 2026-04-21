GTAWorld — your own training loop (no Unreal Learning Agents plugin)
===================================================================

1) Place AGTATrainingDirector in the level.

2) Assign PolicyClass (optional):
   - Leave empty → built-in UGTACustomTrainingPolicy heuristic (pursuit).
   - Or create Blueprint subclass of UGTACustomTrainingPolicy and override EvaluatePolicy.

3) Register NPCs:
   - Blueprint / C++: Get Training Director → Register Agent with each ABaseAgent (or call from NPC BeginPlay).

4) Observation = 8 floats (see ABaseAgent::AppendTrainingObservation).
   Action = 3 floats: strafe, forward, yaw (degrees), matching GTAWorldTraining::DefaultActionDim.

5) UAgentBrainComponent::SetTrainingDirectorActor is optional; RegisterAgent on the director does this for you.

6) Ship build: disable or stop ticking AGTATrainingDirector if you only want scripted/BT AI.
