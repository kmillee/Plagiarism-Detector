# Plagiarism-Detector
This C code analyses the similarities between programs written in C language and computes a resemblance rate between each pair. This code was created in the context of a final project in our "C Programming" class.

## GETTING STARTED

### Prerequisites
* Have a C compiler.

### Installation & use
To install Assembly-code-executor, follow these steps:

#### 1. Clone the Repository
Open the terminal:
```sh
git clone https://github.com/MenSouf07/Assembly-code-executor.git 
cd Assembly-code-executor
```
#### 2. Compile the Program
To compile the program, execute the following command in the terminal:
```sh
gcc simulator.c -o simulator
```
To run a <i>pgm.txt</i> source file, write to the terminal ():
```sh
./simulator acces/path/pgm.txt        #Linux
simulator.exe C:\acces\path\pgm.txt   #Windows
```
Make sure to replace "access/path/pgm.txt" (respectively "C:\access\path\pgm.tx") with the actual path of your <i>pgm.txt</i> file.

The <i>hexa.txt</i> file will contain the translation of the <i>pgm.txt</i> file into machine code. the execution when it will be done in the terminal

## WARNING
### Details
To properly work, the files in input need to compile correctly. If they don't (<i>e.g.></i> a missing ';'), a result will be displayed, but it may not be accurate. 

At least two files need to be present as inputs. It doesn't have a theoretical maximal number of files to compare. 
At the execution, the terminal prints the distance (calculated with the Dice method) between every pair of files. 
Finally, if there are more than two files, it displays a matrix indicating the distance between each couple of algorithms. 
During execution, the program creates three PGM files per comparison, containing matrices of similarities between all the lines. The darker a pixel is, the more different the 
lines of code are. 

//reste Mendel
* If a text file already has the name “copy” in your directory, it will be deleted at runtime.
* The maximum label size is set at 300, to change it, you can change the defined N value
on line 29.
* In the event of a syntax error in the source file (incorrect line structure, unknown instruction, etc.),
the machine language file “hexa.txt” will not be created and the first erroneous line detected will be
indicated on the terminal.
* This program does not necessarily support logic errors that may occur in
the source file: it was designed to stop in the event of a system error (i.e. access out of memory, jmp out of
program…) It is therefore the user's responsibility to avoid writing an infinite program or
to write a halt instruction, for example.


## AUTHORS & CONTACTS
Don't hesitate to contact us for more information :
* Adam Kaufman alias [@ockedman](https://github.com/ockedman)
  * mail : adam.kaufman@dauphine.eu
* Camille Jouanet alias [@kmillee](https://github.com/kmillee)
  * mail : jouanet.camille@gmail.com / camille.jouanet@dauphine.eu
  * [Linkedin](https://fr.linkedin.com/in/camillejouanet)

 Thank you for taking interest in our work :)
