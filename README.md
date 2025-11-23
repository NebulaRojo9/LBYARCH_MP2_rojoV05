# Algorithm of the program
The three elements of vector sizes *N* are used as the main runs that loop inside the main C function. Each main run consists of 50 secondary runs. In one secondary run, vectors $X_{1}$, $X_{2}$, $Y_{1},$ and $Y_{2}$ are assigned with *N* randomly generated floating-point values. Then, the kernels C and x86 are used to calculate the Euclidean distances and store the results in their respective *N*-sized vectors, $Z_{C}$ and $Z_{asm}$. Ensuring correctness of results and at the same time, time measured in microseconds (ms) for both kernels from the time of start of their execution to the time they finished all calculations. From accumulation of 50 secondary runs, average time consumed was calculated for each version of kernel and the program loops back again to another main loop corresponding to different vector size until all elements of vector sizes are finished. 

Euclidean distances:

$Z[i] = \sqrt{(X_2[i] - X_1[i])^2 + (Y_2[i] - Y_1[i])^2}, 0 \le i \le 49$

## Comparative Analysis of C
Assuming no other programs running that would delay the time performance.
|    Vector size *N*   | Memory Consumption   |   C ave. time (ms)   | x86-64 ave. time (ms)  |                       Analysis                              |
| -------------------- | -------------------- | -------------------- | ---------------------- | ------------------------------------------------------------|
|   $2^{20}$           |       24.00 MB       |   3.737984           |   3.324152             |       x86-64 kernel is 1.12 times FASTER than C kernel      |
|   $2^{24}$           |      384.00 MB       |   51.352434          |   47.533634            |       x86-64 kernel is 1.08 times FASTER than C kernel      |
|   $2^{28}$           |     6144.00 MB       |   1155.928442        |   809.502740           |       x86-64 kernel is 1.43 times FASTER than C kernel      |

Usinh the algorithm provided above, comparative analysis shows consistency that average time of x86-64 runs faster than C kernel.  


## C Outputs, x86-64 Outputs, and Correctness Checks
### First main run
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/c5b24552-b512-41c7-9dbc-1ec8ddfe2f95" />


### Second main run
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/37ac8331-4032-4ad8-8a2d-97a62b98b167" />



### Third main run
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/233cabf0-4364-4536-b6e1-598a7113fc4c" />



## Video Compilation
Youtube Video LBYARCH_MP2
https://www.youtube.com/watch?v=YSyYjsbZoXw

I forgot to include the configuration of my program files so are the things I used during the demo following the teachings of my instructor.
<img width="1001" height="684" alt="image" src="https://github.com/user-attachments/assets/bc2a8842-9407-4e35-841b-163236c599e7" />
<img width="1001" height="684" alt="image" src="https://github.com/user-attachments/assets/491bfe8b-b95f-4775-bb66-55e9ed638759" />
<img width="1001" height="684" alt="image" src="https://github.com/user-attachments/assets/ceb47acd-84b5-4e38-9a54-03442079799a" />


