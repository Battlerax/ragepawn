# ragepawn
Pawn language implementation around RAGE:MP (A terrible idea) based on CompuPhase's source code versioned "4.0.5749" dated as of "18-10-2017" (dd-MM-yyyy) as such it is incompatible with SA-MP's outdated pawn (3.2.3664).

The goal of RagePawn is to allow those who wish to use the Pawn language create their own resources for the advanced RAGE Multiplayer platform while keeping the API as close to SA-MP's API as possible as such to keep those familiar with it spend less time reading changes.

## Getting Started

If you're coming from a SA-MP-ish community (most likely), then you'll find this pretty straight forward to use apart from the fact that some of the API is a bit different.

Before we dive into coding, you will have to download and install the PAWN Compiler from CompuPhase's official website:
https://www.compuphase.com/pawn/pawn.htm#DOWNLOAD_WIN

So, let's say you want to output "Hello World!" on console, your hello.pwn file should pretty much contain the following:
```
#include <ragepawn>

main() {
    print("Hello world!\n");
}
```

## Deployment

```
pawncc -C64 hello.pwn
```

Copy your output 'hello.amx' to 'server-files/amx/filterscripts' folder and launch the server!

The result should be:
```
...
Initializing RagePawn..
-> Loading filterscripts..
--> Loading 'script.amx'..
Hello world!

-> Loading gamemodes..
[P] Plugin RagePawn.dll loaded!
...
```

# Issues
Asides from the fact that this is still a work in progress, Linux support is most likely not happening because of Pawn's incomplete 64bit implementation.

## Contributors
* Adam
* Austin

Special thanks to Y-Less for Windows 64bit fixes and those who have helped narrowing down BUGS!?@%)_&

## Acknowledgments

* The glorious RAGE Multiplayer's SDK was used, cause how would we achieve this without it?
* We sinned and looked into SA-MP's leaked code for inspiration *giggles*, copy-paste was not used.
* Pawn is outdated.
