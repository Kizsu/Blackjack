/*
    Tyler Kennedy
    02 December 2019
    COP2220
    Final Project - Blackjack game
*/

#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <random>

using namespace std;

// Declarations

// An enumeration to hold the four available suits
enum class Suit;

// Structure to hold the required values of a card
struct Card;

// Clears the screen independent of Windows/Unix by spamming 30 lines of new lines
void clearScreen();

// Returns a Card vector with the four available suits
vector<Card> getCardSet(int value, string name);
// Returns a fully populated deck, vector, of Cards with all available suits and values
vector<Card> populateDeck();

// Returns if the player has won in the current situation
bool didPlayerWin(vector<Card> playerHand, vector<Card> dealerHand);
// Returns if the an integer vector contains the given integer value
bool containsInt(vector<int> vec);

// Returns a card from the "available" cards vector, appending the newly obtained Card index value from the "available" vector to the "unavailable" vector
Card getRandomAvailableCard(vector<int> &unavailable, vector<Card> available);

// Returns a random number between {min} and {max} with the minimum number defaulting to 1
int getRandomNumber(int max, int min=0);
// Returns the total value of the given "hand", Card vector
int getHandValue(vector<Card> hand);

// Returns a string representation of the Suit enumeration
string getSuitAsString(Suit s);
// Returns a string representation of a vector holding Cards
string getHandAsString(vector<Card> hand);

// Definitions

enum class Suit
{
    HEARTS,
    SPADES,
    CLUBS,
    DIAMONDS
};

struct Card
{
    Card(Suit cs, string name, int value)
    {
        this->cardName = name;
        this->cardSuit = cs;
        this->cardValue = value;
    }

    Suit cardSuit;
    string cardName;
    int cardValue;
};

void clearScreen()
{
    for (int i = 0; i <= 30; i++)
    {
        cout << endl;
    }
}

vector<Card> getCardSet(int value, string name)
{
    vector<Card> cardSet;

    cardSet.push_back(Card(Suit::CLUBS, name, value));
    cardSet.push_back(Card(Suit::DIAMONDS, name, value));
    cardSet.push_back(Card(Suit::HEARTS, name, value));
    cardSet.push_back(Card(Suit::SPADES, name, value));

    return cardSet;
}

vector<Card> populateDeck()
{
    vector<Card> cards = {};
    vector<Card> cardSet = {};

    for (int i = 1; i <= 13; i++)
    {
        switch (i)
        {
        // Ace
        case 1:
            cardSet = getCardSet(1, "Ace");
            cards.insert(cards.end(), cardSet.begin(), cardSet.end());
            break;
        // Jack
        case 11:
            cardSet = getCardSet(10, "Jack");
            cards.insert(cards.end(), cardSet.begin(), cardSet.end());
            break;
        // Queen
        case 12:
            cardSet = getCardSet(10, "Queen");
            cards.insert(cards.end(), cardSet.begin(), cardSet.end());
            break;
        // King
        case 13:
            cardSet = getCardSet(10, "King");
            cards.insert(cards.end(), cardSet.begin(), cardSet.end());
            break;
        // Default to setting the name of the card to the value
        default:
            cardSet = getCardSet(i, to_string(i));
            cards.insert(cards.end(), cardSet.begin(), cardSet.end());
            break;
        }
    }

    return cards;
}

bool didPlayerWin(vector<Card> playerHand, vector<Card> dealerHand)
{
    bool playerWon = false;
    int dealerValue = 0, playerValue = 0;

    dealerValue = getHandValue(dealerHand);
    playerValue = getHandValue(playerHand);

    // Win/Loss conditions
    if (playerValue > 21 || dealerValue == 21 || (dealerValue != 21 && playerValue < dealerValue))
    {
        playerWon = false;
    }
    else
    {
        playerWon = true;
    }

    return playerWon;
}

bool containsInt(vector<int> vec, int val)
{
    bool found = false;

    for (int i : vec)
    {
        if (i == val)
        {
            found = true;
            break;
        }
    }
    return found;
}

Card getRandomAvailableCard(vector<int> &unavailable, vector<Card> available)
{
    int randomNumber = 0;
    randomNumber = getRandomNumber(52);

    while (containsInt(unavailable, randomNumber))
    {
        randomNumber = getRandomNumber(52);
    }

    Card nextCard = available.at(randomNumber);
    // Add the now unavailable index to the unavailable vector
    unavailable.push_back(randomNumber);

    return nextCard;
}

int getRandomNumber(int max, int min)
{
    int num;

    num = rand() % max + min;

    return num;
}

