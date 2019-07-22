/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for tributeEffect function
 *
 * Include the following lines in makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
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


#define TESTCARD "tribute"


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
	int numCardsPlayed = 1, numCardsRevealed = 0, numCardsDiscarded = 0; 
	struct gameState G, testG;
	int i = 0, j = 0;
	int choice1 = -1, choice2 = -1, choice3 = -1, handpos = 0, bonus = 0;
	int k[10] = {ambassador, baron, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// setup
		// current player has deck of 5 cards, each gold, and hand of 5 cards, each copper
	for (i = 0; i < G.deckCount[thisPlayer]; i++) {
		G.deck[thisPlayer][i] = gold;
	}
	G.hand[thisPlayer][0] = tribute;
	for (i = 1; i < G.handCount[thisPlayer]; i++) {
		G.hand[thisPlayer][i] = copper;
	}
	for (i = 0; i < G.deckCount[thisPlayer + 1]; i++) { G.deck[thisPlayer + 1][i] = -1; }
	G.deckCount[thisPlayer + 1] = 0;
	for (i = 0; i < G.discardCount[thisPlayer + 1]; i++) { G.discard[thisPlayer + 1][i] = -1; }
	G.discardCount[thisPlayer + 1] = 0;

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTCARD);


	/*	TEST CASE 1:
				Player to left has 0 cards to reveal.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\n\nPlayer to left has 0 cards to reveal.\n\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
				Player to left has 1 action card to reveal, ambassador,
				in discard.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\n\nPlayer to left has 1 action card to reveal, ambassador, in discard.\n\n");

	// setup
	numActionsEarned = 2;
	numCardsRevealed = 1;
	numCardsShuffled = 1;
	G.deckCount[thisPlayer + 1] = 0;
	G.discardCount[thisPlayer + 1] = 1;
	G.discard[thisPlayer + 1][0] = ambassador;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.discard[thisPlayer + 1][0]); }	
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.discard[thisPlayer + 1][0]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 3:
				Player to left has 1 action card to reveal, baron, in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 3:\n\nPlayer to left has 1 action card to reveal, baron, in deck.\n\n");

	// setup
	numCardsShuffled = 0;
	G.deckCount[thisPlayer + 1] = 1;
	G.deck[thisPlayer + 1][0] = baron;
	G.discardCount[thisPlayer + 1] = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][0]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer + 1][0]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 4:
				Player to left has 2 action cards to reveal in discard,
				each named the same, sea_hag, 5 cards total in discard.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 4:\n\nPlayer to left has 2 action cards to reveal in discard, each named the same, sea_hag, 5 cards total in discard.\n\n");

	// setup
	numActionsEarned = 2;
	numCardsRevealed = 2;
	numCardsShuffled = 5;
	G.deckCount[thisPlayer + 1] = 0;
	G.discardCount[thisPlayer + 1] = 5;
	for (i = 0; i < G.discardCount[thisPlayer + 1]; i++) { G.discard[thisPlayer + 1][i] = sea_hag; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.discard[thisPlayer + 1][0]); }	
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.discard[thisPlayer + 1][0]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	for (i = 0; i < 2; i++) { printf("\n\t(%d)", G.discard[thisPlayer + 1][0]); }
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 5:
				Player to left has 2 action cards to reveal in deck,
				each named differently, smithy and village, 5 cards total
				in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 5:\n\nPlayer to left has 2 action cards to reveal in deck, each named differently, smithy and village, 5 cards total in deck.\n\n");

	// setup
	numActionsEarned = 4;
	numCardsShuffled = 0;
	G.deckCount[thisPlayer + 1] = 5;
	for (i = 0; i < G.deckCount[thisPlayer + 1] - 2; i++) { G.deck[thisPlayer + 1][i] = curse; }
	G.deck[thisPlayer + 1][3] = smithy;
	G.deck[thisPlayer + 1][4] = village;
	G.discardCount[thisPlayer + 1] = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][4]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][3]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer + 1][4]);
	printf("\n\t(%d)", G.deck[thisPlayer + 1][3]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 6:
				Player to left has 2 treasure cards to reveal in deck,
				each named the same, copper, 5 cards total in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 6:\n\nPlayer to left has 2 treasure cards to reveal in deck, each named the same, copper, 5 cards total in deck.\n\n");

	// setup
	numActionsEarned = 0;
	numCoinsEarned = 2;
	for (i = 0; i < G.deckCount[thisPlayer + 1]; i++) { G.deck[thisPlayer + 1][i] = copper; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][i]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][i]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	for (i = 0; i < 2; i++) { printf("\n\t(%d)", G.deck[thisPlayer + 1][i]); }
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 7:
				Player to left has 2 treasure cards to reveal in deck,
				each named differently, silver and gold, 5 cards total in
				deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 7:\n\nPlayer to left has 2 treasure cards to reveal in deck, each named differently, silver and gold, 5 cards total in deck.\n\n");

	// setup
	bonus = 0;
	numCoinsEarned = 4;
	for (i = 0; i < G.deckCount[thisPlayer + 1] - 2; i++) { G.deck[thisPlayer + 1][i] = curse; }
	G.deck[thisPlayer + 1][3] = silver;
	G.deck[thisPlayer + 1][4] = gold;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][4]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][3]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer + 1][4]);
	printf("\n\t(%d)", G.deck[thisPlayer + 1][3]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 8:
				Player to left has 2 victory cards to reveal in deck,
				each named the same, estate, 5 cards total in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 8:\n\nPlayer to left has 2 victory cards to reveal in deck, each named the same, estate, 5 cards total in deck.\n\n");

	// setup
	bonus = 0;
	numCoinsEarned = 0;
	numCardsDrawn = 2;
	for (i = 0; i < G.deckCount[thisPlayer + 1]; i++) { G.deck[thisPlayer + 1][i] = estate; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies the correct cards are in the player's hand
	printf("\nhand actual: ");
	if ( testG.handCount[thisPlayer] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][j]);
			if (j == 0 || j == testG.handCount[thisPlayer] - 1) { assertPassed(1, testG.hand[thisPlayer][j], G.deck[thisPlayer][4]); }
			else { assertPassed(1, testG.hand[thisPlayer][j], G.hand[thisPlayer][4]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.handCount[thisPlayer], 0); }
	printf("hand expected:");
	for (i = 0; i < G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn; i++) {
		if (i == 0 || i == G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn - 1) { printf("\n\t(%d)", G.deck[thisPlayer][4]); }
		else { printf("\n\t(%d)", G.hand[thisPlayer][4]); }
	}
	printf("\n");

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][i]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][i]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	for (i = 0; i < 2; i++) { printf("\n\t(%d)", G.deck[thisPlayer + 1][i]); }
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 9:
				Player to left has 2 victory cards to reveal in deck,
				each named differently, duchy and province, 5 cards total
				in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 9:\n\nPlayer to left has 2 victory cards to reveal in deck, each named differently, duchy and province, 5 cards total in deck.\n\n");

	// setup
	numCardsDrawn = 4;
	for (i = 0; i < G.deckCount[thisPlayer + 1] - 2; i++) { G.deck[thisPlayer + 1][i] = curse; }
	G.deck[thisPlayer + 1][3] = duchy;
	G.deck[thisPlayer + 1][4] = province;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies the correct cards are in the player's hand
	printf("\nhand actual: ");
	if ( testG.handCount[thisPlayer] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][j]);
			if (j < 2 || j > testG.handCount[thisPlayer] - 3) { assertPassed(1, testG.hand[thisPlayer][j], G.deck[thisPlayer][4]); }
			else { assertPassed(1, testG.hand[thisPlayer][j], G.hand[thisPlayer][4]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.handCount[thisPlayer], 0); }
	printf("hand expected:");
	for (i = 0; i < G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn; i++) {
		if (i == 0 || i == G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn - 1) { printf("\n\t(%d)", G.deck[thisPlayer][4]); }
		else { printf("\n\t(%d)", G.hand[thisPlayer][4]); }
	}
	printf("\n");

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][4]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][3]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer + 1][4]);
	printf("\n\t(%d)", G.deck[thisPlayer + 1][3]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 10:
				Player to left has 1 action card to reveal in deck, minion,
				and 1 treasure card to reveal in deck, silver, 5 cards
				total in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 10:\n\nPlayer to left has 1 action card to reveal in deck, minion, and 1 treasure card to reveal in deck, silver, 5 cards total in deck.\n\n");

	// setup
	numActionsEarned = 2;
	numCoinsEarned = 2;
	numCardsDrawn = 0;
	for (i = 0; i < G.deckCount[thisPlayer + 1] - 2; i++) { G.deck[thisPlayer + 1][i] = curse; }
	G.deck[thisPlayer + 1][3] = minion;
	G.deck[thisPlayer + 1][4] = silver;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][4]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][3]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer + 1][4]);
	printf("\n\t(%d)", G.deck[thisPlayer + 1][3]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 11:
				Player to left has 1 treasure card to reveal in deck, gold,
				and 1 victory card to reveal in deck, gardens, 5 cards
				total in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 11:\n\nPlayer to left has 1 treasure card to reveal in deck, gold, and 1 victory card to reveal in deck, gardens, 5 cards total in deck.\n\n");

	// setup
	bonus = 0;
	numActionsEarned = 0;
	numCoinsEarned = 2;	
	numCardsDrawn = 2;
	for (i = 0; i < G.deckCount[thisPlayer + 1] - 2; i++) { G.deck[thisPlayer + 1][i] = curse; }
	G.deck[thisPlayer + 1][3] = gold;
	G.deck[thisPlayer + 1][4] = gardens;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies the correct cards are in the player's hand
	printf("\nhand actual: ");
	if ( testG.handCount[thisPlayer] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][j]);
			if (j == 0 || j == testG.handCount[thisPlayer] - 1) { assertPassed(1, testG.hand[thisPlayer][j], G.deck[thisPlayer][4]); }
			else { assertPassed(1, testG.hand[thisPlayer][j], G.hand[thisPlayer][4]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.handCount[thisPlayer], 0); }
	printf("hand expected:");
	for (i = 0; i < G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn; i++) {
		if (i == 0 || i == G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn - 1) { printf("\n\t(%d)", G.deck[thisPlayer][4]); }
		else { printf("\n\t(%d)", G.hand[thisPlayer][4]); }
	}
	printf("\n");

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][4]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][3]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer + 1][4]);
	printf("\n\t(%d)", G.deck[thisPlayer + 1][3]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 12:
				Player to left has 1 victory card to reveal in deck,
				great_hall, and 1 action card to reveal in deck,
				council_room, 5 cards total in deck.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 12:\n\nPlayer to left has 1 victory card to reveal in deck, great_hall, and 1 action card to reveal in deck, council_room, 5 cards total in deck.\n\n");

	// setup
	bonus = 0;
	numActionsEarned = 2;
	numCoinsEarned = 0;	
	numCardsDrawn = 2;
	for (i = 0; i < G.deckCount[thisPlayer + 1] - 2; i++) { G.deck[thisPlayer + 1][i] = curse; }
	G.deck[thisPlayer + 1][3] = great_hall;
	G.deck[thisPlayer + 1][4] = council_room;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n");

	cardEffect(tribute, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("current player:\n\n");

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// verifies correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	// verifies correct number of actions
	printf("actions = %d, expected = %d\t\t", testG.numActions, G.numActions + numActionsEarned);
	assertPassed(1, testG.numActions, G.numActions + numActionsEarned);

	// verifies correct number of buys
	printf("buys = %d, expected = %d\t\t\t", testG.numBuys, G.numBuys + numBuysEarned);
	assertPassed(1, testG.numBuys, G.numBuys + numBuysEarned);

	// verifies correct number of coins	
	printf("coin = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	// verifies the correct cards are in the player's hand
	printf("\nhand actual: ");
	if ( testG.handCount[thisPlayer] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][j]);
			if (j == 0 || j == testG.handCount[thisPlayer] - 1) { assertPassed(1, testG.hand[thisPlayer][j], G.deck[thisPlayer][4]); }
			else { assertPassed(1, testG.hand[thisPlayer][j], G.hand[thisPlayer][4]); }
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.handCount[thisPlayer], 0); }
	printf("hand expected:");
	for (i = 0; i < G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn; i++) {
		if (i == 0 || i == G.handCount[thisPlayer] - numCardsPlayed + numCardsDrawn - 1) { printf("\n\t(%d)", G.deck[thisPlayer][4]); }
		else { printf("\n\t(%d)", G.hand[thisPlayer][4]); }
	}
	printf("\n");

	printf("\n\nplayer to left:\n\n");

	// verifies correct number of cards in player to left's deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);
	assertPassed(1, testG.deckCount[thisPlayer + 1], G.deckCount[thisPlayer + 1] + numCardsShuffled - numCardsRevealed);

	// verifies correct number of cards in player to left's hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);
	assertPassed(1, testG.handCount[thisPlayer + 1], G.handCount[thisPlayer + 1]);

	// verifies correct number of cards in player to left's discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);
	assertPassed(1, testG.discardCount[thisPlayer + 1], G.discardCount[thisPlayer + 1] - numCardsShuffled + numCardsRevealed);

	// verifies the correct cards are in the player to left's discard
	printf("\ndiscard actual: ");
	if ( testG.discardCount[thisPlayer + 1] > 0 ) {
		printf("\n");
		for (j = 0; j < testG.discardCount[thisPlayer + 1]; j++) {
			printf("\t(%d)\t\t\t\t", testG.discard[thisPlayer + 1][j]);
			if (j == testG.discardCount[thisPlayer + 1] - 2) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][4]); }
			if (j == testG.discardCount[thisPlayer + 1] - 1) { assertPassed(1, testG.discard[thisPlayer + 1][j], G.deck[thisPlayer + 1][3]); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer + 1], 0); }
	printf("discard expected:");
	printf("\n\t(%d)", G.deck[thisPlayer + 1][4]);
	printf("\n\t(%d)", G.deck[thisPlayer + 1][3]);
	printf("\n");

	printf("\n\n------------------------------------------------------------\n");


	printf("\n >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}