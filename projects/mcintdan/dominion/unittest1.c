/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for baronEffect function
 *
 * Include the following lines in makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
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


#define TESTCARD "baron"


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
    int numPlayers = 2;
    int thisPlayer = 0; 
	int numCoinsEarned = 0, numActionsEarned = 0, numBuysEarned = 1;
	int numCardsDrawn = 0, numCardsShuffled = 0;
	int numCardsGainedToDeck = 0, numCardsGainedToHand = 0, numCardsGainedToDiscard = 0;
	int numCardsPlayed = 1, numCardsDiscarded = 0;  
	struct gameState G, testG;
	int i = 0, j = 0;
	int choice1 = -1, choice2 = -1, choice3 = -1, handpos = 0, bonus = 0;
	int k[10] = {ambassador, baron, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);


	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTCARD);


	/*	TEST CASE 1:
			choice1 = 0
				Player chooses not to discard an Estate and gains an
				Estate.

				Note: Test case assumes there are still Estates remaining
				in the supply.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\n\tchoice1 = 0\n\n");
	printf("Player chooses not to discard an Estate and gains an Estate.\n");

	// setup
	choice1 = 0;
	numCardsGainedToDiscard = 1;
	G.hand[thisPlayer][0] = baron;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(baron, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard pile
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coins = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies correct card has been added to discard pile
	printf("discard contents = %d, expected = %d\t", testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 2], estate);
	assertPassed(1, testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 2], estate);

	// verifies correct number of estates in supply
	printf("estate supply count = %d, expected = %d\t", testG.supplyCount[estate], G.supplyCount[estate] - numCardsGainedToDiscard);
	assertPassed(1, testG.supplyCount[estate], G.supplyCount[estate] - numCardsGainedToDiscard);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
			choice1 = 1
				Player chooses to discard an Estate and earns 4 coin.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\n\tchoice1 = 1\n\n");
	printf("Player chooses to discard an Estate and earns 4 coin.\n");

	// setup
	choice1 = 1;
	numCoinsEarned = 4;
	numCardsDiscarded = 1;	
	numCardsGainedToDiscard = 0;

	for (i = 0; i < G.handCount[thisPlayer]; i++) {
		if (i == 0) {	// when testing Estate in first position, move testcard to last position
			G.hand[thisPlayer][0] = estate;
			G.hand[thisPlayer][1] = baron;
			handpos = 1;
			for (j = 2; j < G.handCount[thisPlayer]; j++) { G.hand[thisPlayer][j] = copper; }
		}
		else {
			G.hand[thisPlayer][0] = baron;
			handpos = 0;
			G.hand[thisPlayer][i] = estate;
			for (j = 1; j < G.handCount[thisPlayer]; j++) { if (j != i) { G.hand[thisPlayer][j] = copper; }	}
		}

		// reset
		bonus = 0;

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		printf("\n");
		printf("Testing estate in index [%d]\n", i);
		printf("\n");

		cardEffect(baron, choice1, choice2, choice3, &testG, handpos, &bonus);

		// verifies correct number of cards in deck
		printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
		assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

		// verifies correct number of cards in hand
		printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
		assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

		// verifies correct number of cards in discard pile
		printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
		assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
		
		// verifies correct number of actions
		printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
		assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

		// verifies correct number of buys
		printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
		assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

		// verifies correct number of coins	
		printf("coins = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
		assertPassed(1, bonus, numCoinsEarned);

		// verifies correct card has been added to discard pile
		printf("discard contents = %d, expected = %d\t", testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 2], estate);
		assertPassed(1, testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 2], estate);

		// verifies correct number of estates in supply
		printf("estate supply count = %d, expected = %d\t", testG.supplyCount[estate], G.supplyCount[estate] - numCardsGainedToDiscard);
		assertPassed(1, testG.supplyCount[estate], G.supplyCount[estate] - numCardsGainedToDiscard);

		printf("\n\n\n");
	}

	printf("------------------------------------------------------------\n");


	/*	TEST CASE 3:
			choice1 = 1
				Player chooses to discard an Estate but does not have one
				to discard and thus gains an Estate.

				Note: Test case assumes there are still Estates remaining
				in the supply.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 3:\n\tchoice1 = 1\n\n");
	printf("Player chooses to discard an Estate but does not have one to discard and thus gains an Estate.\n");

	// setup
	choice1 = 1;
	numCoinsEarned = 0;
	numCardsDiscarded = 0;	
	numCardsGainedToDiscard = 1;

	// set players cards without an estate
	G.hand[thisPlayer][0] = baron;
	G.hand[thisPlayer][1] = copper;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;

	// reset
	bonus = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(baron, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard pile
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);
	
	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coins = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies correct card has been added to discard pile
	printf("discard contents = %d, expected = %d\t", testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 2], estate);
	assertPassed(1, testG.discard[thisPlayer][testG.discardCount[thisPlayer] - 2], estate);

	// verifies correct number of estates in supply
	printf("estate supply count = %d, expected = %d\t", testG.supplyCount[estate], G.supplyCount[estate] - numCardsGainedToDiscard);
	assertPassed(1, testG.supplyCount[estate], G.supplyCount[estate] - numCardsGainedToDiscard);

	printf("\n\n\n------------------------------------------------------------\n");

	printf("\n >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}