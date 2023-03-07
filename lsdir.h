/*** ---------------------------- ***\
*>        [ List Directory ]
*>
*>   Read and create the list of directory
*> from the given directory path
*>
*> [ Created by <Ryuu Mitsuki> ]
\*** ---------------------------- ***/

/* ------------------------------
* Example Usage:
*
* int main() {
*     char dirpath[] = "<path_to_directory>";
*     CreateListDir dir(dirpath);
*     char **listdir = dir.get_listdir();
*
*     printl(listdir, dir.dirsize);
*
*     return 0;
* }
------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

class CreateListDir {
    private:
        const char* dirpath;
        char txtcolor[12];
        bool w_color;

        // Get total of files and directories
        unsigned int count_listdir(void)
        {
            DIR *dir;
            struct dirent *entry;
            unsigned int size = 0;

            // Open the directory path
            dir = opendir(this->dirpath);
            if (dir == NULL) {
                perror("Error");
                exit(1);
            }

            // Read the directory
            while ( (entry = readdir(dir)) != NULL ) {
                /** -----
                * Filter and skip the "." (current dir)
                * and ".." (parent dir)
                ----- **/
                if (strcmp(entry->d_name, ".") == 0 ||
                        strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                size++;
            }

            // Close the directory path
            closedir(dir);

            return size;
        }

        static int cmp_str(const void* __a, const void* __b) {
            // Compare two strings (char arrays)
            return strcmp( *(const char**)__a, *(const char**)__b );
        }

    public:
        const unsigned int dirsize;

        // :: Constructor
        CreateListDir(char* __dp, bool __wclr = false, int __iclr = 2)
        : dirpath(__dp), dirsize(this->count_listdir()), w_color(__wclr)
        {
            /** ---
            * Check whether is Windows or Linux
            * If Windows (_WIN32), "w_color" will turn false.
            *
            * You can comment these code below, if you want.
            --- **/
            #if !( defined(__linux__) || defined(__unix__) ) && \
                    defined(_WIN32)
                this->w_color = false;
            #endif



            // Use bright color (90 .. 99)
            bool use_bright = false;

            if (this->w_color) {
                // Return value to default if index greater than 19 or negative value
                if (__iclr > 19 || __iclr < 0) __iclr = 2;

                // Use bright color when index greater than 9, but less than 20
                else if (__iclr > 9 && __iclr < 20) {
                    __iclr -= 10; // Subtract by 10
                    use_bright = true; // Turn ON the "use_bright"
                }

                // Create the color code and copy to "txtcolor"
                if (use_bright) {
                    sprintf(this->txtcolor, "\e[9%dm", __iclr);
                } else {
                    sprintf(this->txtcolor, "\e[3%dm", __iclr);
                }
            }
        }

        // :: Destructor
        ~CreateListDir(void)
        { NULL; }


        /** -----
        *   This function will get a list of directory
        * from the given directory path
        *
        * [!] Don't forget to free the memory [!]
        ----- **/
        char **get_listdir(void)
        {
            DIR *dir;
            struct dirent *entry;
            const size_t str_size = 125; // Size for each string


            // Open the directory path
            dir = opendir(this->dirpath);
            if (dir == NULL) {
                perror("Error");
                exit(1);
            }

            // Declare and allocate memory for list strings
            char **listdir = (char**)malloc(this->dirsize * sizeof(char*));
            if (listdir == NULL) {
                fprintf(stderr, "Error: Failed to allocate memory");
                exit(1);
            }

            for (int i = 0; i < dirsize; i++) {
                // Allocate memory for each string in list
                listdir[i] = (char*)malloc( (str_size + 1) * sizeof(char*) );
                if (listdir[i] == NULL) {
                    fprintf(stderr, "Error: Failed to allocate memory");
                    exit(1);
                }
            }

            // Read directory and insert entry name to the list
            for (int i = 0; (entry = readdir(dir)) != NULL; ) {
                /** -----
                * Filter and skip the "." (current dir)
                * and ".." (parent dir)
                ----- **/
                if (strcmp(entry->d_name, ".") == 0 ||
                        strcmp(entry->d_name, "..") == 0) {
                    continue;
                }

                // Check whether "w_color" is true or false
                if (this->w_color) {
                    // Add color with the entry name to list entries
                    sprintf(listdir[i++], "%s%s\e[0m", this->txtcolor, entry->d_name);
                } else {
                    // Just add the entry name
                    sprintf(listdir[i++], "%s", entry->d_name);
                }
            }

            // Sorting the list by alphabet using "qsort()"
            qsort(listdir, this->dirsize, sizeof(char*), cmp_str);

            // Close the directory path
            closedir(dir);

            return listdir;
        }
};


/////////////////////////
// ADDITIONAL FUNCTION //
/////////////////////////

// Customized "free()" function to free the memory from list strings
void mfree(char** ls, size_t __sz = 0)
{
    // Get sizeof list instead, if 2nd parameter is passed
    if (__sz == 0) __sz = sizeof(ls);
    
    // Iterate list and free the memory each strings
    for (int i = 0; i < __sz; i++) {
        free(ls[i]);
    }
    
    // Free the memory list strings
    free(ls);
}

// Simple function to print the list quickly, then free the memory
void printl(char** ls, size_t __szlist)
{
    // Iterate list and print each strings
    for (int i = 0; i < __szlist; i++) {
        fprintf(stdout, "%s\n", ls[i]);
    }

    // Free the memory list using "mfree()"
    mfree(ls);
}
