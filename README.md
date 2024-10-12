# BoluDOS
The words "boludo" and "pelotudo" have their origin in the gaucho war, when the common soldiers were sent to fight against the raiders, the boludo was sent first, who handled a boleadora made of three or four balls the size of a tennis ball, and in second place went the pelotudo, with a boleadora made of two larger balls. This is what I have always heard and the veracity of this should be verified. It is assumed that in "La Guerra Gaucha" by Leopoldo Lugones, there is a reference to this.

BoluDOS' name is a combination of the words "boludo" and "DOS", or "Disk Operating System", as it is a [ComputerCraft operating system](https://www.egr.msu.edu/~bruienne/on-writing-an-os.pdf) which can interact with the disk of the machine.

The repo is called "brainfuckOS" because this OS is based around the popular esolang "Brainfuck". The source code of the kernel/interpreter is written in Lua, with the OS being written in C, which is compiled to Brainfuck using [c2bf](https://github.com/arthaud/c2bf).

Currently, the OS is *not really* an operating system, as it still runs on top of CraftOS, but in the future a bios will be made to run using [UnBios](https://gist.github.com/MCJack123/42bc69d3757226c966da752df80437dc) so it can be a completely independent OS.

The OS file structure is: (There is currently no installer to set it up)
```
.
└── /
    ├── bfroot/ # Root of the system
    │   └── bin/ # Commands that can be called anywhere (cd and ls are integrated into the shell directly)
    │       ├── append.bf
    │       ├── cp.bf
    │       ├── del.bf
    │       ├── mkdir.bf
    │       ├── mv.bf
    │       ├── read.bf
    │       ├── screenfetch.bf
    │       └── write.bf
    ├── bfk.lua # Brainfuck interpreter / kernel
    └── main.bf # Main shell
```
The Brainfuck side of the OS interacts with the CC side by using syscalls. Those are described on this wiki page: https://piguman3.github.io/boludos/syscalls.html

To use the OS, replicate the file structure, copying all the files to a CC compatible system, and run `bfk main.bf` to launch the main shell.
