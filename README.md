# Plagiarism-Detector
This C code analyses the similarities between programs written in C language and computes a resemblance rate between each pair. This code was created in the context of a final project in our "C Programming" class.

## GETTING STARTED

### Prerequisites
* Have a C compiler.

### Installation & use
To install Plagiarism-Detector, follow these steps:

#### 1. Clone the Repository
Open the terminal:
```sh
git clone https://github.com/kmillee/Plagiarism-Detector.git 
cd Plagiarism-Detector
```
#### 2. Compile the Program
To compile the program, execute the following command in the terminal:
```sh
gcc simulator.c -o simulator
```
To run a <i>file.txt</i> source file, write to the terminal ():
```sh
./simulator acces/path/file.txt        #Linux
simulator.exe C:\acces\path\file.txt   #Windows
```
Make sure to replace "access/path/pgm.txt" (respectively "C:\access\path\pgm.tx") with the actual relative path of your <i>file.txt</i> file.

### Details
* To properly work, the files in input need to compile correctly. If they don't (<i>e.g.></i> a missing ';'), a result will be displayed, but it may not be accurate. 
* At least two files need to be present as inputs. It doesn't have a theoretical maximal number of files to compare. 
* Finally, if there are more than two files, it displays a matrix indicating the distance between each couple of programs input. 
* At execution, the terminal prints the Dice distance between every pair of files. 
* During execution, the program creates three PGM files per comparison, containing matrices of similarities between all the lines. The darker a pixel is, the more different the lines of code are. 


## AUTHORS & CONTACTS
Don't hesitate to contact us for more information:
* Adam Kaufman alias [@ockedman](https://github.com/ockedman)
  * mail : adam.kaufman@dauphine.eu
* Camille Jouanet alias [@kmillee](https://github.com/kmillee)
  * mail : jouanet.camille@gmail.com
  * [Linkedin](https://fr.linkedin.com/in/camillejouanet)

Thank you for taking interest in our work :)
