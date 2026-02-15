# DREMEL

**DREMEL** (Desktop Rhythms & Energy Music Engine Launcher) is a
lightweight background music daemon for Linux desktops.\
It plays music from a directory and automatically pauses when specified
applications are running.

------------------------------------------------------------------------

## Files

This repository includes three main components:

-   **cdremel** - Core program responsible for playback, rule handling,
    exclusions, and options
-   **dremel** - Launcher that runs `cdremel` in the background
-   **dremel-kill** - Stops all running `cdremel` instances

------------------------------------------------------------------------

## Getting Started (Linux)
### remember to add ~/.local/bin to your path 

Clone the repository:

``` bash
git clone https://github.com/howtoedittv/dremel.git
cd dremel
```

Build and install:

``` bash
make install
```
Initialize(no mpv installed on system):

```bash
cdremel init
Move Your Songs To ~/Music/background
```
Initialize(mpv installed on system):

```bash
Mkdir ~/Music/background
Move Your Songs To ~/Music/background
```
Runing:

``` bash
dremel
```

If You Happen To Close the terminal --- it will keep running.

To Stop playback(Run In Terminal):

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
cdremel --add-exclusion vlc //Added Vlc To The List Of Apps That Music Is Silent In
cdremel --remove-exclusion vlc //Removed Vlc From The List Of Apps That Music Is Silent In
cdremel -i #Ignore All Exclusions
```

------------------------------------------------------------------------

## Behavior

-   Runs without a terminal window
-   Respects application exclusion rules
-   Automatically resumes when excluded apps close
-   Can be stopped with `dremel-kill`

------------------------------------------------------------------------

## Media

[ Click to watch (with sound)](https://github.com/howtoedittv/dremel/releases/download/usermedia/showing.mp4)

[ Click to view ] (https://github.com/howtoedittv/dremel/releases/download/usermedia/showing.png)

if you desire to get sample music (some caribou songs) you can do the following commends:
```bash
cd ~
git clone https://github.com/howtoedittv/dremel-sample-music
rm -rf Music/background
mv dremel-sample-music/background Music/
```

## License

MIT License © 2026 howtoedittv
