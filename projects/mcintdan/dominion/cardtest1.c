/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 3
 * Due Date: 14 July 2019
 * 
 * Unit test for initializeGame function
 *
 * Include the following lines in makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
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


#define TESTFUNCTION "initializeGame"


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
    int numPlayers = -1;
	int k1[10] = {ambassador, baron, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	struct gameState testG;
	int i = -1, j = -1, m = -1, result = -1;
	int success = 0, fail = -1;
	int numCurses = -1, numVictories = -1, numTreasures = -1, numKingdomCards = -1;
	int numEstates = -1, numCoppers = -1, numCoins = -1;

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> TESTING [%s] <<<<<\n", TESTFUNCTION);


	/*	TEST CASE 1:
				Invalid number of players entered, too few players.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 1:\nInvalid number of players entered, too few players.\n\n");

	// verify game cannot be initialized with invalid number of players
	numPlayers = 1;
	result = initializeGame(numPlayers, k1, seed, &testG);
	printf("number of players = %d\n", numPlayers);
	printf("game initialization outcome = %d, expected = %d\t\t\t", result, fail);
	assertPassed(1, result, fail);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 2:
				Invalid number of players entered, too many players.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 2:\nInvalid number of players entered, too many players.\n\n");

	// verify game cannot be initialized with invalid number of players
	numPlayers = 5;
	result = initializeGame(numPlayers, k1, seed, &testG);
	printf("number of players = %d\n", numPlayers);
	printf("game initialization outcome = %d, expected = %d\t\t\t", result, fail);
	assertPassed(1, result, fail);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 3:
				Invalid selection of kingdom cards, kingdom cards not
				unique.
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 3:\nInvalid selection of kingdom cards, kingdom cards not unique.\n\n");

	// verify game cannot be initialized with invalid selection of kingdom cards
	numPlayers = 2;
	int k2[10] = {baron, baron, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	result = initializeGame(numPlayers, k2, seed, &testG);
	printf("kingdom cards actual = ");
	for (i = 0; i < 10; i ++) {
		printf("(%d)\t\t", k2[i]);
	}
	printf("\n\n");
	printf("game initialization outcome = %d, expected = %d\t\t\t", result, fail);
	assertPassed(1, result, fail);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 4:
				Valid selection of kingdom cards, kingdom cards unique. 
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 4:\nValid selection of kingdom cards, kingdom cards unique.\n\n");

	int k[10] = {ambassador, baron, village, minion, mine, cutpurse, sea_hag, tribute, smithy, gardens};
	result = initializeGame(numPlayers, k, seed, &testG);
	printf("kingdom cards actual = ");
	for (i = 0; i < 10; i ++) {
		printf("(%d)\t\t", k[i]);
	}
	printf("\n\n");

	// verify game can be initialized with valid selection of kingdom cards
	printf("game initialization outcome = %d, expected = %d\t\t\t", result, success);
	assertPassed(1, result, success);

	printf("\n\n------------------------------------------------------------\n");


	/*	TEST CASE 5:
				Valid number of players entered, number of players within
				range [2, 4]. 
	***********************************************************************/
	printf("\n------------------------------------------------------------\n\n");
	printf("TEST 5:\nValid number of players entered, number of players within range [2, 4].\n\n");

	for (i = 2; i < 5; i++) {
		numCoins = 0;
		numPlayers = i;
		printf("number of players = %d\n\n", numPlayers);
		result = initializeGame(numPlayers, k, seed, &testG);

		// verify game can be initialized with valid number of players
		printf("game initialization outcome = %d, expected = %d\t\t\t", result, success);
		assertPassed(1, result, success);

		// verify correct number of players in game
		printf("number of players actual = %d, expected = %d\t\t\t", testG.numPlayers, numPlayers);
		assertPassed(1, testG.numPlayers, numPlayers);

		// verify correct number of curse cards
		if (i == 2) { numCurses = 10; } else if (i == 3) { numCurses += 10; } else { numCurses += 10; }
		printf("supply of curses actual = %d, expected = %d\t\t\t", testG.supplyCount[curse], numCurses);
		assertPassed(1, testG.supplyCount[curse], numCurses);

		// verify correct number of victory cards
		if (i == 2) { numVictories = 8; } else { numVictories = 12; }
		printf("supply of estates actual = %d, expected = %d\t\t\t", testG.supplyCount[estate], numVictories);
		assertPassed(1, testG.supplyCount[estate], numVictories);
		printf("supply of duchys actual = %d, expected = %d\t\t\t", testG.supplyCount[duchy], numVictories);
		assertPassed(1, testG.supplyCount[duchy], numVictories);
		printf("supply of provinces actual = %d, expected = %d\t\t\t", testG.supplyCount[province], numVictories);
		assertPassed(1, testG.supplyCount[province], numVictories);

		// verify correct number of treasure cards
		numTreasures = 30;
		printf("supply of coppers actual = %d, expected = %d\t\t\t", testG.supplyCount[copper], ((numTreasures *= 2) - (7 * numPlayers)));
		assertPassed(1, testG.supplyCount[copper], numTreasures - (7 * numPlayers));
		printf("supply of silvers actual = %d, expected = %d\t\t\t", testG.supplyCount[silver], numTreasures -= 20);
		assertPassed(1, testG.supplyCount[silver], numTreasures);
		printf("supply of golds actual = %d, expected = %d\t\t\t", testG.supplyCount[gold], numTreasures -= 10);
		assertPassed(1, testG.supplyCount[gold], numTreasures);

		// verify correct number of kingdom cards
		for (j = adventurer; j <= treasure_map; j++) {
			for (m = 0; m < 10; m++) {
				if (k[m] == j) {
					if (k[m] == great_hall || k[m] == gardens) {
						if (i == 2) { numKingdomCards = 8; }
						else { numKingdomCards = 12; }
					}
					else { numKingdomCards = 10; }
					break;
				}
				else { numKingdomCards = -1; }
			}
			printf("supply of (%d) actual = %d, expected = %d\t\t\t", j, testG.supplyCount[j], numKingdomCards);
			if (j == 10 && numPlayers == 2) { printf("\t"); }		// fixing weird alignment problem
			assertPassed(1, testG.supplyCount[j], numKingdomCards);
		}

		// verify correct number of estates and coppers in each player's deck
		for (j = 0; j < numPlayers; j++) {
			numEstates = 0;
			numCoppers = 0;
			printf("\nplayer %d:\n", j + 1);
			for (m = 0; m < 10; m++) {
				if (testG.deck[j][m] == estate) { numEstates++; }
				if (testG.deck[j][m] == copper) { numCoppers++; }
			}
			printf("number of estates actual = %d, expected = %d\t\t\t", numEstates, 3);
			assertPassed(1, numEstates, 3);
			printf("number of coppers actual = %d, expected = %d\t\t\t", numCoppers, 7);
			assertPassed(1, numCoppers, 7);
		}

		// verify each player's deck can be shuffled
		for (j = 0; j < numPlayers; j++) {
			result = shuffle(j, &testG);
			printf("\nplayer %d:\n", j + 1);
			printf("deck shuffle outcome = %d, expected = %d\t\t\t\t", result, success);
			assertPassed(1, result, success);
		}		

		// verify each player's hand and discard are empty
		for (j = 0; j < numPlayers; j++) {
			if (j == 0) {
				printf("\nplayer %d:\n\n", j + 1);
				printf("number cards in hand actual = %d, expected = %d\t\t\t", testG.handCount[j], 5);
				assertPassed(1, testG.handCount[j], 5);
			}
			else {
				printf("\nplayer %d:\n\n", j + 1);
				printf("number cards in hand actual = %d, expected = %d\t\t\t", testG.handCount[j], 0);
				assertPassed(1, testG.handCount[j], 0);
			}
			printf("number cards in discard actual = %d, expected = %d\t\t", testG.discardCount[j], 0);
			assertPassed(1, testG.discardCount[j], 0);
		}
		printf("\n");		

		// verify correct number of embargo tokens for each kingdom card
		for (j = 0; j <= treasure_map; j++) {
			printf("number of (%d) embargo tokens actual = %d, expected = %d\t\t", j, testG.embargoTokens[j], 0);
			assertPassed(1, testG.embargoTokens[j], 0);
		}
		printf("\n");

		// verify the remaining state variables for first player's turn can be initialized
		printf("outpost played actual = %d, expected = %d\t\t\t\t", testG.outpostPlayed, 0);
		assertPassed(1, testG.outpostPlayed, 0);
		printf("phase actual = %d, expected = %d\t\t\t\t\t", testG.phase, 0);
		assertPassed(1, testG.phase, 0);
		printf("number of actions actual = %d, expected = %d\t\t\t", testG.numActions, 1);
		assertPassed(1, testG.numActions, 1);
		printf("number of buys actual = %d, expected = %d\t\t\t\t", testG.numBuys, 1);
		assertPassed(1, testG.numBuys, 1);
		for (j = 0; j < testG.handCount[0]; j++) {
			if (testG.hand[0][j] == copper) { numCoins++; }
			else if (testG.hand[0][j] == silver) { numCoins += 2; }
			else if (testG.hand[0][j] == gold) { numCoins += 3; }
		}
		printf("number of coins actual = %d, expected = %d\t\t\t", testG.coins, numCoins);
		assertPassed(1, testG.coins, numCoins);
		printf("played card count actual = %d, expected = %d\t\t\t", testG.playedCardCount, 0);
		assertPassed(1, testG.playedCardCount, 0);
		printf("whose turn actual = %d, expected = %d\t\t\t\t", testG.whoseTurn, 0);
		assertPassed(1, testG.whoseTurn, 0);
		printf("hand count actual = %d, expected = %d\t\t\t\t", testG.handCount[testG.whoseTurn], 5);
		assertPassed(1, testG.handCount[testG.whoseTurn], 5);

	printf("\n\n------------------------------------------------------------\n\n");
	}

	printf(" >>>>> TESTING [%s] COMPLETE <<<<< \n", TESTFUNCTION);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}