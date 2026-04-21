1. In the Unreal Editor: File → New Level → Empty Level.
2. Save as Content/Maps/StartupMap.
3. Add: Player Start, Directional Light, Sky Atmosphere (optional), Exponential Height Fog, large floor Static Mesh with collision.
4. For vehicles: create a Blueprint subclass of GTADriveableVehicle, assign Chaos wheel/body meshes per Epic's wheeled vehicle docs, place in the level.
5. Uncomment GameDefaultMap in Config/DefaultEngine.ini to point at /Game/Maps/StartupMap.
