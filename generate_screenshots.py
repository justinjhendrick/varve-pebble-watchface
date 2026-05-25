#!/usr/bin/env python3
import os
import time
from pathlib import Path
import sys
import json
from subprocess import check_call, CalledProcessError

THIS_DIR = Path(__file__).absolute().parent

def main() -> int:
    with open(THIS_DIR / "package.json", mode="r") as f:
        package = json.load(f)
    for minute in [21, 23]:
        env = os.environ.copy()
        env["CFLAGS"] = f"-DHOUR_OVERRIDE=8 -DMINUTE_OVERRIDE={minute}"
        check_call(["pebble", "build"], env=env)
        check_call(["pebble", "install", "--emulator", "emery"])
        time.sleep(0.1)
        check_call(["pebble", "screenshot", "--emulator", "emery", f"screenshot_emery_08:{minute:02d}.png"])
    return 0

if __name__ == "__main__":
    sys.exit(main())
