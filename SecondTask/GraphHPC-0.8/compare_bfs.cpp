#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
using namespace std;

int main(int argc, char** argv)
{
	char* checkFilename = NULL;
    char* validFilename = NULL;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-valid")) {
            validFilename = argv[++i];
        }
        if (!strcmp(argv[i], "-check")) {
            checkFilename = argv[++i];
        }
    }

    if (!checkFilename || !validFilename) {
        cerr << "Usage: -valid <file1> -check <file2>" << endl;
        exit(1);
    }

    FILE *F1, *F2; 
    int checkNLevels[MAX_NLEVELS];
    int validNLevels[MAX_NLEVELS];
    int checkNLevelsLength, validNLevelsLength;
    int validation_is_ok = 1;
    F1 = fopen(validFilename, "r");
    F2 = fopen(checkFilename, "r");
    if (!F1) {
        cerr << "Error in opening file " << validFilename << endl;
        exit(1);
    }  
    if (!F2) {
        cerr << "Error in opening file " << checkFilename << endl;
        exit(1);
    }   
    fscanf(F1, "%d\n", &validNLevelsLength);
    fscanf(F2, "%d\n", &checkNLevelsLength);
    if (checkNLevelsLength != validNLevelsLength) {
        fprintf(stderr, "error: the number of levels is not the same\n");
        fprintf(stderr, "valid number of levels is %d, check number of levels is %d\n", validNLevelsLength, checkNLevelsLength);
        validation_is_ok = 0;
    }
    for (int i = 0; i < validNLevelsLength; ++i) {
        fscanf(F1, "%d\n", &validNLevels[i]);
        fscanf(F2, "%d\n", &checkNLevels[i]);
        if(checkNLevels[i] < validNLevels[i]) {
            fprintf(stderr, "error: at level %d valid number of nodes = %d incorrect number of nodes = %d\n",  i, validNLevels[i],checkNLevels[i]);                    
            validation_is_ok = 0;
        }
    }
    if (validation_is_ok) printf("validation is ok\n");
    else printf("validation is not ok\n");
    fclose(F1);
    fclose(F2);
}
