# terminal-sudoku-in-c
sudoku game written in c that can be executed via terminal:)

### >issues/important to note: 

1. i tried dividing final program into few files with headers for better structure, but for some reason i get error trying to compile them with gcc and i couldn't figure out the reason since i was short on time:( 
,but i will look into it and fix it (i speculate it to be an issue with time library? ) the attempted divided version is located in the /src directory, but since it doesn't compile 
the whole working code is in one file - main.c, and i do apologise for that and hope it's readable 🙏 
2. instead of allowing the user to delete a previously placed number program counts their wrong moves (up to 3 then game ends), since i thought that would be more intuitive gameplay 
with less complications and all mobile sudoku games i've tried had this functionality 

all complex functions are explained in [code_explanation.md](https://github.com/werixx1/terminal-sudoku-in-c/blob/main/code_explanation.md) :)
<br/>execute: ```gcc main.c -o sudoku``` then ```./sudoku```

---
### >program features:
- three board sizes: 4x4, 9x9 and 16x16
- four difficulty levels
- save/load game
- boards are randomly generated
- visible gameplay time after finishing

### >gameplay preview:
![gameplay](https://github.com/user-attachments/assets/59f65636-7bfa-49cc-846a-fec3ce27403b)
#### board 9x9
![Zrzut ekranu 2025-04-29 172112](https://github.com/user-attachments/assets/df1b16db-eb72-4778-8b3d-ee636041e9e8)
#### board 16x16
![Zrzut ekranu 2025-04-29 172128](https://github.com/user-attachments/assets/875b8a26-7d6a-4949-94cc-07866e217dc9)
#### displayed solution after losing:
![Zrzut ekranu 2025-04-29 172244](https://github.com/user-attachments/assets/17f525ba-d5d6-44cb-b735-ccd848e06472)

### >usefull resources:
- algorithm for sudoku generation: https://www.geeksforgeeks.org/program-sudoku-generator/
- difftime() function: https://www.tutorialspoint.com/c_standard_library/c_function_difftime.htm
- function for time-delay: https://www.geeksforgeeks.org/time-delay-c/
- handling files: https://www.w3schools.com/c/c_files_read.php and https://www.youtube.com/watch?v=P-fWNCF7Wx8

![siemka](https://github.com/user-attachments/assets/4632c04c-4cd4-4115-991e-ca141d679aad)





