# Age of Empires II scroll bugfix 

A simple python script to patch Age of Empires II binary to fix stuck scroll bug.

## Prerequisites

You'll need python version 2 or 3 installed on your system.

## Applying the patch

Locate the Age of Empires II binary. It's called *AoK HD.exe* and it's located in one of the following folders:

- Under Windows
    - `C:\Program Files\Steam\SteamApps\Common\Age2HD`
    - `C:\Program Files (x86)\Steam\SteamApps\Common\Age2HD`
    - `D: E: F: ... \SteamLibrary\SteamApps\Common\Age2HD`

- Under Linux
    - `~/.steam/steam/steamapps/common/Age2HD`

Apply the patch issuing the following command in the command line: `python aoe2_scroll_bugfix.py <path to you AoK HD.exe>`

For example if your binary is located under `C:\Program Files (x86)\Steam\SteamApps\Common\Age2HD` then the command you have to issue is: `python aoe2_scroll_bugfix.py "C:\Program Files (x86)\Steam\SteamApps\Common\Age2HD\AoK HD.exe"`

## Reverting the patch

If you encounter any issues which you think is caused by this patch, you can revert it any time by overwriting `AoK HD.exe` with its backup `AoK HD.exe.backup` located next to `AoK HD.exe`.

## Authors

[kukkerman](https://github.com/kukkerman)

## License

This project is licensed under the MIT License.

## Acknowledgments

Many thanks go out to [Sulix](https://steamcommunity.com/id/enmangulator) who discovered the root cause of the bug and wrote a [post](https://steamcommunity.com/app/221380/discussions/2/622954302095447538/#c154645539343670235) about it.