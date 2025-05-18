#include "card.h"
#include <vector>
#include <algorithm>
#include <random>

class Player {
    private:
        const int playerNo;
        std::vector<Card> deck;
        Uno& game;

    public:
        Player(int playerNo, Uno& game) : playerNo(playerNo), game(game) {}

        void addCard(Card card) {
            deck.push_back(card);
        }

        void drawCards(int cardCount) {
            for (int i = 0; i < cardCount; i++) {
                std::vector<Card>& drawPile = game.getDrawPile();
                deck.push_back(drawPile.back());
                drawPile.pop_back();
                
                if (drawPile.empty()) {
                    game.newDrawPile();
                }
            }
        }

        void drawCardNoMatch();

        int pickCard();

        void useCard(Card card);

        void useAbility(Card actionCard);

        void maybeSayUno();

        void runTurn();

        void gameWon();
};


class Uno {
    private:
        const int playerCount;
        std::vector<Player> players;
        std::vector<Card> drawPile;
        std::vector<Card> discardPile;
        int currentTurn;
        int turnChange;
        bool gameOver;
        int winner;

    public:
        Uno(int playerCount) : playerCount(playerCount) {}

        std::vector<Player> getPlayers() { return players; }

        std::vector<Card>& getDrawPile() { return drawPile; }

        std::vector<Card>& getDiscardPile() { return discardPile; }

        void createPlayers();

        std::vector<Card> createCards();

        void setupCards();

        void specialFirstDiscard(Card actionCard);

        void setupGame();

        void newDrawPile() {
            std::swap(discardPile, drawPile);

            discardPile.push_back(drawPile.back());
            drawPile.pop_back();

            std::random_device rand;
            std::mt19937 g(rand());
            
            std::shuffle(drawPile.begin(), drawPile.end(), g);
        }
};