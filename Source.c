//Name: Liran Cordova
//ID: 314921651


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define CARDTYPE_1 		1
#define CARDTYPE_2 		2
#define CARDTYPE_3 		3
#define CARDTYPE_4 		4
#define CARDTYPE_5 		5
#define CARDTYPE_6 		6
#define CARDTYPE_7 		7
#define CARDTYPE_8 		8
#define CARDTYPE_9 		9
#define CARDTYPE_TAKI 	10
#define CARDTYPE_COLOR 	11
#define CARDTYPE_DIR 	12
#define CARDTYPE_PLUS 	13
#define CARDTYPE_STOP 	14
#define CARDCOLOR_G 	15
#define CARDCOLOR_R 	16
#define CARDCOLOR_Y 	17
#define CARDCOLOR_B	 	18

/* Struct for saving card information*/
struct cardinfo
{
	int CardType;
	int CardColor;
};
typedef struct cardinfo CARDINFO;

/*Struct for saving Player information*/
struct playerinfo
{
	char firstName[10];
	CARDINFO* PlayerCards;
	int PhSize;
	int PlayerNofCards;

};
typedef struct playerinfo PLAYERINFO;

/*Pointer to arrayy of player information*/
PLAYERINFO* PlayerInfo;

CARDINFO UpperCardInfo;

/*Struct for saving card statistics*/
struct cardstatistic
{
	int CardType;
	int CardFreq;
};
typedef struct cardstatistic CARDSTATISTIC;

/*Pointer to card statistics */
CARDSTATISTIC* CardStatistic;

/*Functions declaration*/
void FirstCardsDist(PLAYERINFO* PInfo);
void gameflow(int NumOfPlayers);
void PrintPlayerCards(PLAYERINFO* PInfo);
void PrintCard(int Type, int Color);
void RemovePlayerCard(PLAYERINFO* PInfo, int PlayerChoice);
void PickCard(PLAYERINFO* PInfo);
void PutFirstCard();



int main()
{
	/*Main function*/

	int i, j, NumOfPlayers, tempType, tempFreq;

	/*Memory allocation for cards statistics array*/
	CardStatistic = (CARDSTATISTIC*)malloc(14 * sizeof(CARDSTATISTIC));
	if (CardStatistic == NULL)
	{
		printf("Memory allocation failed!!!\n");
		exit(1);
	}

	/*Initialize cards statistics array*/
	for (i = 0; i < 14; i++)
	{
		CardStatistic[i].CardType = i + 1;
		CardStatistic[i].CardFreq = 0;
	}

	printf("************  Welcome to TAKI game  !!! ************\n");
	printf("Please enter the number of players:\n");
	scanf("%d", &NumOfPlayers);
	printf("\n");

	/*Memory allocation for player information array*/
	PlayerInfo = (PLAYERINFO*)malloc(NumOfPlayers * sizeof(PLAYERINFO));
	if (PlayerInfo == NULL)
	{
		printf("Memory allocation failed!!!\n");
		exit(1);
	}

	/*Players data input and saving in allocated memory*/
	for (i = 0; i < NumOfPlayers; i++)
	{
		printf("Please enter the first name of player #%d:\n", i + 1);
		scanf("%s", PlayerInfo[i].firstName);
		printf("\n");
		PlayerInfo[i].PlayerCards = (CARDINFO*)malloc(4 * sizeof(CARDINFO));
		if (PlayerInfo[i].PlayerCards == NULL)
		{
			printf("Memory allocation failed!!!\n");
			exit(1);
		}

		PlayerInfo[i].PhSize = 4;
		PlayerInfo[i].PlayerNofCards = 4;
		FirstCardsDist(&PlayerInfo[i]);

	}
	printf("\n");

	/*Call for first card to put on deck function*/
	PutFirstCard();

	/*Call for the main flow of the game*/
	gameflow(NumOfPlayers);

	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");

	/*Sorting card statistics data from high to low*/
	for (i = 0; i <= 12; i++)
		for (j = i + 1; j <= 13; j++)
		{
			if (CardStatistic[i].CardFreq < CardStatistic[j].CardFreq)
			{
				tempType = CardStatistic[i].CardType;
				tempFreq = CardStatistic[i].CardFreq;
				CardStatistic[i].CardType = CardStatistic[j].CardType;
				CardStatistic[i].CardFreq = CardStatistic[j].CardFreq;
				CardStatistic[j].CardType = tempType;
				CardStatistic[j].CardFreq = tempFreq;
			}
		}

	/*Printing the card statistics table*/
	for (i = 0; i < 14; i++)
	{
		if (CardStatistic[i].CardType > 0 && CardStatistic[i].CardType < 10)
			printf("   %d   |    %d\n", CardStatistic[i].CardType, CardStatistic[i].CardFreq);
		else if (CardStatistic[i].CardType == 10)
			printf(" TAKI  |    %d\n", CardStatistic[i].CardFreq);
		else if (CardStatistic[i].CardType == 11)
			printf(" COLOR |    %d\n", CardStatistic[i].CardFreq);
		else if (CardStatistic[i].CardType == 12)
			printf("  <->  |    %d\n", CardStatistic[i].CardFreq);
		else if (CardStatistic[i].CardType == 13)
			printf("   +   |    %d\n", CardStatistic[i].CardFreq);
		else if (CardStatistic[i].CardType == 14)
			printf(" STOP  |    %d\n", CardStatistic[i].CardFreq);
	}

	/*Free all allocated memory*/
	free(CardStatistic);
	free(PlayerInfo);
	for (i = 0; i < NumOfPlayers; i++)
		free(PlayerInfo[i].PlayerCards);
}


