#!/usr/bin/env python3
from collections import defaultdict
import os
import time
from pathlib import Path
import sys
from subprocess import check_call, CalledProcessError

THIS_DIR = Path(__file__).absolute().parent


def reset() -> None:
    check_call(["pebble", "kill"])
    check_call(["pebble", "wipe"])


def run(platforms: list[str]) -> int:
    reset()
    minute = 0
    screenshot_fnames = defaultdict(list)
    while minute < 60:
        env = os.environ.copy()
        env["CFLAGS"] = f"-DHOUR_OVERRIDE=8 -DMINUTE_OVERRIDE={minute}"
        check_call(["pebble", "build"], env=env)
        for platform in platforms:
            try:
                check_call(["pebble", "install", "--emulator", platform])
                time.sleep(0.2)
                screenshot_fname = f"screenshot_{platform}_08{minute:02d}.png"
                check_call(["pebble", "screenshot", "--emulator", platform, screenshot_fname])
                screenshot_fnames[platform].append(screenshot_fname)
                minute += 1
            except CalledProcessError:
                # Sometimes install fails and I don't know why (real watch and emulator both)
                # Sometimes screenshot fails and I don't know why (emulator only?)
                reset()
    reset()

    # Combine all the pngs into one gif with ImageMagick
    # https://joeldare.com/combining-multiple-images-into-an-animated-gif-with-imagemagick
    for platform, fnames in screenshot_fnames.items():
        check_call(
            [
                "convert",
                "-delay",
                "80",
                "-loop",
                "0",
                "-dispose",
                "previous",
                *fnames,
                f"screenshots_{platform}_08xx.gif",
            ]
        )

    return 0


def main() -> int:
    return run(["basalt", "emery", "flint"])


if __name__ == "__main__":
    sys.exit(main())
