/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 4
 * Due Date: 11 August 2019
 * 
 * Unit test for baronEffect function
 *
 * Include the following lines in makefile:
 *
 * randomtestcard1: randomtestcard1.c dominion.o rngs.o
 *      gcc -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
 *
 * REFERENCE NOTE:
 *		Some code from the testDrawCard.c file and other help files provided
 *			as part of the CS362 course materials has been borrowed and/or
 *			adapted for the drafting of this unit test.
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>


#define TESTCARD "baron"
#define MAX_TESTS 2000


/************************************************************************
 * assertPassed
 *		verifies equality/inequality of two values to determine whether
 *			test case passed or failed
 ************************************************************************/
void assertPassed(int assertType, int value1, int value2) {
	if (assertType) {		// testing for equality
		if (value1 != value2) { printf("FAILED [ ]\n"); }
	}
	else {					// testing for inequality
		if (value1 == value2) { printf("FAILED [ ]\n"); }
	}
}

/************************************************************************
 * checkCard
 *		verifies expected behavior for the particular card being tested
 ************************************************************************/
void checkCard(int choice1, int choice2, int choice3, struct gameState *post, int handpos, int *bonus) {
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int r;

	r = cardEffect(baron, choice1, choice2, choice3, post, handpos, bonus);

	pre.numBuys++;

	if (choice1 = 0) {		// player does not discard estate but gains estate
		pre.handCount[pre.whoseTurn]--;		// -1 for played card
		pre.discardCount[pre.whoseTurn] += 2;		// +2 for played card and gained card
		pre.supplyCount[estate]--;		// -1 for gained card
	}
	else {			// player discards an estate for +4 coin
		pre.handCount[pre.whoseTurn] -= 2;		// -2 for played card and discarded card
		pre.discardCount[pre.whoseTurn] += 2;		// +2 for played card and discarded card
		pre.coins += 4;		// +4 for gained coins
	}

	assertPassed(1, r, 0);		// confirm function does not fail
	r = memcmp(&pre, post, sizeof(struct gameState));
	assertPassed(1, r, 0);		// confirm expected outcome
}

/***********************************************************************/
int main() {

    int seed = 1000;
    int i = 0, n = 0;
    int numPlayers = 0;
    int deckCount, handCount, discardCount;
    int k[10] = {ambassador, baron, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	struct gameState G;
	int choice2 = -1, choice3 = -1;
	int handpos = 0, bonus = 0;

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> RANDOM TESTING [%s] <<<<<\n", TESTCARD);
	for (n = 0; n < MAX_TESTS; n++) {
		numPlayers = floor(Random() * 4) + 1;
	    for (i = 0; i < sizeof(struct gameState); i++) {
	      ((char*)&G)[i] = floor(Random() * 256);
	    }
	    p = floor(Random() * numPlayers);
	    G.deckCount[p] = floor(Random() * MAX_DECK);
	    G.discardCount[p] = floor(Random() * MAX_DECK);
	    G.handCount[p] = floor(Random() * MAX_HAND);
	    choice1 = floor(Random() * 2);
	    handpos = floor(Random() * handCount);
	    bonus = floor(Random() * 256);

		// initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &G);	    

		// verify the card works as expected with random values	
	    checkCard(choice1, choice2, choice3, &G, handpos, &bonus);		
	}
	printf("\n\n\n------------------------------------------------------------\n");
	printf("\n >>>>> RANDOM TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

/*
	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> FIXED TESTING [%s] <<<<<\n", TESTCARD);




	printf("\n\n\n------------------------------------------------------------\n");
	printf("\n >>>>> FIXED TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");
*/
	return 0;
}