void gameflow(int NumOfPlayers)
{
	/*This function is the main flow of the game.
	  It iterates forever until a player wins*/

	int CardNo, u;
	int PIndex = 0;
	bool Direction = true, ValidColorChoise = true;
	int PlayerChoice;
	int ColorChoice = -1;
	bool CardTypePlus = false;
	bool PutCard = true;
	bool NextPlayer = true;
	bool StartTaki = false;

	while (true)
	{
		PutCard = true;
		NextPlayer = true;
		CardTypePlus = false;

		while (PutCard == true)
		{
			/*Print cards of a player per turn*/
			PrintPlayerCards(&PlayerInfo[PIndex]);

			if (NextPlayer == true || CardTypePlus == true)
				printf("Please enter 0 if you want to take a card from the deck\n");
			else
				printf("Please enter 0 if you want to finish your turn\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", PlayerInfo[PIndex].PlayerNofCards);

			/*Player input choice according to the above instructions*/
			scanf("%d", &PlayerChoice);
			if (PlayerChoice == 0)
			{
				/*The player choose to take a card from the deck or to just finish his turn*/

				printf("Upper card:\n");
				PrintCard(UpperCardInfo.CardType, UpperCardInfo.CardColor);
				if (NextPlayer == true)
					PickCard(&PlayerInfo[PIndex]);
				PutCard = false;
				StartTaki = false;
			}
			else if (PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardType == CARDTYPE_COLOR)
			{
				/*Player choose to put a COLOR card in the middle*/

				if (UpperCardInfo.CardType == CARDTYPE_TAKI && StartTaki == true)
				{
					printf("Invalid card! Try again.\n");
					PutCard = true;
				}

				else
				{
					/*Player choosen is valid and game continue*/

					UpperCardInfo.CardType = CARDTYPE_COLOR;
					ValidColorChoise = false;
					ColorChoice = 0;
					while (ValidColorChoise == false)
					{
						/*Continue to ask for entering the color choice until it is a valid choise*/

						if (ColorChoice == 0)
							printf("Please enter your color choice:\n");
						else
							printf("Invalid color choice. Please enter your color choice:\n");
						printf("1 - Yellow\n");
						printf("2 - Red\n");
						printf("3 - Blue\n");
						printf("4 - Green\n");
						scanf("%d", &ColorChoice);
						printf("\n");

						if (ColorChoice >= 1 && ColorChoice <= 4)
						{
							/*Valid color choice*/

							ValidColorChoise = true;
							if (ColorChoice == 1)
								UpperCardInfo.CardColor = CARDCOLOR_Y;
							if (ColorChoice == 2)
								UpperCardInfo.CardColor = CARDCOLOR_R;
							if (ColorChoice == 3)
								UpperCardInfo.CardColor = CARDCOLOR_B;
							if (ColorChoice == 4)
								UpperCardInfo.CardColor = CARDCOLOR_G;
						}
						else
						{
							/*Invalid color choice*/

							ValidColorChoise = false;
							if (ColorChoice == 0)
								ColorChoice = -1;
						}
					}

					/*Print the new upper card*/
					printf("Upper card:\n");
					PrintCard(UpperCardInfo.CardType, UpperCardInfo.CardColor);
					printf("\n");

					/*Remove the card that was choosen to be in the middle from the player struct*/
					RemovePlayerCard(&PlayerInfo[PIndex], PlayerChoice);
					PutCard = false;
				}
			}
			else
			{
				/*Any other card type was choosen*/

				if (PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardType == UpperCardInfo.CardType || PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardColor == UpperCardInfo.CardColor)
				{
					/*At least the type or the color of the choosen card match to the upper card*/

					if (UpperCardInfo.CardType == CARDTYPE_TAKI && StartTaki == true && (PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardColor != UpperCardInfo.CardColor || PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardType == CARDTYPE_COLOR))
					{
						/*During a fall of cards on TAKI, the player can't put on the TAKI
						 either a card that doesn't match to the TAKI's color or a COLOR card*/

						printf("Invalid card! Try again.\n");
						PutCard = true;
					}
					else
					{
						/*The card that was choosen to be on the TAKI is valid*/

						/*Update the upper card info and print it*/
						UpperCardInfo.CardType = PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardType;
						UpperCardInfo.CardColor = PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardColor;
						printf("Upper card:\n");
						PrintCard(UpperCardInfo.CardType, UpperCardInfo.CardColor);
						printf("\n");

						if (PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardType == CARDTYPE_TAKI || PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardType == CARDTYPE_PLUS)
						{
							if (PlayerInfo[PIndex].PlayerCards[PlayerChoice - 1].CardType == CARDTYPE_PLUS)
								CardTypePlus = true;
							else
							{
								CardTypePlus = false;
								StartTaki = true;
							}
							NextPlayer = false;
							PutCard = true;
						}
						else
							if (StartTaki == false)
								PutCard = false;
						RemovePlayerCard(&PlayerInfo[PIndex], PlayerChoice);
					}
				}
				else
				{
					printf("Invalid card! Try again.\n");
					PutCard = true;
				}
			}
		}
		if (PlayerInfo[PIndex].PlayerNofCards == 0 && UpperCardInfo.CardType != CARDTYPE_PLUS)
		{
			printf("The winner is... %s! Congratulations!\n\n", PlayerInfo[PIndex].firstName);
			break;
		}

		//Handling the next round's player choosen
		if (UpperCardInfo.CardType == CARDTYPE_DIR && PlayerChoice != 0)
		{
			if (Direction)
			{
				Direction = false;
				if (PIndex == 0)
					PIndex = NumOfPlayers - 1;
				else
					PIndex--;
			}
			else
			{
				Direction = true;
				if (PIndex == NumOfPlayers - 1)
					PIndex = 0;
				else
					PIndex++;
			}
		}
		else if (UpperCardInfo.CardType == CARDTYPE_STOP && PlayerChoice != 0)
		{
			if (Direction)
			{
				if (PIndex == NumOfPlayers - 2)
					PIndex = 0;
				else if (PIndex == NumOfPlayers - 1)
					PIndex = 1;
				else
					PIndex += 2;
			}
			else
			{
				if (PIndex == 1)
					PIndex = NumOfPlayers - 1;
				else if (PIndex == 0)
					PIndex = NumOfPlayers - 2;
				else
					PIndex -= 2;
			}
		}
		else
		{
			if (Direction)
			{
				if (PIndex == NumOfPlayers - 1)
					PIndex = 0;
				else
					PIndex++;
			}
			else
			{
				if (PIndex == 0)
					PIndex = NumOfPlayers - 1;
				else
					PIndex--;
			}
		}
	}
}


