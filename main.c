#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PRISONERS       2500
//#define NUMTRIALS       10
#define NUMTRIALS       100000

//#define BRUTEFORCEALGO
#ifdef BRUTEFORCEALGO
/* We are allowed to open half of the boxes. We must find our number. If we don't don't find our number, we must
 * return false. If we do find our number we are allowed to return true. The algorithrm will be timed in outside scope.
 */
bool didFind(int lookfor, int* boxes, int numboxes) {

    //printf("I am prisoner #%u. Here are the numbers I found:\n", lookfor);

    int * myguesses = (int *)malloc(numboxes * sizeof(int));
    for (int i = 0; i < numboxes; i++) {
        myguesses[i] = i;
    }

    // printf("Here are the guesses:\n");
    // for (int i = 0; i < numboxes; i++) { printf("%u ", myguesses[i]); }
    // printf("\n");

    /* Now random shuffle them */
    for (int i = 0; i < numboxes; i++) {
        int randgen = rand() % numboxes;
        int tmp = myguesses[randgen];
        myguesses[randgen] = myguesses[i];
        myguesses[i] = tmp;
    }

    // printf("Here are the guesses after shuffling:\n");
    // for (int i = 0; i < numboxes; i++) { printf("%u ", myguesses[i]); }
    // printf("\n");    

    for (int attempts = (numboxes/2); attempts > 0; attempts--) {
        //printf("%u ", rand() % numboxes);
        if (boxes[myguesses[attempts]] == lookfor) {
            //printf("\n");
            free(myguesses);
            return true;
        }
    }
    // printf("\n");
    free(myguesses);
    return false;
}
#else

/* The math hack approach */
bool didFind(int lookfor, int* boxes, int numboxes) {
    int tmp = lookfor;
    for (int attempts = (numboxes/2); attempts > 0; attempts--) {
        if (boxes[tmp] == lookfor) {
            return true;
        }
        tmp = boxes[tmp]; /* Our next guess */
    }
    return false;
}

#endif

/* Linear time - for every box, shuffle its contents with another arbitrary box. */
void shufflepris(int * cont, int numpris) {
    srand(time(NULL));
    for (int i = 0; i < numpris; i++) {
        int randgen = rand() % numpris;
        int tmp = cont[randgen];
        cont[randgen] = cont[i];
        cont[i] = tmp;
    }
}

bool validate(int * cont, int numpris) {
    for (int i = 0; i < numpris - 1; i++) {
        for (int j = i+1; j < numpris-1; j++) {
            if (cont[i] == cont[j]) {
                return false;
            }
        }
    }

    for (int i = 0; i < numpris; i++) {
        bool found = false;
        for (int j = 0; j < numpris; j++) {
            if (cont[j] == i) {
                found = true;
            }
        }
        if (found == false) {
            return false;
        }
    }

    /* Also require that prisoners are an even number */
    if (numpris / 2.0 != numpris / (int)2) {
        printf("Not an even number!\n");
        return false;
    }
    return true;
}

void printboxes(int * boxes, int numpris) {
    for (int i = 0; i < numpris; i++) {
        printf("%d\t", boxes[i]);
        if ((i+1) % (int)sqrt(numpris) == 0) {
            printf("\n");
        }
    }
}

int main(int argc, char ** argv) {

    if (sqrt((double)PRISONERS) == (int)sqrt((double)PRISONERS)) {
        srand(time(NULL));

        /* Allocate prisoners */
        int * boxes = malloc(PRISONERS * sizeof(int));
        for (int i = 0; i < PRISONERS; i++) {
            boxes[i] = i;
        }

        /* Shuffle them around */
        shufflepris(boxes, PRISONERS);
        if (validate(boxes, PRISONERS)) {
            printboxes(boxes, PRISONERS);
        } else {
            printf("Boxes bad - end test.\n");
            return -1;
        }


        int successrate = 0;
        int goodtrials = 0;
        int total_good_pris = 0;

        /* Test the prisoners */
        for (int i = 0; i < NUMTRIALS; i ++) {
            int successfulprisoners = 0;

            for (int j = 0; j < PRISONERS; j++) {
                if (didFind(j, boxes, PRISONERS)) {
                    successfulprisoners++;
                    total_good_pris++;
                } else {
                    break;
                    /* If one prisoner fails - they all fail. */
                }
            }
            if (successfulprisoners == PRISONERS) {
                goodtrials++;
            }
            //printf("Successful prisoners: %u/%u total.\n", successfulprisoners, PRISONERS);

            /* Shuffle them around */
            shufflepris(boxes, PRISONERS);      
        }
        float percentgood = (float)goodtrials / NUMTRIALS;
        printf("\n\nResult: The prisoners live %.3f%% of the time over %u trials.\n", percentgood*100, NUMTRIALS);
        printf("The total number of successful prisoners was: %u out of %u = %.3f%% individual success rate\n\n", total_good_pris, PRISONERS * NUMTRIALS, (float)total_good_pris/(PRISONERS * NUMTRIALS)*100);

        free(boxes);
        return 0;
    } else {
        printf("Num prisoners is not a perfect square - not supported yet");
        return -1;
    }
}