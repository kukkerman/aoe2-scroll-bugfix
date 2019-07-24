# Age of Empires II scroll bugfix 

A simple tool to patch Age of Empires II binary to fix stuck scroll bug.

## Prerequisites

Under windows you can simply download the prebuilt binary. In this case you won't need any additional software
installed.

If you're planning to use the python variant, then
you'll need _Python version 2_ or _3_ installed on your system.

If you're fan of C++, then you'll need a C++ compiler which supports the C++14 standard (for example GCC v5.0
and above).

## Downloading the windows binary

- Download the most recent zipped binary located under the `releases` folder.
- **_Always_** check the checksums of the downloaded file before doing anything else, especially if it's  from an alternate source. You can find the correct checksums
in a file with the same name but with the `.checksum.txt` extension in the `releases` folder. For example, under windows you can generate
the appropriate checksums of `aoe2_scroll_bugfix_v0.1.zip` with the following commands from the command line:
 - `certutil -hashfile aoe2_scroll_bugfix_v0.1.zip SHA1`
 - `certutil -hashfile aoe2_scroll_bugfix_v0.1.zip SHA256`
 - `certutil -hashfile aoe2_scroll_bugfix_v0.1.zip SHA512`

## Compiling the C++ source

If you opt to use the C++ source code you have to compile it first.

- Under windows (using Visual C from developer command prompt):
`cl aoe2_scroll_bugfix.cpp /O2 /EHsc`
- Under linux (using GCC):
`g++ aoe2_scroll_bugfix.cpp -std=c++14 -O2 -o aoe2_scroll_bugfix`

## Applying the patch

Locate the Age of Empires II binary. It's called *AoK HD.exe* and it's located in one of the following folders:

- Under Windows
    - `C:\Program Files\Steam\SteamApps\Common\Age2HD`
    - `C:\Program Files (x86)\Steam\SteamApps\Common\Age2HD`
    - `D: E: F: ... \SteamLibrary\SteamApps\Common\Age2HD`

- Under Linux
    - `~/.steam/steam/steamapps/common/Age2HD`

Apply the patch issuing the following command in the command line:

- using the windows binary:
`aoe2_scroll_bugfix.exe <path to you AoK HD.exe>`
- using the linux binary:
`./aoe2_scroll_bugfix <path to you AoK HD.exe>`
- using the python script: `python aoe2_scroll_bugfix.py <path to you AoK HD.exe>`


For example if your binary is located under `C:\Program Files (x86)\Steam\SteamApps\Common\Age2HD` then the command
you have to issue under windows is:
`aoe2_scroll_bugfix.exe "C:\Program Files (x86)\Steam\SteamApps\Common\Age2HD\AoK HD.exe"`

## Reverting the patch

If you encounter any issues which you think is caused by this patch, you can revert it any time by overwriting
`AoK HD.exe` with its backup `AoK HD.exe.backup` located next to `AoK HD.exe`.

## Authors

[kukkerman](https://github.com/kukkerman)

## License

This project is licensed under the MIT License.

## Acknowledgments

Many thanks go out to [Sulix](https://steamcommunity.com/id/enmangulator) who discovered the root cause of the bug
and wrote a [post](https://steamcommunity.com/app/221380/discussions/2/622954302095447538/#c154645539343670235)
about it.