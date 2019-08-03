/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 4
 * Due Date: 11 August 2019
 * 
 * Unit test for minionEffect function
 *
 * Include the following lines in makefile:
 *
 * randomtestcard2: randomtestcard2.c dominion.o rngs.o
 *      gcc -o randomtestcard2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)
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
#include <time.h>


#define TESTCARD "minion"
#define MAX_TESTS 100000


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
void checkCard(int *n, int choice1, int choice2, int choice3, struct gameState *post, int handpos, int *bonus) {
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));
	int i = 0, j = 0, k = 0;
	int r;
	(*n)++;

	// print information about test case
	printf("test #%d\n", *n);
	printf("choice1: %d\n", choice1);
	printf("choice2: %d\n", choice2);
	printf("pre.numPlayers: %d\n", pre.numPlayers);
	printf("pre.whoseTurn: %d\n", pre.whoseTurn);
	printf("pre.deckCount[pre.whoseTurn]: %d\n", pre.deckCount[pre.whoseTurn]);
	printf("pre.handCount[pre.whoseTurn]: %d\n", pre.handCount[pre.whoseTurn]);
	printf("pre.discardCount[pre.whoseTurn]: %d\n", pre.discardCount[pre.whoseTurn]);

	r = cardEffect(minion, choice1, choice2, choice3, post, handpos, bonus);

	if (choice1) {		// player gains 2 coin
		pre.coins += 2;
	}
	else if (choice2) {			// player redraws
		// move all cards from player's hand to discard
		for (i = pre.discardCount[pre.whoseTurn], j = 0; i < (pre.discardCount[pre.whoseTurn] + pre.handCount[pre.whoseTurn]); i++, j++) {
			pre.discard[pre.whoseTurn][i] = pre.hand[pre.whoseTurn][j];
		}
		pre.discardCount[pre.whoseTurn] += pre.handCount[pre.whoseTurn];		// discard count increased by hand count
		pre.handCount[pre.whoseTurn] = 4;		// hand count is now increased by 4
		pre.deckCount[pre.whoseTurn] -= 4;		// deck count is now decreased by 4

		// each other player must also discard their hand and draw 4 new cards
		for (k = 0; k < pre.numPlayers; k++) {
			if (k != pre.whoseTurn && pre.handCount[k] > 5) {
				for (i = pre.discardCount[k], j = 0; i < (pre.discardCount[k] + pre.handCount[k]); i++, j++) {
					pre.discard[k][i] = pre.hand[k][j];
				}
				pre.discardCount[k] += pre.handCount[k];		// discard count increased by hand count
				pre.handCount[k] = 4;		// hand count is now increased by 4
				pre.deckCount[k] -= 4;		// deck count is now decreased by 4				
			}
		}
	}

	assertPassed(1, r, 0);		// confirm function does not fail
	r = memcmp(&pre, post, sizeof(struct gameState));
	assertPassed(1, r, 0);		// confirm expected outcome
	printf("\n");
}

/***********************************************************************/
int main() {

	srand(time(0));
    int seed = 1000;
    int testNo = 0;
    int i = 0, j = 0;
    int n = 0, p = 0;
    int numPlayers = 0;
    int k[10] = {ambassador, baron, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	struct gameState G;
	int choice1 = -1, choice2 = -1, choice3 = -1;
	int handpos = 0, bonus = 0;

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> RANDOM TESTING [%s] <<<<<\n", TESTCARD);
	printf("\n------------------------------------------------------------\n\n\n");

	for (n = 0; n < MAX_TESTS; n++) {
		numPlayers = (rand() % 4);
		numPlayers++;
	    p = (rand() % numPlayers);

		// initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &G);

		G.numPlayers = numPlayers;
		G.whoseTurn = p;   

	    G.deckCount[p] = (rand() % MAX_DECK);
	    for (i = 0; i < G.deckCount[p]; i++) {
	    	G.deck[p][i] = rand() % 28;
	    }
	    G.discardCount[p] = (rand() % MAX_DISCARD);
	    for (i = 0; i < G.discardCount[p]; i++) {
	    	G.discard[p][i] = rand() % 28;
	    }
	    G.handCount[p] = (rand() % MAX_HAND);
	    for (i = 0; i < G.handCount[p]; i++) {
	    	G.hand[p][i] = rand() % 28;
	    }

	    for (j = 0; j < numPlayers; j++) {
			if (j != p) {
			    G.deckCount[j] = (rand() % MAX_DECK);
			    for (i = 0; i < G.deckCount[j]; i++) {
			    	G.deck[j][i] = rand() % 28;
			    }
			    G.discardCount[j] = (rand() % MAX_DISCARD);
			    for (i = 0; i < G.discardCount[j]; i++) {
			    	G.discard[j][i] = rand() % 28;
			    }
			    G.handCount[j] = (rand() % MAX_HAND);
			    for (i = 0; i < G.handCount[j]; i++) {
			    	G.hand[j][i] = rand() % 28;
			    }				
			}
		}

	    choice1 = (rand() % 2);
	    if (choice1 == 0) { choice2 = 1; } else if (choice1 == 1) { choice2 = 0; }
	    bonus = (rand() % 256);
	 
		// verify the card works as expected with random values	
	    checkCard(&testNo, choice1, choice2, choice3, &G, handpos, &bonus);		
	}

	printf("\n------------------------------------------------------------\n");
	printf("\n >>>>> RANDOM TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}