int getHandValue(vector<Card> hand)
{
    int value = 0;
    bool hasAce = false;

    for (Card c : hand)
    {
        value += c.cardValue;

        if (c.cardName == "Ace")
        {
            hasAce = true;
        }
    }

    // If the player has an ace and with it the value is closer to 21 and not over return it as +10
    if (hasAce && value + 10 <= 21)
    {
        value += 10;
    }

    return value;
}

string getSuitAsString(Suit s)
{
    string suit;

    switch (s)
    {
    case Suit::HEARTS:
        suit = "Hearts";
        break;
    case Suit::SPADES:
        suit = "Spades";
        break;
    case Suit::CLUBS:
        suit = "Clubs";
        break;
    case Suit::DIAMONDS:
        suit = "Diamonds";
        break;
    }

    return suit;
}

string getHandAsString(vector<Card> hand)
{
    string handString = "";

    for (Card c : hand)
    {
        handString += c.cardName + " of " + getSuitAsString(c.cardSuit) + "\n";
    }

    return handString;
}

int main()
{
    // Seed random with time
    srand(time(0));

    // Initialize all the variables to be used
    vector<Card> completeDeck = populateDeck();

    vector<Card> playerHand = {};
    vector<Card> dealerHand = {};

    vector<int> unavailableCards = {};

    bool isRunning = true;
    bool checkVictory = false;
    bool resetGame = false;

    string input = "";

    while (isRunning)
    {
        clearScreen();

        // Victory/Defeat condition checking
        bool playerWin = didPlayerWin(playerHand, dealerHand);
        int dealerValue = getHandValue(dealerHand);
        int playerValue = getHandValue(playerHand);

        if (!resetGame && (dealerValue > 21 || (checkVictory && playerWin)))
        {
            cout << "Congratulations, you won this game of blackjack!" << endl;
            checkVictory = true;
        }
        else if (!resetGame && (playerValue > 21 || (checkVictory && !playerWin)))
        {
            cout << "Unfortunately you have lost this game of blackjack..., better luck next time." << endl;
            checkVictory = true;
        }

        if (!resetGame && checkVictory)
        {
            cout << "Your hand value was " + to_string(playerValue) << endl;
            cout << "Dealer's hand value was " + to_string(dealerValue) << endl;
            cout << "\nWould you like to play again (Y/N)? ";
            cin >> input;
            // Clear cin so it pauses when cin.get() is called
            cin.ignore();

            switch (toupper(input.at(0)))
            {
            case 'Y':
                // Reset all the variables that need to be reset
                resetGame = true;
                checkVictory = false;
                break;
            case 'N':
                // End the game
                checkVictory = false;
                isRunning = false;
                break;
            default:
                // Inform the user the input they gave is not a valid choice
                cout << "You have entered an unavailable option, try again. Press any key to continue..." << endl;
                cin.get();
                break;
            }
        }

        else
        {
            if (!checkVictory && (resetGame || playerHand.size() < 2 || dealerHand.size() < 2))
            {
                // Reset the vectors
                unavailableCards = {};
                playerHand = {};
                dealerHand = {};

                // Distribute cards to the dealer and the player
                playerHand.push_back(getRandomAvailableCard(unavailableCards, completeDeck));
                playerHand.push_back(getRandomAvailableCard(unavailableCards, completeDeck));
                dealerHand.push_back(getRandomAvailableCard(unavailableCards, completeDeck));
                dealerHand.push_back(getRandomAvailableCard(unavailableCards, completeDeck));

                resetGame = false;
            }

            // Output the player's current hand
            cout << "Your hand is currently:" << endl;
            cout << getHandAsString(playerHand);

            // Get what the player's choice of action will be
            cout << "What would you like to do (H/S/E)? ";
            cin >> input;
            // Clear cin so it pauses when cin.get() is called
            cin.ignore();

            switch (toupper(input.at(0)))
            {
            case 'H':
                // Give the player and the dealer a new card, if the player does not already have five cards
                if (!(playerHand.size() >= 5))
                {
                    playerHand.push_back(getRandomAvailableCard(unavailableCards, completeDeck));
                    dealerHand.push_back(getRandomAvailableCard(unavailableCards, completeDeck));
                }
                else
                {
                    cout << "You may not have more than five cards at a time. Press any key to continue..." << endl;
                    cin.get();
                }
                break;
            case 'S':
                // Check whether the player or the dealer has won
                checkVictory = true;
                break;
            case 'E':
                // Stop playing blackjack
                isRunning = false;
                break;
            default:
                // Inform the user the input they gave is not a valid choice
                cout << "You have entered an unavailable option, try again. Press any key to continue..." << endl;
                cin.get();
                break;
            }
        }
    }

    cout << "Thanks for playing!" << endl;

    return 0;
}