void PrintPlayerCards(PLAYERINFO* PInfo)
{
	/*This function prints the player's cards by calling the "PrintCard" function*/

	int i;
	printf("%s%s\n\n", (*PInfo).firstName, "'s turn:");
	for (i = 0; i < (*PInfo).PlayerNofCards; i++)
	{
		printf("Card #%d\n", i + 1);
		PrintCard((*PInfo).PlayerCards[i].CardType, (*PInfo).PlayerCards[i].CardColor);
	}
}

void PrintCard(int Type, int Color)
{
	/*This function prints one card*/

	char sColor;

	if (Color == 15)
		sColor = 'G';
	else if (Color == 16)
		sColor = 'R';
	else if (Color == 17)
		sColor = 'Y';
	else if (Color == 18)
		sColor = 'B';
	else
		sColor = ' ';


	printf("*********\n");
	printf("*       *\n");
	if (Type == CARDTYPE_TAKI)
		printf("*  %s *\n", "TAKI");
	else if (Type == CARDTYPE_COLOR)
		printf("* %s *\n", "COLOR");
	else if (Type == CARDTYPE_DIR)
		printf("*  %s  *\n", "<->");
	else if (Type == CARDTYPE_PLUS)
		printf("*   %s   *\n", "+");
	else if (Type == CARDTYPE_STOP)
		printf("* %s  *\n", "STOP");
	else
		printf("*   %d   *\n", Type);

	if (Type != CARDTYPE_COLOR || Color != -1)
	{
		if (Color == CARDCOLOR_G)
			printf("*   %c   *\n", 'G');
		else if (Color == CARDCOLOR_R)
			printf("*   %c   *\n", 'R');
		else if (Color == CARDCOLOR_Y)
			printf("*   %c   *\n", 'Y');
		else if (Color == CARDCOLOR_B)
			printf("*   %c   *\n", 'B');
	}
	else
		printf("*       *\n");
	printf("*       *\n");
	printf("*********\n\n");
}


