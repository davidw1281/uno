#include "card.h"
#include <vector>
#include <memory>

class Player {
    private:
        const int playerNo;
        std::vector<std::unique_ptr<Card>> deck;
        Uno& game;

    public:
        Player(int playerNo, Uno& game) : playerNo(playerNo), game(game) {}

        void addCard(std::unique_ptr<Card> cardPointer) {
            this->deck.push_back(cardPointer);
        }

        void drawCards(int cardCount) {
            for (int i = 0; i < cardCount; i++) {
                this->deck.push_back(std::move(this->game.getDrawPile().back()));
                this->game.getDrawPile().pop_back();
                //...
            }
        }

        void drawCardNoMatch();

        int pickCard();

        void useCard(std::unique_ptr<Card> cardPointer);

        void useAbility(std::unique_ptr<ActionCard> actionCardPointer);

        void maybeSayUno();

        void runTurn();

        void gameWon();
};


class Uno {
    private:
        const int playerCount;
        std::vector<Player> players;
        std::vector<std::unique_ptr<Card>> drawPile;
        std::vector<std::unique_ptr<Card>> discardPile;
        int currentTurn;
        int turnChange;
        bool gameOver;
        int winner;

    public:
        Uno(int playerCount) : playerCount(playerCount) {}

        std::vector<Player> getPlayers() { return this->players; }

        std::vector<std::unique_ptr<Card>> getDrawPile() { return this->drawPile; }

        std::vector<std::unique_ptr<Card>> getDiscardPile() { return this->discardPile; }

        void createPlayers();

        std::vector<std::unique_ptr<Card>> createCards();

        void setupCards();

        void specialFirstDiscard(std::unique_ptr<ActionCard> actionCardPointer);

        void setupGame();

        void newDrawPile();
};