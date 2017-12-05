#include <stdio.h>
#include "elem.h"
#include "config.h"

int main(){
    Elem e = newElem();
    int i = 0;
    for(i; i < CONSUMERS; ++i){
        printf("To jest: %s", boolNames[e.alreadyReadBy[i]]);
    }
    return 0;
}