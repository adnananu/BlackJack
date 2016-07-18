//
// Created by Adnan on 13-Jul-16.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define DECK_SIZE 52
#define TURN_ARR_SIZE 9
static bool AceUp = false;  //keep track that Ace is 11 or 1 (hard or soft)

//get random number in range
int getRandom(int limit){
    return (int)((double)rand() / ((double)RAND_MAX + 1) * limit);
}

//return value of KING QUEEN JACK otherwise return same value
int getValue_JQK(int tValue)
{
    if ((tValue%100 == 11) ||(tValue%100 == 12) ||(tValue%100 == 13))
        return (tValue/100)*100 + 10;
    else
        return tValue;
}

//function initialize deck and shuffles the card
void ShuffleCards(int *cards) {
    // local variables
    int r, temp, i;

    for (i = 0; i < DECK_SIZE; i++)
        cards[i] = (i / 13 + 3) * 100 + i % 13 + 1;

    i = DECK_SIZE;
    srand (time(NULL));
    while(--i >= 0){  //fisher yates shuffle
        r = getRandom(i);
        //swapping
        temp = cards[r];
        cards[r] = cards[i];
        cards[i] = temp;
    }

}

//function display the cards value
void displayCards(int *arrCards, bool hideFirstCard){
    // local variables
    int i;
    if(hideFirstCard)
    {
        i = 1;
        printf("**** "); //hiding first card
    }
    else
        i = 0;

    for(i; i < TURN_ARR_SIZE; i++)
    {
        if(arrCards[i] != 0)
        {
            switch(arrCards[i]%100)
            {
                case 1:
                    printf(" A ");
                    break;
                case 11:
                    printf(" J ");
                    break;
                case 12:
                    printf(" Q ");
                    break;
                case 13:
                    printf(" K ");
                    break;
                //For non-face cards, just using 10 value as rank.
                default:
                    printf(" %2d ", arrCards[i]%100);
            }
            printf("");
        }
        else
            break;
    }
    printf("\n");
}

//returns next available card
int getTopCard(int *card){
    // local variables
    int i,tCard = 0;
    for(i = 0; i < DECK_SIZE; i++) {
        if(card[i] != 0){
            tCard = card[i]; 
            card[i] = 0;
            break;
        }
    }
    return tCard;    
}

//checks who is final winner
void finalWinner(int BotValue, int PlayerValue){
    if (BotValue > 21 || PlayerValue > BotValue){
        printf("Player won!\n");
        EXIT_SUCCESS;
    }
    else if (PlayerValue == BotValue){
        printf("Game finished, Player and Computer have same score!\n");
        EXIT_FAILURE;
    }
    else if (PlayerValue < BotValue){
        printf("Computer won!\n");
        EXIT_SUCCESS;
    }
}

//checks if player won
int ifPlayerWins(int tValue){
    if (tValue > 21){
        printf("Total of player cards: %d\n\n",tValue);
        printf("Player score is greater than 21.\n");
        printf("Computer won!\n");
        EXIT_SUCCESS;
    }
    else if (tValue == 21){
        printf("Total of player cards now: %d\n\n",tValue);
        printf("Player score equals to 21.\n");
        printf("Player won!\n");
        EXIT_SUCCESS;
    }
}

//returns reasonable Ace value
int getAceValue(int tPoints, bool isAceCard){
    if(tPoints <= 11 && isAceCard){
        AceUp = true;
        tPoints += 10;
    }
    else if(AceUp && tPoints > 21){
        AceUp = false;
        tPoints -= 10;
    }
    return tPoints;
}

//return's total score of first two cards
int getFirstTwoCardValue(int *inHandCards){
    int i, cardSum = 0;
    for (i = 0; i < 2; i++)
    {
        if (inHandCards[i]%100 == 1) {
            cardSum = cardSum + 1;
            cardSum = getAceValue(cardSum,true);
        }
        else
            cardSum = cardSum + getValue_JQK(inHandCards[i]) % 100;
    }
    return cardSum;
}

