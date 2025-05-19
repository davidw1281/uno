#include "card.h"
#include <vector>
#include <algorithm>
#include <random>
#include <print>

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
            int removeCardIndex = -1;
            Colour colour = card.getColour();
            Symbol symbol = card.getSymbol();
            CardType cardType = card.getCardType();

            for (int i = 0; i < deck.size(); i++) {
                if (colour == deck[i].getColour() && symbol == deck[i].getSymbol() && cardType == deck[i].getCardType()) {
                    removeCardIndex = i;
                    break;
                }
            }

            if (removeCardIndex != -1) {
                deck.erase(deck.begin() + removeCardIndex);
            }
        }

        void setColour(Card& wildCard) {
            std::vector<Colour> colours = { Red, Blue, Yellow, Green };
            std::random_device rand;
            std::mt19937 g(rand());
            std::uniform_int_distribution<> dist(0, 3);
            int colourIndex = dist(g);

            Colour newColour = colours[colourIndex];
            wildCard.setColour(newColour);
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
            switch (actionCard.getSymbol()) {
                case Skip:
                    game.nextTurn();
                    break;
                case Reverse:
                    game.reverseTurnDirection();
                    break;
                case Draw2:
                    game.nextPlayerDrawCards(2);
                    game.nextTurn();
                    break;
                case Wild:
                    setColour(actionCard);
                    break;
                case WildDraw4:
                    setColour(actionCard);

                    game.nextPlayerDrawCards(4);
                    game.nextTurn();
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

        void gameWon() {
            game.gameWon(playerNo);
        }

        void runTurn() {
            int cardIndex = pickCardIndex();
            if (cardIndex != -1) {
                useCard(deck[cardIndex]);
            } else {
                drawCardNoMatch();
            }
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

        std::vector<Player> getPlayers() { 
            return players; 
        }

        std::vector<Card>& getDrawPile() { 
            return drawPile; 
        }

        std::vector<Card>& getDiscardPile() { 
            return discardPile; 
        }

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

        void shuffleCards(std::vector<Card>& cards) {
            std::random_device rand;
            std::mt19937 g(rand());
            std::shuffle(cards.begin(), cards.end(), g);
        }

        void createPlayers() {
            for (int i = 1; i <= playerCount; i++) {
                players.push_back(Player(i, *this));
            }
        }

        std::vector<Card> createCards() {
            std::vector<Card> cards;
            std::vector<Colour> colours = { Red, Blue, Yellow, Green };
            std::vector<Symbol> symbols = { One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Skip, Reverse, Draw2 };

            for (Colour colour : colours) {
                cards.push_back(Card(colour, Zero));
            }

            for (Symbol symbol : symbols) {
                for (Colour colour : colours) {
                    for (int count = 0; count < 2; count++) {
                        cards.push_back(Card(colour, symbol));
                    }
                }
            }

            for (int count = 0; count < 4; count++) {
                cards.push_back(Card(Wild));
                cards.push_back(Card(WildDraw4));
            }

            shuffleCards(cards);
            return cards;
        }

        void setupCards(std::vector<Card> cards) {
            for (int count = 0; count < 7; count++) {
                for (Player& player : players) {
                    player.addCard(cards.back());
                    cards.pop_back();
                }
            }

            drawPile = cards;
            discardPile.push_back(drawPile.back());
            drawPile.pop_back();

            if (discardPile.back().getCardType() == Action) {
                specialFirstDiscard(discardPile.back());
            }
        }

        void specialFirstDiscard(Card& actionCard) {
            switch (actionCard.getSymbol()) {
                case Skip:
                    nextTurn();
                    break;
                case Reverse:
                    reverseTurnDirection();
                    nextTurn();
                    break;
                case Draw2:
                    players[0].drawCards(2);
                    nextTurn();
                    break;
                case Wild:
                    players[0].setColour(actionCard);
                    break;
                case WildDraw4:
                    drawPile.push_back(discardPile.back());
                    discardPile.pop_back();

                    shuffleCards(drawPile);

                    discardPile.push_back(drawPile.back());
                    drawPile.pop_back();

                    if (discardPile.back().getCardType() == Action) {
                        specialFirstDiscard(discardPile.back());
                    }
                    break;
            }
        }

        void setupGame() {
            createPlayers();
            setupCards(createCards());
        }

        void newDrawPile() {
            std::swap(discardPile, drawPile);

            discardPile.push_back(drawPile.back());
            drawPile.pop_back();

            shuffleCards(drawPile);
        }

        void gameWon(int playerNo) {
            gameOver = true;
            winner = playerNo;
        }

        void runGame() {
            setupGame();

            while (!gameOver) {
                Player currentPlayer = players[currentTurnIndex];
                currentPlayer.runTurn();
                nextTurn();
            }

            std::println("Player {} wins", winner);
        }
};