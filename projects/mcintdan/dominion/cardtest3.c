/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for endTurn function
 *
 * Include the following lines in makefile:
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
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


#define TESTFUNCTION "endTurn"


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
	int i = -1, result = -1, numCoins = -1;

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTFUNCTION);


	/*	TEST CASE 1:
				End turn for player 1.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\nEnd turn for player 1.\n\n");
	numCoins = 0;
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// verify players cards are correctly discarded and no longer in hand
	endTurn(&testG);
	printf("number of cards in hand before turn: %d", G.handCount[thisPlayer]);
	printf("\nhand before turn:\t");
	for (i = 0; i < G.handCount[thisPlayer]; i++) { printf("(%d)\t", G.hand[thisPlayer][i]); }
	printf("\nnumber of cards in discard before turn: %d", G.discardCount[thisPlayer]);
	printf("\ndiscard before turn:\t[empty]");

	printf("\n\nnumber of cards in hand after turn ends actual: %d, expected: %d\t\t", testG.handCount[thisPlayer], 0);
	assertPassed(1, testG.handCount[thisPlayer], 0);
	printf("cards in hand after turn ends, actual:\n\t");
	if (testG.handCount[thisPlayer] > 0) { for (i = 0; i < testG.handCount[thisPlayer]; i++) { printf("(%d)\t", testG.hand[thisPlayer][i]); } }
	else { printf("[empty]\n"); }
	printf("cards in hand after turn ends, expected:\n\t");
	printf("[empty]\t\t\t\t\t\t\t\t");
	result = memcmp(&testG.hand[thisPlayer], &G.hand[thisPlayer], 5 * sizeof(int));
	assertPassed(0, result, 0);		// verify hand is not the same as before turn

	printf("\nnumber of cards in discard after turn ends actual: %d, expected: %d\t", testG.discardCount[thisPlayer], G.handCount[thisPlayer]);
	assertPassed(1, testG.discardCount[thisPlayer], G.handCount[thisPlayer]);
	printf("cards in discard after turn ends, actual:\n\t");
	if (testG.discardCount[thisPlayer] > 0) { for (i = 0; i < testG.discardCount[thisPlayer]; i++) { printf("(%d)\t", testG.discard[thisPlayer][i]); } }
	else { printf("[empty]"); }
	printf("\ncards in discard after turn ends, expected:\n\t");
	if (testG.discardCount[thisPlayer] > 0) { for (i = 0; i < G.handCount[thisPlayer]; i++) { printf("(%d)\t", G.hand[thisPlayer][i]); } printf("\t\t\t"); }
	else { printf("[empty]\t\t\t\t\t\t\t\t"); }
	result = memcmp(&testG.discard[thisPlayer], &G.hand[thisPlayer], 5 * sizeof(int));
	assertPassed(1, result, 0);		// verify discard after turn matches hand before turn

	// verify new/next player becomes new current player after turn
	printf("\nplayer whose turn follows after end turn, actual: %d, expected: %d\t", testG.whoseTurn, G.whoseTurn + 1);
	assertPassed(1, testG.whoseTurn, G.whoseTurn + 1);
	printf("\n");

	// verify the remaining state variables for new/next player's turn
	printf("outpost played actual = %d, expected = %d\t\t\t\t\t", testG.outpostPlayed, 0);
	assertPassed(1, testG.outpostPlayed, 0);
	printf("phase actual = %d, expected = %d\t\t\t\t\t\t", testG.phase, 0);
	assertPassed(1, testG.phase, 0);
	printf("number of actions actual = %d, expected = %d\t\t\t\t", testG.numActions, 1);
	assertPassed(1, testG.numActions, 1);
	printf("number of buys actual = %d, expected = %d\t\t\t\t\t", testG.numBuys, 1);
	assertPassed(1, testG.numBuys, 1);
	if (testG.handCount[thisPlayer + 1] > 0) {
		for (i = 0; i < testG.handCount[thisPlayer + 1]; i++) {
			if (testG.hand[thisPlayer + 1][i] == copper) { numCoins++; }
			else if (testG.hand[thisPlayer + 1][i] == silver) { numCoins += 2; }
			else if (testG.hand[thisPlayer + 1][i] == gold) { numCoins += 3; }
		}
	}
	printf("number of coins actual = %d, expected = %d\t\t\t\t", testG.coins, numCoins);
	assertPassed(1, testG.coins, numCoins);
	printf("played card count actual = %d, expected = %d\t\t\t\t", testG.playedCardCount, 0);
	assertPassed(1, testG.playedCardCount, 0);

	// verify new/next player draws a hand of 5 cards
	printf("\nnew player's number of cards in hand actual: %d, expected: %d\t\t", testG.handCount[thisPlayer + 1], 5);
	assertPassed(1, testG.handCount[thisPlayer + 1], 5);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
				End turn for last player.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\nEnd turn for last player.\n\n");
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.whoseTurn = 3;

	// verify new/next player becomes new current player after turn
	endTurn(&testG);
	printf("player whose turn follows after end turn, actual: %d, expected: %d\t", testG.whoseTurn, 0);
	assertPassed(1, testG.whoseTurn, 0);

	printf("\n\n------------------------------------------------------------\n");


	printf(" >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTFUNCTION);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}