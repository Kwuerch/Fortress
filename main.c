#include <stdio.h>

#include "config.h"
#include "mask.h"
#include "board.h"
#include "move.h"
#include "test.h"
#include "game.h"

int main(){
    initMasks();
    playGame();

    //runTests();

    return 0;
}
