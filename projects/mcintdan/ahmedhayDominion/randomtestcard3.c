/* -----------------------------------------------------------------------
 * Danielle McIntosh
 * CS362 Section 400
 * Assignment 4
 * Due Date: 11 August 2019
 * 
 * Unit test for tributeEffect function
 *
 * Include the following lines in makefile:
 *
 * randomtestcard3: randomtestcard3.c dominion.o rngs.o
 *      gcc -o randomtestcard3 -g  randomtestcard3.c dominion.o rngs.o $(CFLAGS)
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
#include <time.h>


#define TESTCARD "tribute"
#define MAX_TESTS 200


/************************************************************************
 * assertPassed
 *		verifies equality/inequality of two values to determine whether
 *			test case passed or failed
 ************************************************************************/
void assertPassed(int assertType, int value1, int value2) {
	if (assertType) {		// testing for equality
		if (value1 != value2) { printf("--- FAILED ---\n"); }
	}
	else {					// testing for inequality
		if (value1 == value2) { printf("--- FAILED ---\n"); }
	}
}

/************************************************************************
 * checkCard
 *		verifies expected behavior for the particular card being tested
 ************************************************************************/
void checkCard(int *n, int choice1, int choice2, int choice3, struct gameState *post, int handpos, int *bonus) {
	struct gameState pre;
	int i = 0, j = 0;

    for (j = 0; j < post->numPlayers; j++) {
		if (j != post->whoseTurn) {
		    post->deckCount[j] = (rand() % MAX_DECK);
		    for (i = 0; i < post->deckCount[j]; i++) {
		    	post->deck[j][i] = rand() % 28;
		    }
		    post->discardCount[j] = (rand() % MAX_DISCARD);
		    for (i = 0; i < post->discardCount[j]; i++) {
		    	post->discard[j][i] = rand() % 28;
		    }
		    post->handCount[j] = (rand() % MAX_HAND);
		    for (i = 0; i < post->handCount[j]; i++) {
		    	post->hand[j][i] = rand() % 28;
		    }				
		}
	}

	memcpy(&pre, post, sizeof(struct gameState));
	int r;
	int nextPlayer = pre.whoseTurn + 1;
	if (nextPlayer == pre.numPlayers) { nextPlayer = 0; }
	int tributeRevealedCards[2] = {-5, -5};
	(*n)++;

	// print information about test case
	printf("test #%d\n", *n);
	printf("pre.numPlayers: %d\n", pre.numPlayers);
	printf("pre.whoseTurn: %d\n", pre.whoseTurn);
	printf("pre.deckCount[pre.whoseTurn]: %d\n", pre.deckCount[pre.whoseTurn]);
	printf("pre.handCount[pre.whoseTurn]: %d\n", pre.handCount[pre.whoseTurn]);
	printf("pre.discardCount[pre.whoseTurn]: %d\n", pre.discardCount[pre.whoseTurn]);

	r = cardEffect(tribute, choice1, choice2, choice3, post, handpos, bonus);

	// if player only has 0 or 1 card between their deck and discard piles
	if ((pre.discardCount[nextPlayer] + pre.deckCount[nextPlayer]) <= 1) {
		if (pre.deckCount[nextPlayer] > 0) {     // if they have 1 card in their deck
			tributeRevealedCards[0] = pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1];
			pre.deckCount[nextPlayer]--;
		}
		else if (pre.discardCount[nextPlayer] > 0) {   // if they have 1 card in their discard
			tributeRevealedCards[0] = pre.discard[nextPlayer][pre.discardCount[nextPlayer] - 1];
			pre.discardCount[nextPlayer]--;
		}
	}
	else {
		if (pre.deckCount[nextPlayer] == 0) {
			for (i = 0; i < pre.discardCount[nextPlayer]; i++) {
				pre.deck[nextPlayer][i] = pre.discard[nextPlayer][i];   // move to deck
				pre.deckCount[nextPlayer]++;
				pre.discardCount[nextPlayer]--;
			}
			shuffle(nextPlayer, &pre);    // shuffle the deck
		} 
		tributeRevealedCards[0] = pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1];
		pre.deckCount[nextPlayer]--;
		tributeRevealedCards[1] = pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1];
		pre.deckCount[nextPlayer]--;
	}    

	// if we have a duplicate card, just drop one 
	if ((tributeRevealedCards[0] == tributeRevealedCards[1]) && tributeRevealedCards[0] != -5) {   
		pre.playedCards[pre.playedCardCount] = tributeRevealedCards[1];
		pre.playedCardCount++;
		tributeRevealedCards[1] = -5;
	}

	for (i = 0; i < 2; i++) {
		if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold) { pre.coins += 2; }    
		else if ((tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy) || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall) {   
			drawCard(pre.whoseTurn, &pre);
			drawCard(pre.whoseTurn, &pre);
		}
		else if (tributeRevealedCards[i] != -5 && tributeRevealedCards[i] != curse) { pre.numActions += 2; }
	}

	pre.discardCount[pre.whoseTurn]++;	// increase by 1 for played card
	pre.discard[pre.whoseTurn][pre.discardCount[pre.whoseTurn] - 1] = pre.hand[pre.whoseTurn][0];
	pre.hand[pre.whoseTurn][0] = pre.hand[pre.whoseTurn][pre.handCount[pre.whoseTurn] - 1];
	pre.handCount[pre.whoseTurn]--;		// decrease by 1 for played card

	assertPassed(1, r, 0);		// confirm function does not fail
	r = memcmp(&pre, post, sizeof(struct gameState));
	assertPassed(1, r, 0);		// confirm expected outcome
	printf("\n");
}

/***********************************************************************/
int main() {

	srand(time(0));
    int seed = 1000;
    int testNo = 0;
    int i = 0; //, j = 0;
    int n = 0, p = 0;
    int numPlayers = 0;
    int k[10] = {ambassador, baron, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	struct gameState G;
	int choice1 = -1, choice2 = -1, choice3 = -1;
	int handpos = 0, bonus = 0;

	printf("\n----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n");
	printf(" >>>>> RANDOM TESTING [%s] <<<<<\n", TESTCARD);
	printf("\n------------------------------------------------------------\n\n\n");

	for (n = 0; n < MAX_TESTS; n++) {
		numPlayers = (rand() % 3);
		numPlayers += 2;
	    p = (rand() % numPlayers);

		// initialize a game state and player cards
		initializeGame(numPlayers, k, seed, &G);

		G.numPlayers = numPlayers;
		G.whoseTurn = p;   

	    G.deckCount[p] = (rand() % MAX_DECK);
	    for (i = 0; i < G.deckCount[p]; i++) {
	    	G.deck[p][i] = rand() % 28;
	    }
	    G.discardCount[p] = (rand() % MAX_DISCARD);
	    for (i = 0; i < G.discardCount[p]; i++) {
	    	G.discard[p][i] = rand() % 28;
	    }
	    G.handCount[p] = (rand() % MAX_HAND);
	    for (i = 0; i < G.handCount[p]; i++) {
	    	G.hand[p][i] = rand() % 28;
	    }

	    bonus = (rand() % 256);
	 
		// verify the card works as expected with random values	
	    checkCard(&testNo, choice1, choice2, choice3, &G, handpos, &bonus);		
	}

	printf("\n------------------------------------------------------------\n");
	printf("\n >>>>> RANDOM TESTING [%s] COMPLETE <<<<< \n", TESTCARD);
	printf("----------------------------------------------------------------------\n");
	printf("----------------------------------------------------------------------\n\n");

	return 0;
}