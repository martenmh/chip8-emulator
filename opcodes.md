# Opcode table

xxx = memory address
xx = value
x & y = registers
n = single value

|  Opcode 	|  Assembly 	| Explanation  	|  Note* 	|
|---	|---	|---	|---	|
|00E0   	|CLS   	|Clear screen   	|   	|
|00EE   	|RET   	|Return from subroutine   	|   	|
|1xxx  	|JMP $xxx   	|Jump to address xxx   	|   	|
|2xxx 	|CALL $xxx   	|`Call` xxx |Max call stack of *16*, has to be returned from using `00EE`   	|
|Bxxx	|JMP $xxx(V0)   	|Jump to address xxx + value of V0   	|   	|
|3yxx 	|SKIP.EQ   	|`Skip` next instruction if the value of `Vy == xx` 	|   	|
|4yxx	|SKIP.NE   	|`Skip` next instruction if the value of `Vy != xx`  	|   	|
|5yx0	|SKIP.EQ   	|`Skip` next instruction if the value of `Vy == Vx`	|   	|
|9yx0	|SKIP.NE   	|`Skip` next instruction if the value of `Vy != Vx`	|   	|

|8xy0	|MOV Vx, Vy   	|Move Vy into Vx   	|   	|
|8xy1	|OR Vx, Vy   	|OR Vy into Vx   	|   	|
|8xy2	|AND Vx, Vy   	|AND Vy into Vx    	|   	|
|8xy3	|XOR Vx, Vy   	|XOR Vy into Vx   	|   	|
|8xy4	|ADD Vx, Vy   	|Add Vy to Vx   	|VF (Flags register) is set to 1 when there's a carry, and 0 when there isn't|
|8xy5	|SUB Vx, Vy   	|Subtract Vy to Vx   	|VF (Flags register) is set to 0 when there's a borrow, and 1 when there isn't. 	|
|8xy6	|SHR Vx "or" SHR Vx, Vy 	|Shift the value of Vy 1 bit to the right and store in Vx   	|Stores the "removed" bit into VF (Flags register)	|
|8xy7 	|SUBB Vx, Vy   	|Subtract backwards `Vx=Vy-Vx`   	|Alters VF (Flags register)	|
|8xyE	|SHL Vx "or" SHL Vx, Vy   	|Shift the value of Vy 1 bit to the left and store in Vx    	|Stores the "removed" bit into VF (Flags register)	|

|Dxyn   	|DISP x, y, n   	|Draw sprite at coordinate (Vx, Vy) with height of N pixels   	|VF (Flags register) is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn't happen   	|
|Ex9E   	|   	| Skip the next instruction if the key stored in VX is pressed  	|   	|
| ExA1  	|   	| Skip the next instruction if the key stored in VX isn't pressed  	|   	|

| Fx07  	|   	|Set Vx to the value of the delay timer   	|   	|
| Fx0A  	|   	|  Wait for key press and store in Vx (All instructions halted until key press) 	|   	|
|Fx15   	|   	|Set the delay timer to Vx   	|   	|
|Fx18   	|   	|Set the sound timer to Vx   	|   	|
|Fx1e   	|   	|Add Vx to I (Address register) `I += Vx`|VF is set to 1 when there is a range overflow    	|
| Fx29  	|   	|Sets I to the location of the sprite for the character in Vx (Characters 0-F hex)   	|   	|
|Fx33   	|   	|Stores the binary-coded decimal representation of Vx in the value of I, I+1 and I+2   	|   	|
| Fx55  	|   	|Stores V0 to Vx (including Vx) in memory starting at address I.   	|In the original CHIP-8 implementation, and also in CHIP-48, I is left incremented after this instruction had been executed. In SCHIP, I is left unmodified.   	|
| Fx65  	|   	|Fills V0 to Vx (including Vx) in memory starting at address I.   	|In the original CHIP-8 implementation, and also in CHIP-48, I is left incremented after this instruction had been executed. In SCHIP, I is left unmodified.   	|