//return total with score of new card
int getCardsSum(int newCardsPoint, int oldCardPoints){

    if (newCardsPoint % 100 == 1) {
        oldCardPoints = oldCardPoints + 1;
        oldCardPoints = getAceValue(oldCardPoints,true);
    }
    else {
        oldCardPoints = oldCardPoints + getValue_JQK(newCardsPoint) % 100;
        oldCardPoints = getAceValue(oldCardPoints,false);
    }
    return  oldCardPoints;// return final total
}

// function starts the game
int startGame(){
    // local variables
    int playerSum = 0, botSum = 0, i = 0;
    int playerCards[TURN_ARR_SIZE]={0};
    int botCards[TURN_ARR_SIZE]={0};
    int cards[DECK_SIZE];
    bool bTakeMore = true;

    printf("Welcome to BlackJack!\n\n\n");
    ShuffleCards(cards);
    //give the cards to player & bot
    playerCards[0] = getTopCard(cards);
    botCards[0] = getTopCard(cards);
    playerCards[1] = getTopCard(cards);
    botCards[1] = getTopCard(cards);;


    printf("Computer cards:\n");
    displayCards(botCards,true);
    printf("\nPlayer cards:\n");
    displayCards(playerCards,false);
    //get sum of first 2 cards
    playerSum = getFirstTwoCardValue(playerCards);

    if(playerSum == 21)    //natural blackjack win check
        bTakeMore = false;
    printf("Total of player cards now: %d\n\n",playerSum);
    if(bTakeMore){
        for (i = 0; i < TURN_ARR_SIZE - 2; i++){
        //maximum 9 times a player can get card to reach 21 points, 2 already taken then 7 left so (TURN_ARR_SIZE - 2)
            char UserInput = 'n';
            printf("if you want to get a card? Enter 'y' otherwise 'n':\n");
            do{
                UserInput = getchar();
            } while (UserInput != 'y' && UserInput != 'n');

            if (UserInput == 'y' || UserInput == 'Y')
            {
                printf("Cards in player hand now:\n");
                playerCards[i + 2] = getTopCard(cards);// giving new card
                displayCards(playerCards,false);
                playerSum = getCardsSum(playerCards[i+2],playerSum);
                if(ifPlayerWins(playerSum) == 0)
                    return 1;

                printf("Total of player's cards now: %d\n\n",playerSum);
            }
            else{
                printf("Total of player's cards now: %d\n\n",playerSum);
                break;
            }
        }
    }
    else
        printf("Player has natural blackjack (21 Score), now Computer's turn!\n");

    // if player chooses No more cards then computer turn
    printf("Computer Turn!\n");
    printf("Computer cards:\n");
    displayCards(botCards,false);
    AceUp = false;
    botSum = getFirstTwoCardValue(botCards);//get total of first two cards
    printf("Total of computer cards now: %d\n\n",botSum);

    //computer gets another card until botSum becomes greater than 16
    //maximum 9 times a player can get card to reach 21 points
    // 2 already taken then 7 left so (TURN_ARR_SIZE - 2)
    for (i=0; i <  TURN_ARR_SIZE - 2 && botSum < 17; i++){

        botCards[i+2] = getTopCard(cards);
        printf("Computer with new card are:\n");
        displayCards(botCards,false);
        botSum = getCardsSum(botCards[i + 2],botSum); //return total after adding new card score
        printf("Total of computer cards now: %d\n\n", botSum);
    }
    //check the final winner of game
    finalWinner(botSum, playerSum);
}



int main(void)
{
    char startGameAgain;
    startGame();
    printf("\nWould you like to play again? Enter 'y' otherwise 'n':\n");
    do{
        startGameAgain = getchar();
    } while (startGameAgain != 'y' && startGameAgain != 'n');
    if (startGameAgain == 'y'){
        printf("\nNew game started!\n\n");
        main();
    }

    return 0;
}