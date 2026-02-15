# DREMEL

**DREMEL** (Desktop Rhythms & Energy Music Engine Launcher) is a
lightweight background music daemon for Linux desktops.\
It plays music from a directory and automatically pauses when specified
applications are running.

------------------------------------------------------------------------

## Files

This repository includes three main components:

-   **cdremel** -- Core program responsible for playback, rule handling,
    exclusions, and options\
-   **dremel** -- Launcher that runs `cdremel` in the background\
-   **dremel-kill** -- Stops all running `cdremel` instances

------------------------------------------------------------------------

## Getting Started (Linux)

Clone the repository:

``` bash
git clone https://github.com/howtoedittv/dremel.git
cd dremel
```

Build and install:

``` bash
make install
```

Run in background:

``` bash
dremel
```

Close the terminal --- it will keep running.

Stop playback:

``` bash
dremel-kill
```

------------------------------------------------------------------------

## Music Directory

Music files should be placed in:

``` text
~/Music/background
```

All supported audio files in this directory will be played sequentially.

------------------------------------------------------------------------

## Options (cdremel)

    -i                     Ignore all rules and play continuously
    --add-exclusion APP    Pause when APP is running
    --remove-exclusion APP Remove an exclusion
    --help                 Show help
    init                   Install dependencies and create folders

Example:

``` bash
cdremel --add-exclusion brave
cdremel -i
```

------------------------------------------------------------------------

## Behavior

-   Runs without a terminal window
-   Respects application exclusion rules
-   Automatically resumes when excluded apps close
-   Can be stopped with `dremel-kill`

------------------------------------------------------------------------

## License

MIT License © 2026 howtoedittv
