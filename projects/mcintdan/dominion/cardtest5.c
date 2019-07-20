/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for getWinners function
 *
 * Include the following lines in makefile:
 *
 * cardtest5: cardtest5.c dominion.o rngs.o
 *      gcc -o cardtest5 -g  cardtest5.c dominion.o rngs.o $(CFLAGS)
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


#define TESTFUNCTION "getWinners"


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
	int i = -1, j = -1;
	int result = -5;

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTFUNCTION);


	/*	TEST CASE 1:
				Player does not have any cards to draw.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\nPlayer does not have any cards to draw.\n");
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// setup
	for (j = 0; j < G.deckCount[thisPlayer]; j++) { G.deck[thisPlayer][j] = -1; }
	G.deckCount[thisPlayer] = 0;	
	G.handCount[thisPlayer] = 5;
	for (j = 0; j < G.handCount[thisPlayer]; j++) { G.hand[thisPlayer][j] = estate; }
	for (j = 0; j < G.discardCount[thisPlayer]; j++) { G.discard[thisPlayer][j] = -1; }
	G.discardCount[thisPlayer] = 0;	

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// verify player cannot draw cards when no cards in deck to draw
	result = drawCard(thisPlayer, &testG);
	printf("\ndraw card outcome actual: %d", result);
	printf("\ndraw card outcome expected: %d\t\t", -1);
	assertPassed(1, result, -1);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
				Player has cards in deck to draw.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\nPlayer has cards in deck to draw.\n\n");
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// setup
	G.deckCount[thisPlayer] = 3;
	for (j = 0; j < G.deckCount[thisPlayer]; j++) { G.deck[thisPlayer][j] = copper; }
	G.handCount[thisPlayer] = 5;
	for (j = 0; j < G.handCount[thisPlayer]; j++) { G.hand[thisPlayer][j] = estate; }
	G.discardCount[thisPlayer] = 0;
	for (j = 0; j < G.discardCount[thisPlayer]; j++) { G.discard[thisPlayer][j] = -1; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	drawCard(thisPlayer, &testG);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 1);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 1);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);

	// verifies the correct cards are in the player's hand
	printf("\nhand actual: \n");
	if ( testG.handCount[thisPlayer] > 0 ) {
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][j]);
			if (j > 4) { assertPassed(1, testG.hand[thisPlayer][j], G.deck[thisPlayer][0]); }
			else { assertPassed(1, testG.hand[thisPlayer][j], G.hand[thisPlayer][0]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.handCount[thisPlayer], 0); }
	printf("hand expected:");
	for (i = 0; i < G.handCount[thisPlayer] + 1; i++) {
		if (i <= 4) { printf("\n\t(%d)\t\t", G.hand[thisPlayer][i]); }
		else { printf("\n\t(%d)\t\t", G.deck[thisPlayer][0]); }
	}


	printf("\n\n------------------------------------------------------------\n");	


	/*	TEST CASE 3:
				Player has cards in discard but deck is empty and thus they
				need to shuffle in order to draw cards.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 3:\nPlayer has cards in discard but deck is empty and thus they need to shuffle in order to draw cards.\n\n");
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// setup
	for (j = 0; j < G.deckCount[thisPlayer]; j++) { G.deck[thisPlayer][j] = -1; }
	G.deckCount[thisPlayer] = 0;
	G.handCount[thisPlayer] = 5;
	for (j = 0; j < G.handCount[thisPlayer]; j++) { G.hand[thisPlayer][j] = estate; }
	G.discardCount[thisPlayer] = 3;
	for (j = 0; j < G.discardCount[thisPlayer]; j++) { G.discard[thisPlayer][j] = copper; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));	

	drawCard(thisPlayer, &testG);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.discardCount[thisPlayer] - 1);
	assertPassed(1, testG.deckCount[thisPlayer], G.discardCount[thisPlayer] - 1);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + 1);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] - G.discardCount[thisPlayer]);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] - G.discardCount[thisPlayer]);

	// verifies the correct cards are in the player's hand
	printf("\nhand actual: \n");
	if ( testG.handCount[thisPlayer] > 0 ) {
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][j]);
			if (j > 4) { assertPassed(1, testG.hand[thisPlayer][j], G.discard[thisPlayer][0]); }
			else { assertPassed(1, testG.hand[thisPlayer][j], G.hand[thisPlayer][0]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.handCount[thisPlayer], 0); }
	printf("hand expected:");
	for (i = 0; i < G.handCount[thisPlayer] + 1; i++) {
		if (i <= 4) { printf("\n\t(%d)\t\t", G.hand[thisPlayer][i]); }
		else { printf("\n\t(%d)\t\t", G.discard[thisPlayer][0]); }
	}


	printf("\n\n------------------------------------------------------------\n");


	printf(" >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTFUNCTION);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}