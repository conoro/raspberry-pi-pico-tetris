This is not my code. It is just a copy of the code referenced [here on Reddit](https://www.reddit.com/r/raspberry_pi/comments/l6flip/a_very_tiny_game_of_tetris_my_first_test_of_the/) with minor changes to make it compile on Windows. I copied an existing Pimoroni project to get all the build files and settings.

If you follow the Raspberry Pi Pico [Getting Started PDF](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf) for Windows, then this should build by then following the [Pimoroni instructions](https://github.com/pimoroni/pimoroni-pico/blob/main/setting-up-the-pico-sdk.md) but replacing the last two steps with:

```
cmake -G "NMake Makefiles" .. 
nmake
```

