#!/usr/bin/env python3
"""
SelfUpdateManager — read GameRules.md (YAML frontmatter) and patch Config defaults.

Usage (repo root, requires PyYAML *or* falls back to a tiny frontmatter parser):

  python Scripts/SelfUpdateManager.py --project-root "C:/Users/bryan/OneDrive/Desktop/gta"
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any, Dict

try:
    import yaml  # type: ignore
except Exception:  # pragma: no cover
    yaml = None  # type: ignore


def _parse_frontmatter_fallback(block: str) -> Dict[str, Any]:
    """Minimal frontmatter parser when PyYAML is unavailable (nested dicts limited)."""
    rules: Dict[str, Any] = {}
    current: Dict[str, Any] | None = None
    for raw in block.splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        if not line.startswith("-") and line.endswith(":") and " " not in line.replace(":", ""):
            key = line[:-1].strip()
            rules[key] = rules.get(key, {})
            current = rules[key]  # type: ignore[assignment]
            continue
        m = re.match(r"^([A-Za-z0-9_]+):\s*(.+)$", line)
        if not m or current is None:
            continue
        k, v = m.group(1), m.group(2).strip()
        if v.lower() in {"true", "false"}:
            current[k] = v.lower() == "true"
        elif re.fullmatch(r"-?\d+(\.\d+)?", v):
            current[k] = float(v) if "." in v else int(v)
        else:
            current[k] = v.strip("\"'")
    return rules


def read_frontmatter_md(path: Path) -> Dict[str, Any]:
    text = path.read_text(encoding="utf-8")
    if not text.lstrip().startswith("---"):
        return {}
    rest = text.lstrip()[3:]
    end = rest.find("---")
    if end == -1:
        return {}
    fm = rest[:end].strip()
    if yaml is not None:
        data = yaml.safe_load(fm) or {}
        return data if isinstance(data, dict) else {}
    return _parse_frontmatter_fallback(fm)


def patch_default_engine(project_root: Path, engine_cfg: Dict[str, Any]) -> None:
    ini = project_root / "Config" / "DefaultEngine.ini"
    if not ini.exists():
        return

    gravity = engine_cfg.get("default_gravity_z")
    if gravity is None:
        return

    text = ini.read_text(encoding="utf-8")
    if "SelfUpdateManager: default_gravity_z" in text:
        return

    physics = "[/Script/Engine.PhysicsSettings]"
    block = [physics, f"; SelfUpdateManager: default_gravity_z={gravity}", f"DefaultGravityZ={gravity}", ""]
    ini.write_text(text.rstrip() + "\n\n" + "\n".join(block), encoding="utf-8")


def patch_default_game(project_root: Path, wd_cfg: Dict[str, Any]) -> None:
    if not wd_cfg:
        return
    ini = project_root / "Config" / "DefaultGame.ini"
    if not ini.exists():
        return
    marker = "; SelfUpdateManager WorldDirector"
    text = ini.read_text(encoding="utf-8")
    if marker in text:
        return
    snippet = (
        f"\n{marker}\n"
        f"; enabled={wd_cfg.get('enabled')}\n"
        f"; min_env_touch_seconds={wd_cfg.get('min_env_touch_seconds')}\n"
    )
    ini.write_text(text.rstrip() + snippet + "\n", encoding="utf-8")


def apply_with_unreal(project_root: Path, rules: Dict[str, Any]) -> None:
    try:
        import unreal  # type: ignore
    except Exception:
        return

    unreal.log_warning("SelfUpdateManager: Editor session detected — printing rules for validation.")
    unreal.log(json.dumps(rules, default=str))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--project-root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    root: Path = args.project_root
    md = root / "GameRules.md"
    if not md.exists():
        print(f"Missing {md}", file=sys.stderr)
        return 2

    rules = read_frontmatter_md(md)
    if args.dry_run:
        print(json.dumps(rules, indent=2, default=str))
        return 0

    patch_default_engine(root, rules.get("engine") or {})
    patch_default_game(root, rules.get("world_director") or {})
    apply_with_unreal(root, rules)
    print("SelfUpdateManager: completed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
