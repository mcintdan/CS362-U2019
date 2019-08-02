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


#define TESTCARD "minion"


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
	int numCoinsEarned = 0, numActionsEarned = 1, numBuysEarned = 0;
	int numCardsDrawn = 0, numCardsShuffled = 0;
	int numCardsGainedToDeck = 0, numCardsGainedToHand = 0, numCardsGainedToDiscard = 0;
	int numCardsPlayed = 1, numCardsDiscarded = 0;  
	struct gameState G, testG;
	int i = 0;//, j = 0;
	int choice1 = -1, choice2 = -1, choice3 = -1, handpos = 0, bonus = 0;
	int k[10] = {ambassador, baron, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);


	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTCARD);


	/*	TEST CASE 1:
			choice1 = 1 and choice2 = 0
				Player chooses to gain $2.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\n\tchoice1 = 1 and choice2 = 0\n\n");
	printf("Player chooses to gain $2.\n");

	// setup
	choice1 = 1;
	choice2 = 0;
	numCoinsEarned = 2;
	G.hand[thisPlayer][0] = minion;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

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
	printf("coins = %d, expected = %d\t\t\t", bonus, numCoinsEarned);
	assertPassed(1, bonus, numCoinsEarned);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
			choice1 = 0 and choice1 = 1
				Player chooses to discard their hand, draw 4 cards, and
				each other player with at least 5 cards in hand discards
				their hand and draws 4 cards.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\n\tchoice1 = 0 and choice2 = 1\n\n");
	printf("Player chooses to discard their hand, draw 4 cards, and each other player with at least 5 cards in hand discards their hand and draws 4 cards.\n");

	// setup
	choice1 = 0;
	choice2 = 1;
	numCoinsEarned = 0;

	// player 1 has 5 cards in hand, 1 minion and 4 curses, 5 cards in deck
	G.hand[thisPlayer][0] = minion;		
	for (i = 1; i < G.handCount[thisPlayer]; i++) { G.hand[thisPlayer][i] = curse; }

	// player 2 has 5 cards in hand, all curses, 10 cards in deck, all coppers
	G.deckCount[G.whoseTurn + 1] = 10;
	G.handCount[G.whoseTurn + 1] = 5;	
	for (i = 0; i < G.deckCount[G.whoseTurn + 1]; i++) { G.deck[G.whoseTurn + 1][i] = copper; }
	for (i = 0; i < G.handCount[G.whoseTurn + 1]; i++) { G.hand[G.whoseTurn + 1][i] = curse; }

	// player 2 has 5 cards in hand, all curses, 10 cards in deck, all coppers
	G.deckCount[G.whoseTurn + 2] = 10;
	G.handCount[G.whoseTurn + 2] = 5;	
	for (i = 0; i < G.deckCount[G.whoseTurn + 2]; i++) { G.deck[G.whoseTurn + 2][i] = copper; }
	for (i = 0; i < G.handCount[G.whoseTurn + 2]; i++) { G.hand[G.whoseTurn + 2][i] = curse; }

	// player 4 has a hand of only 4 cards, all curses, 5 cards in deck
	G.deckCount[G.whoseTurn + 3] = 5;
	G.handCount[G.whoseTurn + 3] = 4;
	for (i = 0; i < G.handCount[G.whoseTurn + 3]; i++) { G.hand[G.whoseTurn + 3][i] = curse; }

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(minion, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf("\n\n");

// tests state change for current player
	printf("player 1\n\n");

	numCardsDrawn = 4;
	numCardsDiscarded = 4;

	// tests current player's discarded cards are no longer in their hand
	printf("hand actual: ");
	if ( testG.handCount[thisPlayer] > 0 ) {
		printf("\n");	
		for (i = 0; i < testG.handCount[thisPlayer]; i++) {
			printf("\t(%d)\t\t\t\t", testG.hand[thisPlayer][i]);
			assertPassed(0, testG.hand[thisPlayer][i], curse);
		}
	}
	else { printf("[empty]\n"); }
	printf("hand expected:");
	for (i = G.deckCount[thisPlayer] - 1; i > G.deckCount[thisPlayer] - 5; i--) {
		printf("\n\t(%d)", G.deck[thisPlayer][i]);
	}
	printf("\n");

	// test current player's discarded cards are now in their discard
	printf("discard actual: ");
	if ( testG.discardCount[thisPlayer] > 0 ) {
		printf("\n");
		for (i = 0; i < testG.discardCount[thisPlayer]; i++) {
			printf("\t(%d)\t\t\t", testG.discard[thisPlayer][i]);
			if (i == testG.discardCount[thisPlayer] - 1) { assertPassed(1, testG.discard[thisPlayer][i], minion); }
			else { assertPassed(1, testG.discard[thisPlayer][i], curse); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[thisPlayer], 0); }
	printf("discard expected:");
	for (i = 0; i < G.handCount[thisPlayer]; i++) {
		printf("\n\t(%d)", G.hand[thisPlayer][i]);
	}
	printf("\n");

	// tests current player has correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// tests current player has correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// tests current player has correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	printf("\n\n\n");

// tests state change for player 2
	printf("player 2\n\n");

	numCardsPlayed = 0;
	numCardsDiscarded = 5;

	// tests player's discarded cards are no longer in their hand
	printf("hand actual: ");
	if ( testG.handCount[testG.whoseTurn + 1] > 0 ) {
		printf("\n");	
		for (i = 0; i < testG.handCount[testG.whoseTurn + 1]; i++) {
			printf("\t(%d)\t\t\t\t", testG.hand[testG.whoseTurn + 1][i]);
			assertPassed(0, testG.hand[testG.whoseTurn + 1][i], curse);
		}
	}
	else { printf("[empty]\n"); }
	printf("hand expected:");
	for (i = G.deckCount[G.whoseTurn + 1] - 1; i > G.deckCount[G.whoseTurn + 1] - 5; i--) {
		printf("\n\t(%d)", G.deck[G.whoseTurn + 1][i]);
	}
	printf("\n");

	// test player's discarded cards are now in their discard
	printf("discard actual: ");
	if ( testG.discardCount[testG.whoseTurn + 1] > 0 ) {
		printf("\n");
		for (i = 0; i < testG.discardCount[testG.whoseTurn + 1]; i++) {
			printf("\t(%d)\t\t\t", testG.discard[testG.whoseTurn + 1][i]);
			if (i == testG.discardCount[testG.whoseTurn + 1] - 1) { assertPassed(1, testG.discard[testG.whoseTurn + 1][i], minion); }
			else { assertPassed(1, testG.discard[testG.whoseTurn + 1][i], curse); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[testG.whoseTurn + 1], 0); }
	printf("discard expected:");
	for (i = 0; i < G.handCount[G.whoseTurn + 1]; i++) {
		printf("\n\t(%d)", G.hand[G.whoseTurn + 1][i]);
	}
	printf("\n");

	// tests player has correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[testG.whoseTurn + 1], G.deckCount[G.whoseTurn + 1] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[testG.whoseTurn + 1], G.deckCount[G.whoseTurn + 1] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// tests player has correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[testG.whoseTurn + 1], G.handCount[G.whoseTurn + 1] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[testG.whoseTurn + 1], G.handCount[G.whoseTurn + 1] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// tests player has correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[testG.whoseTurn + 1], G.discardCount[G.whoseTurn + 1] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[testG.whoseTurn + 1], G.discardCount[G.whoseTurn + 1] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	printf("\n\n\n");

// tests state change for player 3
	printf("player 3\n\n");

	numCardsPlayed = 0;
	numCardsDiscarded = 5;

	// tests player's discarded cards are no longer in their hand
	printf("hand actual: ");
	if ( testG.handCount[testG.whoseTurn + 2] > 0 ) {
		printf("\n");	
		for (i = 0; i < testG.handCount[testG.whoseTurn + 2]; i++) {
			printf("\t(%d)\t\t\t\t", testG.hand[testG.whoseTurn + 2][i]);
			assertPassed(0, testG.hand[testG.whoseTurn + 2][i], curse);
		}
	}
	else { printf("[empty]\n"); }
	printf("hand expected:");
	for (i = G.deckCount[G.whoseTurn + 2] - 1; i > G.deckCount[G.whoseTurn + 2] - 5; i--) {
		printf("\n\t(%d)", G.deck[G.whoseTurn + 2][i]);
	}
	printf("\n");

	// test player's discarded cards are now in their discard
	printf("discard actual: ");
	if ( testG.discardCount[testG.whoseTurn + 2] > 0 ) {
		printf("\n");
		for (i = 0; i < testG.discardCount[testG.whoseTurn + 2]; i++) {
			printf("\t(%d)\t\t\t", testG.discard[testG.whoseTurn + 2][i]);
			if (i == testG.discardCount[testG.whoseTurn + 2] - 1) { assertPassed(1, testG.discard[testG.whoseTurn + 2][i], minion); }
			else { assertPassed(1, testG.discard[testG.whoseTurn + 2][i], curse); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[testG.whoseTurn + 2], 0); }
	printf("discard expected:");
	for (i = 0; i < G.handCount[G.whoseTurn + 2]; i++) {
		printf("\n\t(%d)", G.hand[G.whoseTurn + 2][i]);
	}
	printf("\n");

	// tests player has correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[testG.whoseTurn + 2], G.deckCount[G.whoseTurn + 2] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[testG.whoseTurn + 2], G.deckCount[G.whoseTurn + 2] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// tests player has correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[testG.whoseTurn + 2], G.handCount[G.whoseTurn + 2] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[testG.whoseTurn + 2], G.handCount[G.whoseTurn + 2] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// tests player has correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[testG.whoseTurn + 2], G.discardCount[G.whoseTurn + 2] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[testG.whoseTurn + 2], G.discardCount[G.whoseTurn + 2] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	printf("\n\n\n");

// tests state change for player 4
	printf("player 4\n\n");

	numCardsDrawn = 0;
	numCardsDiscarded = 0;

	// tests player's hand still has the same cards
	printf("hand actual: ");
	if ( testG.handCount[testG.whoseTurn + 3] > 0 ) {
		printf("\n");	
		for (i = 0; i < testG.handCount[testG.whoseTurn + 3]; i++) {
			printf("\t(%d)\t\t\t\t", testG.hand[testG.whoseTurn + 3][i]);
			assertPassed(1, testG.hand[testG.whoseTurn + 3][i], curse);
		}
	}
	else { printf("[empty]\n"); }
	printf("hand expected:");
	for (i = G.handCount[G.whoseTurn + 3] - 1; i > G.handCount[G.whoseTurn + 3] - 5; i--) {
		printf("\n\t(%d)", G.hand[G.whoseTurn + 3][i]);
	}
	printf("\n");

	// test player's discard is empty
	printf("discard actual: ");
	if ( testG.discardCount[testG.whoseTurn + 3] > 0 ) {
		printf("\n");
		for (i = 0; i < testG.discardCount[testG.whoseTurn + 3]; i++) {
			printf("\t(%d)\t\t\t", testG.discard[testG.whoseTurn + 3][i]);
			if (i == testG.discardCount[testG.whoseTurn + 3] - 1) { assertPassed(1, testG.discard[testG.whoseTurn + 3][i], minion); }
			else { assertPassed(0, testG.discard[testG.whoseTurn + 3][i], curse); }			
		}
	}
	else { printf("[empty]\t\t\t"); assertPassed(1, testG.discardCount[testG.whoseTurn + 3], 0); }
	printf("discard expected: [empty]\n");

	// tests current player has correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[testG.whoseTurn + 3], G.deckCount[G.whoseTurn + 3] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[testG.whoseTurn + 3], G.deckCount[G.whoseTurn + 3] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// tests current player has correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[testG.whoseTurn + 3], G.handCount[G.whoseTurn + 3] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);
	assertPassed(1, testG.handCount[testG.whoseTurn + 3], G.handCount[G.whoseTurn + 3] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded);

	// tests current player has correct number of cards in discard
	printf("discard count = %d, expected = %d\t\t", testG.discardCount[testG.whoseTurn + 3], G.discardCount[G.whoseTurn + 3] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);
	assertPassed(1, testG.discardCount[testG.whoseTurn + 3], G.discardCount[G.whoseTurn + 3] + numCardsPlayed + numCardsDiscarded + numCardsGainedToDiscard);

	printf("\n\n\n");

	printf("------------------------------------------------------------\n");


	printf("\n >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}