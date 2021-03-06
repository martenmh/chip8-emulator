# Opcode table

xxx = memory address<br>
xx = value<br>
x & y = registers<br>
n = single value<br>

|  Opcode 	| Pseudo C |  Assembly 	| Explanation  	|  Note* 	|
|---	|---	|---  |---	|---	|
|0xxx   	|  |CALL RCA_1802   	|Calls RCA 1802 program at address NNN. Not necessary for most ROMs.    	|   	|
| 00E0   	|  |CLS   	|Clear screen   	|   	|
| 00EE   	| return; |RET   	|Return from subroutine   	|   	|
| Axxx   	| I = $xxx; |SETI $x   	|SET   	|I to the value at memory address xxx |
| Cxnn   	| Vx = rand(0, 255) & nn; |RAND $x  	|Set Vx to the result of a bitwise & operation on a number (0-255) and nn   	|   	|
|1xxx  	|  |JMP $xxx   	|Jump to address xxx   	|   	|
|2xxx 	| xxx(); |CALL $xxx   	|`Call` xxx |Max call stack of *16*, has to be returned from using `00EE`   	|
|Bxxx	| goto $xxx; |JMP $xxx(V0)   	|Jump to address xxx + value of V0   	|   	|
|3yxx 	|  |SKIP.EQ   	|`Skip` next instruction if the value of `Vy == xx` 	|   	|
|4yxx	|  |SKIP.NE   	|`Skip` next instruction if the value of `Vy != xx`  	|   	|
|5yx0	|  |SKIP.EQ   	|`Skip` next instruction if the value of `Vy == Vx`	|   	|
|9yx0	|  |SKIP.NE   	|`Skip` next instruction if the value of `Vy != Vx`	|   	|
|8xy0	|  |MOV Vx, Vy   	|Move Vy into Vx   	|   	|
| 8xy1	|  |OR Vx, Vy   	|OR Vy into Vx   	|   	|
| 8xy2	| Vx &= Vy |AND Vx, Vy   	|AND Vy into Vx    	|   	|
| 8xy3	| Vx ^= Vy |XOR Vx, Vy   	|XOR Vy into Vx   	|   	|
| 8xy4	| Vx += Vy |ADD Vx, Vy   	|Add Vy to Vx   	|VF (Flags register) is set to 1 when there's a carry, and 0 when there isn't|
| 8xy5	| Vx -= Vy |SUB Vx, Vy   	|Subtract Vy to Vx   	|VF (Flags register) is set to 0 when there's a borrow, and 1 when there isn't. 	|
| 8xy6	| Vx >>= 1; |SHR Vx "or" SHR Vx, Vy 	|Shift the value of Vy 1 bit to the right and store in Vx   	|Stores the "removed" bit into VF (Flags register)	|
| 8xy7 	| Vx=Vy-Vx |SUBB Vx, Vy   	|Subtract backwards   	|Alters VF (Flags register)	|
| 8xyE	|  Vx <<= 1; |SHL Vx "or" SHL Vx, Vy   	|Shift the value of Vy 1 bit to the left and store in Vx    	|Stores the "removed" bit into VF (Flags register)	|
|Dxyn   	|  |DISP x, y, n   	|Draw sprite at coordinate (Vx, Vy) with height of N pixels   	|VF (Flags register) is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn't happen   	|
|Ex9E   	|  |   	| Skip the next instruction if the key stored in Vx is pressed  	|   	|
| ExA1  	|  |   	| Skip the next instruction if the key stored in Vx isn't pressed  	|   	|
| Fx07  	| Vx = delay_timer; |   	|Set Vx to the value of the delay timer   	|   	|
| Fx0A  	| Vx = getchar(); |   	|  Wait for key press and store in Vx (All instructions halted until key press) 	|   	|
|Fx15   	| delay_timer = Vx; |   	|Set the delay timer to Vx   	|   	|
| Fx18   	| sound_timer = Vx; |   	|Set the sound timer to Vx   	|   	|
| Fx1e   	| I += Vx; |   	|Add Vx to I (Address register)|VF is set to 1 when there is a range overflow    	|
| Fx29  	|  |   	|Sets I to the location of the sprite for the character in Vx (Characters 0-F hex)   	|   	|
| Fx33   	| I = bcd(Vx); |   	|Stores the binary-coded decimal representation of Vx in the value of I, I+1 and I+2   	|   	|
| Fx55  	|  |   	|Stores V0 to Vx (including Vx) in memory starting at address I.   	|In the original CHIP-8 implementation, and also in CHIP-48, I is left incremented after this instruction had been executed. In SCHIP, I is left unmodified.   	|
| Fx65  	|  |   	|Fills V0 to Vx (including Vx) in memory starting at address I.   	|In the original CHIP-8 implementation, and also in CHIP-48, I is left incremented after this instruction had been executed. In SCHIP, I is left unmodified.   	|
