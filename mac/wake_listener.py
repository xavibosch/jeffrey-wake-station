#!/usr/bin/env python3
import argparse
import subprocess
from pathlib import Path

import serial


ROUTINES = {
    "MOTION": "focus.applescript",
    "SOUND": "music.applescript",
    "BUTTON": "build.applescript",
}


def run_routine(name: str) -> None:
    script = Path(__file__).parent / "routines" / ROUTINES[name]
    subprocess.run(["osascript", str(script)], check=False)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", required=True, help="Arduino serial port")
    parser.add_argument("--baud", type=int, default=115200)
    args = parser.parse_args()

    with serial.Serial(args.port, args.baud, timeout=1) as connection:
        while True:
            raw = connection.readline().decode("utf-8", errors="ignore").strip()
            event = raw.partition(",")[0]
            if event in ROUTINES:
                run_routine(event)


if __name__ == "__main__":
    main()
