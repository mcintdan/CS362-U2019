/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for shuffle function
 *
 * Include the following lines in makefile:
 *
 * cardtest2: cardtest2.c dominion.o rngs.o
 *      gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
 *
 * REFERENCE NOTE:
 *		Some code from the cardtest4.c file and other help files provided
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


#define TESTFUNCTION "shuffle"


/************************************************************************
 * assertPassed
 *		verifies equality/inequality of two values to determine whether
 *			test case passed or failed
 ************************************************************************/
void assertPassed(int assertType, int value1, int value2) {
	if (assertType) {		// testing for equality
		if (value1 == value2) { printf("PASSED [X]\n"); }
		else { printf("FAILED [ ]\n"); }
	}
	else {					// testing for inequality
		if (value1 != value2) { printf("PASSED [X]\n"); }
		else { printf("FAILED [ ]\n"); }
	}
}

/***********************************************************************/
int main() {

    int seed = 1000;
    int numPlayers = 4;
    int thisPlayer = 0;
	int k[10] = {ambassador, baron, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	struct gameState testG, G;
	int i = -1, result = -1;
	int fail = -1, success = 0;

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTFUNCTION);


	/*	TEST CASE 1:
				Invalid number of cards in deck, too few cards.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\nInvalid number of cards in deck, too few cards.\n\n");
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	testG.deckCount[thisPlayer] = 0;

	// verify deck cannot be shuffled with too few cards
	result = shuffle(0, &testG);
	printf("number of cards in deck = %d\n", testG.deckCount[thisPlayer]);
	printf("deck shuffle outcome = %d, expected = %d\t\t\t", result, fail);
	assertPassed(1, result, fail);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
				Valid number of cards in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\nValid number of cards in deck.\n\n");
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// verify deck is correctly shuffled with valid number of cards
	shuffle(0, &testG);
	printf("deck before shuffle:\t");
	for (i = 0; i < G.deckCount[thisPlayer]; i++) { printf("(%d)\t", G.deck[thisPlayer][i]); }
	printf("\n\ndeck after shuffle:\t");
	for (i = 0; i < testG.deckCount[thisPlayer]; i++) { printf("(%d)\t", testG.deck[thisPlayer][i]); }
	result = memcmp(testG.deck[thisPlayer], &G.deck[thisPlayer], sizeof(struct gameState));
	assertPassed(0, result, success);	// verify deck is shuffled

	printf("\n\n------------------------------------------------------------\n");


	printf(" >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTFUNCTION);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}