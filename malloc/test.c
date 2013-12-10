#include "malloc2_prevs.h"

int main(int argc, char* argv[]) {

    if(argc == 2) {

        /*tests on malloc*/
        if(strcmp("1", argv[1]) == 0){
            char *p = malloc(4000);
            char *p2 = malloc(1000);
            free(p);
            free(p2);
        }

        else if(strcmp("2", argv[1]) == 0){
            char *p = malloc(4000);
            char *p2 = malloc(1001);
            free(p);
            free(p2);
        }

        /*tests on free*/
        else if(strcmp("3", argv[1]) == 0){
            int *x;
            free(x);
        }

        else if(strcmp("4", argv[1]) == 0){
            char *p = malloc(4000);
            free(p + 100);
        }

        else if(strcmp("5", argv[1]) == 0){
            char *p = malloc(4000);
            free(p);
            free(p);
        }

        else if(strcmp("6", argv[1]) == 0){
            char *p;
            p = malloc(4000);
            free(p);
            p = malloc(4000);
            free(p);
        }

        else {
            printf("Please enter a number [1-6], to test this program.\n");
        }
    }
    else {
            printf("Please enter 2 arguments './malloc <[1-4]>'");
        }

    return 0;
}
