#include "card.h"

class Uno;

class Player {
    private:
        const int playerNo;
        std::vector<Card> deck;
        Uno& game;

    public:
        Player(int playerNo, Uno& game);

        void addCard(Card card);

        void removeCard(Card card);

        std::string displayDeck();

        void setColour(Card& wildCard);

        void drawCards(int cardCount);

        void drawCardNoMatch();

        int pickCardIndex();

        void useCard(Card card);

        void useAbility(Card& actionCard);

        void maybeSayUno();

        void gameWon();

        void runTurn();
};

class Uno {
    private:
        const int playerCount;
        std::vector<Player> players;
        std::vector<Card> drawPile;
        std::vector<Card> discardPile;
        int currentTurnIndex = 0;
        int turnChange = 1;
        bool gameOver = false;
        int winner = 0;

    public:
        Uno(int playerCount);

        std::vector<Player> getPlayers();

        std::vector<Card>& getDrawPile();

        std::vector<Card>& getDiscardPile();

        void reverseTurnDirection();

        void nextTurn();

        void nextPlayerDrawCards(int cardCount);

        void shuffleCards(std::vector<Card>& cards);

        void createPlayers();

        std::vector<Card> createCards();
        
        void setupCards(std::vector<Card> cards);

        void specialFirstDiscard(Card& actionCard);

        void setupGame();

        void newDrawPile();

        void gameWon(int playerNo);

        void runGame();
};