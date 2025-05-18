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

        void removeCard(Card card) {
            auto it = std::find(deck.begin(), deck.end(), card);
            if (it != deck.end()) {
                deck.erase(it);
            }
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

        void drawCardNoMatch() {
            std::vector<Card>& drawPile = game.getDrawPile();
            Card cardOnDrawPile = drawPile.back();
            drawPile.pop_back();

            if (cardOnDrawPile.doesMatch(game.getDiscardPile().back())) {
                useCard(cardOnDrawPile);
            } else {
                addCard(cardOnDrawPile);
            }

            if (drawPile.empty()) {
                game.newDrawPile();
            }
        }

        int pickCardIndex() {
            Card cardOnDiscardPile = game.getDiscardPile().back();

            for (int i = 0; i < deck.size(); i++) {
                Card currentCard = deck[i];
                if (currentCard.doesMatch(cardOnDiscardPile)) {
                    return i;
                } 
            }

            return -1;
        }

        void useCard(Card card) {
            removeCard(card);

            std::vector<Card>& discardPile = game.getDiscardPile();
            discardPile.push_back(card);

            if (deck.empty()) {
                gameWon();
                return;
            }

            if (game.getDrawPile().empty()) {
                game.newDrawPile();
            }

            if (deck.size() == 1) {
                maybeSayUno();
            }

            if (card.getCardType() == Action) {
                useAbility(discardPile.back());
            }
        }

        void useAbility(Card& actionCard) {
            std::vector<Colour> colours = { Red, Blue, Yellow, Green };
            std::random_device rand;
            std::mt19937 g(rand());
            std::uniform_int_distribution<> dist(0, 3);
            int colourIndex = dist(g);

            switch (actionCard.getSymbol()) {
                case Skip:
                    game.nextTurn();
                    break;
                case Reverse:
                    game.reverseTurnDirection();
                    break;
                case Draw2:
                    game.nextPlayerDrawCards(2);
                    break;
                case Wild:
                    Colour newColour = colours[colourIndex];
                    actionCard.setColour(newColour);
                    break;
                case WildDraw4:
                    Colour newColour = colours[colourIndex];
                    actionCard.setColour(newColour);

                    game.nextPlayerDrawCards(4);
                    break;
            }
        }

        void maybeSayUno() {
            std::random_device rand;
            std::mt19937 g(rand());
            std::uniform_int_distribution<> dist(1, 5);

            int didNotSayUno = dist(g);
            if (didNotSayUno == 1) {
                drawCards(2);
            } 
        }

        void runTurn() {
            int cardIndex = pickCardIndex();
            if (cardIndex != -1) {
                useCard(deck[cardIndex]);
            } else {
                drawCardNoMatch();
            }
        }

        void gameWon() {
            game.gameWon(playerNo);
        }
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
        Uno(int playerCount) : playerCount(playerCount) {}

        std::vector<Player> getPlayers() { return players; }

        std::vector<Card>& getDrawPile() { return drawPile; }

        std::vector<Card>& getDiscardPile() { return discardPile; }

        void reverseTurnDirection() {
            turnChange *= -1;
        }

        void nextTurn() {
            currentTurnIndex = (currentTurnIndex + turnChange + playerCount) % playerCount;
        }

        void nextPlayerDrawCards(int cardCount) {
            int nextTurnIndex = (currentTurnIndex + turnChange + playerCount) % playerCount;
            Player nextPlayer = players[nextTurnIndex];
            nextPlayer.drawCards(cardCount);
        }

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

        void runGame();

        void gameWon(int playerNo) {
            gameOver = true;
            winner = playerNo;
        }
};