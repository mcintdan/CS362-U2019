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
 * unittest1: cardtest4.c dominion.o rngs.o
 *      gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
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
    int numPlayers = 3;
	int k[10] = {ambassador, baron, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	struct gameState testG, G;
	int i = -1, j = -1;
	int result = -1;
	int winners1[MAX_PLAYERS];
	int winners2[MAX_PLAYERS];
	int winnersExpected1[4] = {0, 0, 1, 0};
	int winnersExpected2[4] = {1, 1, 0, 0};

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTFUNCTION);


	/*	TEST CASE 1:
				Player 3 has high score in 3-player game when player 2
				ends game. Player 3's points are in their hand.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\nPlayer 3 has high score in 3-player game when player 2 ends game.\n\n");
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// setup
	for (i = 0; i < numPlayers; i++) {
		for (j = 0; j < G.deckCount[i]; j++) { G.deck[i][j] = -1; }
		G.deckCount[i] = 0;
		for (j = 0; j < G.handCount[i]; j++) { G.hand[i][j] = -1; }
		G.handCount[i] = 0;
		for (j = 0; j < G.discardCount[i]; j++) { G.discard[i][j] = -1; }
		G.discardCount[i] = 0;	
	}	
	i = 2;		// player 3 has hand of provinces
	G.handCount[i] = 5;
	for (j = 0; j < G.handCount[i]; j++) { G.hand[i][j] = province; }
	G.whoseTurn = 1;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// verify player 3 is declared winner
	getWinners(winners1, &testG);
	printf("\nwinners actual:\t\t");
	for (i = 0; i < MAX_PLAYERS; i++) { printf("(%d)\t", winners1[i]); }
	printf("\nwinners expected:\t");
	for (i = 0; i < MAX_PLAYERS; i++) { printf("(%d)\t", winnersExpected1[i]); }
	printf("\t\t\t\t");
	result = memcmp(winners1, winnersExpected1, 4 * sizeof(int));
	assertPassed(1, result, 0);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
				Players 1 and 2 tie in 3-player game when player 2 ends
				game. Player 1's points are in their deck, player 2's 
				points are in their discard.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\nPlayers 1 and 2 tie in 3-player game when player 2 ends game.\n\n");
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// setup	
	for (i = 0; i < numPlayers; i++) {
		for (j = 0; j < G.deckCount[i]; j++) { G.deck[i][j] = -1; }
		G.deckCount[i] = 0;	
		for (j = 0; j < G.handCount[i]; j++) { G.hand[i][j] = -1; }
		G.handCount[i] = 0;
		for (j = 0; j < G.discardCount[i]; j++) { G.discard[i][j] = -1; }
		G.discardCount[i] = 0;	
	}
	// players 1 has deck of provinces
	i = 0;
	G.deckCount[i] = 5;
	for (j = 0; j < G.deckCount[i]; j++) { G.deck[i][j] = province; }	
	// player 2 has discard of provinces
	i = 1;
	G.discardCount[i] = 5;
	for (j = 0; j < G.discardCount[i]; j++) { G.discard[i][j] = province; }	
	G.whoseTurn = 1;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	// verify players 1 and 2 are both declared winners
	getWinners(winners2, &testG);
	printf("\nwinners actual:\t\t");
	for (i = 0; i < MAX_PLAYERS; i++) { printf("(%d)\t", winners2[i]); }
	printf("\nwinners expected:\t");
	for (i = 0; i < MAX_PLAYERS; i++) { printf("(%d)\t", winnersExpected2[i]); }
	printf("\t\t\t\t");
	result = memcmp(winners2, winnersExpected2, 4 * sizeof(int));
	assertPassed(1, result, 0);

	printf("\n\n------------------------------------------------------------\n");


	printf(" >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTFUNCTION);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}