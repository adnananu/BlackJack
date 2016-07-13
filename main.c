//
// Created by Adnan on 13-Jul-16.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define DECK_SIZE 52
#define TURN_ARR_SIZE 9

//return value of KING QUEEN JACK otherwise return same value
int getValue_JQK(int tValue)
{
    if ((tValue%100 == 11) ||(tValue%100 == 12) ||(tValue%100 == 13))
        return (tValue/100)*100 + 10;
    else
        return tValue;
}

//function initialize deck and shuffles the card
void ShuffleCards(int *cards){
    // local variables
    int tempVar,i;
    int tempArray[DECK_SIZE];

    for (i = 0; i < DECK_SIZE; i++)
        tempArray[i] = (i/13+3)*100 + i%13 + 1;
    
    srand (time(NULL));
    for (i = 0; i < DECK_SIZE; i++)
    {
        do{
            tempVar = rand() % DECK_SIZE;
        } while (tempArray[tempVar] == 0);

        cards[i] = tempArray[tempVar];
        tempArray[tempVar] = 0;
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
        printf("Player and Computer have same score!\n");
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

// function starts the game
int startGame()
{
    // local variables
    int playerSum = 0, botSum = 0, i = 0;
    int playerCards[TURN_ARR_SIZE]={0};
    int botCards[TURN_ARR_SIZE]={0};
    int cards[DECK_SIZE];

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
    //check if player win's with first 2 cards
    for (i; i < 2; i++)
    {
        if (playerCards[i]%100 == 1)
              playerSum = playerSum + (((playerSum + 11) <= 21) ? 11 : 1);//taking reasonable value of ACE
        else if (getValue_JQK(playerCards[i]) %100 ==10)
                    playerSum = playerSum + 10;
        else
            playerSum = playerSum + playerCards[i]%100;

        if(ifPlayerWins(playerSum) == 0)
            return 1;
    }
    printf("Total of player cards now: %d\n\n",playerSum);
    for (i = 0; i < TURN_ARR_SIZE - 2; i++){
    //maximum 9 times a player can get card to reach 21 points, 2 already taken so 7 left so (TURN_ARR_SIZE - 2)
        char UserInput = 'n';
        printf("if you want to get a card? Enter y otherwise n:\n");
        do{
            UserInput = getchar();
        } while (UserInput != 'y' && UserInput != 'Y' && UserInput != 'n' && UserInput != 'N');

        if (UserInput == 'y' || UserInput == 'Y')
        {
            printf("Cards in player hand now:\n");
            playerCards[i+2] = getTopCard(cards);// giving new card
            displayCards(playerCards,false);

            if (playerCards[i+2]%100 == 1)
                playerSum = playerSum + (((playerSum + 11) <= 21) ? 11 : 1);//taking reasonable value of ACE
            else if (getValue_JQK(playerCards[i+2]) % 100 == 10)
                playerSum = playerSum + 10;
            else
                playerSum = playerSum + playerCards[i+2] % 100;

            if(ifPlayerWins(playerSum) == 0)
                return 1;
            printf("Total of player's cards now: %d\n\n",playerSum);
        }
        else{
            printf("Total of player's cards now: %d\n\n",playerSum);
            break;
        }
    }
    // if player chooses NO more cards then computer turn
    printf("Computer cards:\n");
    displayCards(botCards,false);
    if ((botCards[0] % 100 + botCards[1] % 100) == 2)//if both cards of computer are Ace's
    {
        botSum=12;
        printf("Total of computer cards now: %d\n\n", botSum);
    }
    else if (botCards[0]%100==1 || botCards[1]%100==1)
    {
        botSum=(botCards[0]+botCards[1])%100+(rand()%2)*10;
        printf("Total of computer cards now: %d\n\n", botSum);
    }
    else
    {
        botSum = (getValue_JQK(botCards[0]))%100 + (getValue_JQK(botCards[1]))%100;
        printf("Total of computer cards now: %d\n\n", botSum);
    }

    //computer gets another card until botSum becomes greater than 16
    for (i=0; i <  TURN_ARR_SIZE - 2 && botSum < 17; i++){
        //maximum 9 times a player can get card to reach 21 points, 2 already taken so 7 left so (TURN_ARR_SIZE - 2)
        botCards[i+2] = getTopCard(cards);
        printf("Computer cards with new card are:\n");
        displayCards(botCards,false);
        if (botCards[i+2]%100 == 1)
            botSum = botSum + (((botSum + 11) <= 21) ? 11 : 1);//taking reasonable value of ACE
        else
            botSum = botSum + getValue_JQK(botCards[i+2])%100;
        
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