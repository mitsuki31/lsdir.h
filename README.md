# lsdir.h<br>
> **Author: Ryuu Mitsuki**

## DESCRIPTION<br>
**Read and create a list entries of directory from specified directory path.**

## EXAMPLE USAGE
### Simple Usage
- _example.cpp_
```cpp
#include <stdio.h>
#include "lsdir.h"

int main(void) {
    char dirpath[ ] = "<path_to_directory>"; // directory path
    CreateListDir dir(dirpath);
    char** listdir = dir.get_listdir();
    
    /* ---
    * printl() is customized function
    * that will print list entries directory instantly
    * and then automatically free the memory.
    * 
    * printl(char **ls, size_t __szlist);
    --- */
    printl(listdir, dir.dirsize);
    
    return 0;
}
```
**And then compile it with `gcc` compiler<br>**
**`g++ example.cpp -o example`. Run it with `./example`<br>**

### With Argument Parameters
- _example.cpp_
```cpp
#include <stdio.h>
#include <stdlib.h>
#include "lsdir.h"

int main(int argc, char* argv[ ]) {
    // declare and allocate memory for string directory path
    char* dirpath = (char*)malloc(256 * sizeof(char));

    if (argc >= 2) {
        sprintf(dirpath, "%s", argv[1]);
    } else {
        sprintf(dirpath, "%s", "."); // "." is current directory
    }
    
    CreateListDir dir(dirpath);
    char** listdir = dir.get_listdir();
    
    printl(listdir, dir.dirsize);
    
    // free the memory "dirpath"
    free(dirpath);
    
    return 0;
}
```
**Compile it, and then run it.<br>**
`./example` **>>** read current directory<br>
`./example <directory_path>` **>>** read specified directory

### With Argument Parameters & Colors
- example.cpp
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lsdir.h"

int main(int argc, char* argv[ ]) {
    // declare and allocate memory for string directory path
    char* dirpath = (char*)malloc(256 * sizeof(char));
    bool useclr = false;
    int idclr = 2;

    if (argc >= 2) {
        sprintf(dirpath, "%s", argv[1]);
        if (argc >= 3) {
            if (strcmp(argv[2], "-C") == 0 || strcmp(argv[2], "-c") ==>
                if (argc == 4) idclr = atoi(argv[3]);
                useclr = true;
            } else {
                fprintf(stderr, "[!] Error: Unknown options\n");
                return 1;
            }
        }
    } else {
        sprintf(dirpath, "%s", "."); // "." = current dir
    }

    // create object class
    CreateListDir dir(dirpath, useclr, idclr);

    // get list of directory entries
    char** listdir = dir.get_listdir();

    // print all directory entries with custom function
    printl(listdir, dir.dirsize);

    // free the memory "dirpath"
    free(dirpath);

    return 0;
}
```
**Compile with `g++ example -o example`<br>**
**Usage:<br>**
`./example [directory] [options] [idcolor]`<br>
`./example . -C 12` this code will print list of entries from current directory with color code 12 (bright green)
