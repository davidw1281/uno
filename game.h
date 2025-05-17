#include "card.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

class Player {
    private:
        const int playerNo;
        std::vector<std::unique_ptr<Card>> deck;
        Uno& game;

    public:
        Player(int playerNo, Uno& game) : playerNo(playerNo), game(game) {}

        void addCard(std::unique_ptr<Card> cardPointer) {
            deck.push_back(cardPointer);
        }

        void drawCards(int cardCount) {
            for (int i = 0; i < cardCount; i++) {
                auto& drawPile = game.getDrawPile();
                deck.push_back(std::move(drawPile.back()));
                drawPile.pop_back();
                
                if (drawPile.empty()) {
                    game.newDrawPile();
                }
            }
        }

        void drawCardNoMatch();

        int pickCard();

        void useCard(std::unique_ptr<Card> cardPointer);

        void useAbility(ActionCard actionCard);

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

        std::vector<Player> getPlayers() { return players; }

        std::vector<std::unique_ptr<Card>>& getDrawPile() { return drawPile; }

        std::vector<std::unique_ptr<Card>>& getDiscardPile() { return discardPile; }

        void createPlayers();

        std::vector<std::unique_ptr<Card>> createCards();

        void setupCards();

        void specialFirstDiscard(ActionCard actionCard);

        void setupGame();

        void newDrawPile() {
            std::swap(discardPile, drawPile);

            discardPile.push_back(std::move(drawPile.back()));
            drawPile.pop_back();

            std::random_device rand;
            std::mt19937 g(rand());
            
            std::shuffle(drawPile.begin(), drawPile.end(), g);
        }
};