/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for mineEffect function
 *
 * Include the following lines in makefile:
 *
 * unittest5: unittest5.c dominion.o rngs.o
 *      gcc -o unittest5 -g  unittest5.c dominion.o rngs.o $(CFLAGS)
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


#define TESTCARD "mine"


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
	int numCoinsEarned = 0, numActionsEarned = 0, numBuysEarned = 0;
	int numCardsDrawn = 0, numCardsShuffled = 0;
	int numCardsGainedToDeck = 0, numCardsGainedToHand = 0, numCardsGainedToDiscard = 0;
	int numCardsPlayed = 1, numCardsDiscarded = 0, numCardsTrashed = 0; 
	struct gameState G, testG;
	int i = 0, j = 0;
	int choice1 = -1, choice2 = -1, choice3 = -1, handpos = 0, bonus = 0;
	int k[10] = {ambassador, baron, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// setup
		// current player has hand of 5 cards, first card is played card, mine
	G.handCount[thisPlayer] = 5;
	G.hand[thisPlayer][0] = mine;

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTCARD);


	/*	TEST CASE 1:
				Player chooses to trash a non-treasure card, which is an 
				invalid choice.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\n\tchoice1 = 0 and choice2 = 6\n\n");
	printf("Player chooses to trash a non-treasure card, which is an invalid choice.\n");

	// setup
	choice1 = 1;
	choice2 = 6;
	G.hand[thisPlayer][1] = cutpurse;
	for (i = 2; i < G.handCount[thisPlayer]; i++) { G.hand[thisPlayer][i] = copper; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsTrashed);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsTrashed);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies correct number of chosen card in supply
	if (choice2 > treasure_map || choice2 < curse) { printf("selected card to gain is invalid"); }
	else {
		printf("card supply count = %d, expected = %d\t", testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
		assertPassed(1, testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
	}

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
				Player chooses to gain an invalid card.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\n\tchoice1 = 2 and choice2 = -1\n\n");
	printf("Player chooses to gain an invalid card.\n");

	// setup
	choice1 = 2;
	choice2 = -1;
	for (i = 1; i < G.handCount[thisPlayer]; i++) { G.hand[thisPlayer][i] = copper; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsTrashed);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsTrashed);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies correct number of chosen card in supply
	if (choice2 > treasure_map || choice2 < curse) { printf("selected card to gain is invalid"); }
	else {
		printf("card supply count = %d, expected = %d\t", testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
		assertPassed(1, testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
	}

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 3:
				Player chooses to trash a copper and gain a silver.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 3:\n\tchoice1 = 2 and choice2 = 5\n\n");
	printf("Player chooses to trash a copper and gain a silver.\n");

	// setup
	choice1 = 2;
	choice2 = 5;
	numCardsGainedToHand = 1;
	numCardsTrashed = 1;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsTrashed);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsTrashed);

	// verifies the correct cards are in the player's hand
	printf("hand actual: ");
	if ( testG.handCount[thisPlayer] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][j]);
			if (j == 0) { assertPassed(1, testG.hand[thisPlayer][j], choice2); }
			else { assertPassed(1, testG.hand[thisPlayer][j], G.hand[thisPlayer][1]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("hand expected:");
	for (i = 0; i < G.handCount[thisPlayer] - numCardsPlayed - numCardsTrashed + numCardsGainedToHand; i++) {
		if (i == 0) { printf("\n\t(%d)", choice2); }
		else { printf("\n\t(%d)", G.hand[thisPlayer][1]); }
	}
	printf("\n");

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);

	// verifies the correct cards are in the player's discard
	printf("discard actual: ");
	if ( testG.discardCount[thisPlayer] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer][j]);
			if (j == testG.discardCount[thisPlayer] - 1) { assertPassed(1, testG.discard[thisPlayer][j], G.deck[thisPlayer][0]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer][0]);
	printf("\n");

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies correct number of chosen card in supply
	if (choice2 > treasure_map || choice2 < curse) { printf("selected card to gain is invalid"); }
	else {
		printf("card supply count = %d, expected = %d\t", testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
		assertPassed(1, testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
	}

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 4:
				Player chooses to trash a copper and gain a gold.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 4:\n\tchoice1 = 2 and choice2 = 6\n\n");
	printf("Player chooses to trash a copper and gain a gold.\n");

	// setup
	choice1 = 2;
	choice2 = 6;
	numCardsGainedToHand = 0;
	numCardsTrashed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsTrashed);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsTrashed);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsDiscarded - numCardsShuffled + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies correct number of chosen card in supply
	if (choice2 > treasure_map || choice2 < curse) { printf("selected card to gain is invalid"); }
	else {
		printf("card supply count = %d, expected = %d\t", testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
		assertPassed(1, testG.supplyCount[choice2], G.supplyCount[choice2] - numCardsGainedToHand);
	}

	printf("\n\n------------------------------------------------------------\n");


	printf("\n >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}