void RemovePlayerCard(PLAYERINFO* PInfo, int PlayerChoice)
{
	/*Remove one card from the players's info struct*/

	int i;

	for (i = PlayerChoice; i < (*PInfo).PlayerNofCards; i++)
	{
		(*PInfo).PlayerCards[i - 1] = (*PInfo).PlayerCards[i];
	}
	(*PInfo).PlayerNofCards--;
}


void PickCard(PLAYERINFO* PInfo)
{
	/*This function pick one card from the deck*/

	int i;

	(*PInfo).PlayerNofCards++;
	if ((*PInfo).PlayerNofCards - 1 == (*PInfo).PhSize)
	{
		(*PInfo).PhSize++;
		PInfo->PlayerCards = realloc(PInfo->PlayerCards, PInfo->PlayerNofCards * sizeof(CARDINFO));
		if (PInfo->PlayerCards == NULL)
		{
			printf("Memory allocation failed!!!\n");
			exit(1);
		}
	}

	srand(time(NULL));

	(*PInfo).PlayerCards[(*PInfo).PlayerNofCards - 1].CardType = 1 + rand() % 14;
	if (PInfo->PlayerCards[(*PInfo).PlayerNofCards - 1].CardType == CARDTYPE_COLOR)
		(*PInfo).PlayerCards[(*PInfo).PlayerNofCards - 1].CardColor = -1;
	else
		(*PInfo).PlayerCards[(*PInfo).PlayerNofCards - 1].CardColor = 15 + rand() % 4;

	(CardStatistic[(*PInfo).PlayerCards[(*PInfo).PlayerNofCards - 1].CardType - 1].CardFreq)++;
}

void FirstCardsDist(PLAYERINFO* PInfo)
{
	/*This function chooses "randomaly" (uniform distribution) 4 cards per player.
	  This function is being called once at the beginning of the game*/

	int i;

	srand(time(NULL));

	for (i = 0; i < 4; i++)
	{
		PInfo->PlayerCards[i].CardType = 1 + rand() % 14;
		if (PInfo->PlayerCards[i].CardType == CARDTYPE_COLOR)
			PInfo->PlayerCards[i].CardColor = -1;
		else
			PInfo->PlayerCards[i].CardColor = 15 + rand() % 4;
	}
}

void PutFirstCard()
{
	/*This function chooses "randomaly" (uniform distribution) the first card to be in the middle.
	  This function is being called once at the beginning of the game*/

	srand(time(NULL));

	UpperCardInfo.CardType = 1 + rand() % 9;
	UpperCardInfo.CardColor = 15 + rand() % 4;
	printf("Upper card:\n");
	PrintCard(UpperCardInfo.CardType, UpperCardInfo.CardColor);
}