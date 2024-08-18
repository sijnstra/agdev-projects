# UE14500
This is a compile (not much porting required - most features work as-is) of the Usagi Electric valve computer. The Usagi Electric Vacuum Tube Computer, or UEVTC, is a vacuum tube computer that is heavily inspired by the Motorola MC14500 1-bit ICU. The entire computer will work on a 1-bit data bus, however the control word will be 12-bits - 4-bit instruction, 2-bit memory banking, 6-bit memory address.


Please see the following pages:
* Project description: https://github.com/Nakazoto/UEVTC/wiki
* Original file repository: https://github.com/Nakazoto/UEVTC

# Provided files
The files include:
* ue14500-asm.bin - an agon .bin file, providing an assembler for compiling UE14500 code (Thanks TurBoss! and thanks for the tip on this project in the first place)
* us14500-emu.bin - an agon .bin file, providing the actual emulator of the machine
* hello.emu - a compiled test program
* hello1.emu - same program with a small delay so you can see it running.

for both bin files you can prove an optional input file followed by an optional output file.


