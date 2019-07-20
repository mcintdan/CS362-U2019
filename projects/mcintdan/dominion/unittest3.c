/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for ambassadorEffect function
 *
 * Include the following lines in makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
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


#define TESTCARD "ambassador"


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

/************************************************************************
 * checkOtherPlayersDiscard
 *		verifies correctness of other player's discard
 ************************************************************************/
void checkOtherPlayersDiscard(struct gameState G, struct gameState testG, int numPlayersWithGainedCard, int numPlayers) {
	int i = 0, j = 0;

	// tests state change for each player that gained a card
	for (i = 1; i <= numPlayersWithGainedCard; i++) {
		printf("\nplayer %d\n\n", i + 1);

		printf("discard actual: ");
		if ( testG.discardCount[i] > 0 ) {
			printf("\n");

			for (j = 0; j < testG.discardCount[i]; j++) {
				printf("\t(%d)\t\t\t\t", testG.discard[i][j]);

				if (j == testG.discardCount[i] - 1) { assertPassed(1, testG.discard[i][j], curse); }
				else { assertPassed(1, testG.discard[i][j], copper); }			
			}
		}
		else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[i], 0); }
		printf("discard expected:");

		for (j = 0; j < G.discardCount[i]; j++) {
			printf("\n\t(%d)", G.discard[i][j]);
		}
		printf("\n\t(%d)", curse);
		printf("\n");
	}
		// tests state change for each player that did not gain a card
	for (i = numPlayersWithGainedCard + 1; i < numPlayers; i++) {
		// if case where no other players gained a card bc current player's choice was invalid
		if (numPlayersWithGainedCard == 0) {	// increment 1st time so not looking at player 1
			i++;
			printf("\nplayer %d\n\n", i);
			i--;
		}
		else {
			printf("player %d\n\n", i);
		}

		printf("discard actual: ");
		if ( testG.discardCount[i] > 0 ) {
			printf("\n");

			for (j = 0; j < testG.discardCount[i]; j++) {
				printf("\t(%d)\t\t\t", testG.discard[i][j]);
				assertPassed(1, testG.discard[i][j], copper);		
			}
		}
		else { printf("[empty]\t\t\t"); assertPassed(0, testG.discardCount[i], 0); }
		printf("discard expected:");

		for (j = 0; j < G.discardCount[i]; j++) {
			printf("\n\t(%d)", G.discard[i][j]);
		}

		printf("\n");
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
	int numCardsPlayed = 1, numCardsReturnedToSupply = 0, numCardsDiscarded = 0; 
	int numCardsInSupply = -1;
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
			choice1 = 1 and choice2 = 0
				Player chooses to return 0 cards to the supply.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\n\tchoice1 = 1 and choice2 = 0\n\n");
	printf("Player chooses to return 0 cards to the supply.\n");

	// setup
	choice1 = 1;
	choice2 = 0;
	numCardsReturnedToSupply = 0;
	G.hand[thisPlayer][0] = ambassador;
	G.hand[thisPlayer][1] = curse;
	for (i = 2; i < G.handCount[thisPlayer]; i++) {
		G.hand[thisPlayer][i] = copper;
	}
		// every player besides current one has 5 coppers in discard
	for (i = 1; i < numPlayers; i++) {
		G.discardCount[i] = 5;
		for (j = 0; j < G.discardCount[i]; j++) {
			G.discard[i][j] = copper;
		}
	}

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);

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

	// verifies correct number of chosen card in supply and correctness of other player's discard
	numCardsInSupply = G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply - (numPlayers - 1);
	if (numCardsInSupply > 0) {
		printf("card supply count = %d, expected = %d\t", testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
		assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);

		checkOtherPlayersDiscard(G, testG, 3, numPlayers);
	}
	else {
		printf("card supply count = %d, expected = 0\t", testG.supplyCount[G.hand[thisPlayer][choice1]]);
		assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], 0);

		if (numPlayers == 4) {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 3, numPlayers);		
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 2) {
				checkOtherPlayersDiscard(G, testG, 2, numPlayers);
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 3) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}			
		}
		else if (numPlayers == 3) {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 2, numPlayers);
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 2) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}
		}
		else {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}
		}
	}

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
			choice1 = 1 and choice2 = 1
				Player chooses to return 1 card to the supply.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\n\tchoice1 = 1 and choice2 = 1\n\n");
	printf("Player chooses to return 1 card to the supply.\n");

	// setup
	choice1 = 1;
	choice2 = 1;
	numCardsReturnedToSupply = 1;
	for (i = 2; i < G.handCount[thisPlayer]; i++) {
		G.hand[thisPlayer][i] = copper;
	}
		// every player besides current one has 5 coppers in discard
	for (i = 1; i < numPlayers; i++) {
		G.discardCount[i] = 5;
		for (j = 0; j < G.discardCount[i]; j++) {
			G.discard[i][j] = copper;
		}
	}

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);

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

	// verifies correct number of chosen card in supply and correctness of other player's discard
	numCardsInSupply = G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply - (numPlayers - 1);
	if (numCardsInSupply > 0) {
		printf("card supply count = %d, expected = %d\t", testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
		assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);

		checkOtherPlayersDiscard(G, testG, 3, numPlayers);
	}
	else {
		printf("card supply count = %d, expected = 0\t", testG.supplyCount[G.hand[thisPlayer][choice1]]);
		assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], 0);

		if (numPlayers == 4) {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 3, numPlayers);		
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 2) {
				checkOtherPlayersDiscard(G, testG, 2, numPlayers);
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 3) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}			
		}
		else if (numPlayers == 3) {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 2, numPlayers);
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 2) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}
		}
		else {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}
		}
	}

	printf("\n\n------------------------------------------------------------\n");


		/*	TEST CASE 3:
			choice1 = 1 and choice2 = 2
				Player chooses to return 2 cards to the supply.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 3:\n\tchoice1 = 1 and choice2 = 2\n\n");
	printf("Player chooses to return 2 cards to the supply.\n");

	// setup
	choice1 = 1;
	choice2 = 2;
	numCardsReturnedToSupply = 2;
	G.hand[thisPlayer][2] = curse;
	for (i = 3; i < G.handCount[thisPlayer]; i++) {
		G.hand[thisPlayer][i] = copper;
	}
		// every player besides current one has 5 coppers in discard
	for (i = 1; i < numPlayers; i++) {
		G.discardCount[i] = 5;
		for (j = 0; j < G.discardCount[i]; j++) {
			G.discard[i][j] = copper;
		}
	}

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);

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

	// verifies correct number of chosen card in supply and correctness of other player's discard
	numCardsInSupply = G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply - (numPlayers - 1);
	if (numCardsInSupply > 0) {
		printf("card supply count = %d, expected = %d\t", testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
		assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);

		checkOtherPlayersDiscard(G, testG, 3, numPlayers);
	}
	else {
		printf("card supply count = %d, expected = 0\t", testG.supplyCount[G.hand[thisPlayer][choice1]]);
		assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], 0);

		if (numPlayers == 4) {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 3, numPlayers);		
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 2) {
				checkOtherPlayersDiscard(G, testG, 2, numPlayers);
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 3) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}			
		}
		else if (numPlayers == 3) {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 2, numPlayers);
			}
			else if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 2) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}
		}
		else {
			if (G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply == numPlayers - 1) {
				checkOtherPlayersDiscard(G, testG, 1, numPlayers);
			}
		}
	}

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 4:
			choice1 = 1 and choice2 = 3
				Player chooses to return 3 cards to the supply, which is
				an invalid choice.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 4:\n\tchoice1 = 1 and choice2 = 3\n\n");
	printf("Player chooses to return 3 cards to the supply, which is an invalid choice.\n");

	// setup
	choice1 = 1;
	choice2 = 3;
	numCardsReturnedToSupply = 0;
	G.hand[thisPlayer][0] = ambassador;
	G.hand[thisPlayer][1] = curse;
	for (i = 2; i < G.handCount[thisPlayer]; i++) {
		G.hand[thisPlayer][i] = copper;
	}
		// every player besides current one has 5 coppers in discard
	for (i = 1; i < numPlayers; i++) {
		G.discardCount[i] = 5;
		for (j = 0; j < G.discardCount[i]; j++) {
			G.discard[i][j] = copper;
		}
	}

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);

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

	// verifies correct number of chosen card in supply and correctness of other player's discard
	numCardsInSupply = G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply;
	printf("card supply count = %d, expected = %d\t", testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
	assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
	checkOtherPlayersDiscard(G, testG, 0, numPlayers);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 5:
			choice1 = 0 and choice2 = 1
				Player chooses to return played card ambassador to the
				supply, which is an invalid choice.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 5:\n\tchoice1 = 0 and choice2 = 1\n\n");
	printf("Player chooses to return played card ambassador to the supply, which is an invalid choice.\n");

	// setup
	choice1 = 0;
	choice2 = 1;
	numCardsReturnedToSupply = 0;
	G.hand[thisPlayer][0] = ambassador;
	for (i = 1; i < G.handCount[thisPlayer]; i++) {
		G.hand[thisPlayer][i] = copper;
	}
		// every player besides current one has 5 coppers in discard
	for (i = 1; i < numPlayers; i++) {
		G.discardCount[i] = 5;
		for (j = 0; j < G.discardCount[i]; j++) {
			G.discard[i][j] = copper;
		}
	}

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);

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

	// verifies correct number of chosen card in supply and correctness of other player's discard
	numCardsInSupply = G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply;
	printf("card supply count = %d, expected = %d\t", testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
	assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
	checkOtherPlayersDiscard(G, testG, 0, numPlayers);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 6:
			choice1 = 1 and choice2 = 2
				Player chooses to return 2 cards to the supply, which
				is an invalid choice if the player only has 1 copy in hand.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 6:\n\tchoice1 = 1 and choice2 = 2\n\n");
	printf("Player chooses to return 2 cards to the supply, which is an invalid choice if the player only has 1 copy in hand.\n");

	// setup
	choice1 = 1;
	choice2 = 2;
	numCardsReturnedToSupply = 0;
	G.hand[thisPlayer][0] = ambassador;
	G.hand[thisPlayer][1] = curse;
	for (i = 2; i < G.handCount[thisPlayer]; i++) {
		G.hand[thisPlayer][i] = copper;
	}
		// every player besides current one has 5 coppers in discard
	for (i = 1; i < numPlayers; i++) {
		G.discardCount[i] = 5;
		for (j = 0; j < G.discardCount[i]; j++) {
			G.discard[i][j] = copper;
		}
	}

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf("\n\n");

	cardEffect(ambassador, choice1, choice2, choice3, &testG, handpos, &bonus);

	// verifies correct number of cards in deck
	printf("deck count = %d, expected = %d\t\t", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);
	assertPassed(1, testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - numCardsDrawn + numCardsShuffled + numCardsGainedToDeck);

	// verifies correct number of cards in hand
	printf("hand count = %d, expected = %d\t\t", testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);
	assertPassed(1, testG.handCount[thisPlayer], G.handCount[thisPlayer] + numCardsDrawn + numCardsGainedToHand - numCardsPlayed - numCardsDiscarded - numCardsReturnedToSupply);

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

	// verifies correct number of chosen card in supply and correctness of other player's discard
	numCardsInSupply = G.supplyCount[G.hand[thisPlayer][choice1]] + numCardsReturnedToSupply;
	printf("card supply count = %d, expected = %d\t", testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
	assertPassed(1, testG.supplyCount[G.hand[thisPlayer][choice1]], numCardsInSupply);
	checkOtherPlayersDiscard(G, testG, 0, numPlayers);

	printf("\n\n------------------------------------------------------------\n");


	printf("\n >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}