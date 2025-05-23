#include "game.h"
#include <random>
#include <algorithm>
#include <iostream>

Player::Player(int playerNo, Uno& game) : playerNo(playerNo), game(game) {}

void Player::addCard(Card card) {
    deck.push_back(card);
}

void Player::removeCard(Card card) {
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

std::string Player::displayDeck() {
    std::string deckString = "Player " + std::to_string(playerNo) + "'s deck: [";

    if (!deck.empty()) {
        for (int i = 0; i < deck.size() - 1; i++) {
            deckString += deck[i].displayCard() + ", ";
        }

        deckString += deck[deck.size() - 1].displayCard();
    }

    deckString += "]\n";
    return deckString;
}

void Player::setColour(Card& wildCard) {
    std::vector<Colour> colours = { Red, Blue, Yellow, Green };
    std::random_device rand;
    std::mt19937 g(rand());
    std::uniform_int_distribution<> dist(0, 3);
    int colourIndex = dist(g);

    Colour newColour = colours[colourIndex];
    wildCard.setColour(newColour);
}

void Player::drawCards(int cardCount) {
    for (int i = 0; i < cardCount; i++) {
        std::vector<Card>& drawPile = game.getDrawPile();
        deck.push_back(drawPile.back());
        drawPile.pop_back();
        
        if (drawPile.empty()) {
            game.newDrawPile();
        }
    }
}

void Player::drawCardNoMatch() {
    std::vector<Card>& drawPile = game.getDrawPile();
    Card cardOnDrawPile = drawPile.back();
    drawPile.pop_back();

    std::cout << "No matching cards! Card drawn: " << cardOnDrawPile.displayCard() << "\n";

    if (cardOnDrawPile.doesMatch(game.getDiscardPile().back())) {
        useCard(cardOnDrawPile);
    } else {
        addCard(cardOnDrawPile);
        std::cout << displayDeck();
    }

    if (drawPile.empty()) {
        game.newDrawPile();
    }
}

int Player::pickCardIndex() {
    Card& cardOnDiscardPile = game.getDiscardPile().back();

    for (int i = 0; i < deck.size(); i++) {
        if (deck[i].doesMatch(cardOnDiscardPile)) {
            return i;
        } 
    }

    return -1;
}

void Player::useCard(Card card) {
    std::cout << "Card used: " << card.displayCard() << "\n";

    removeCard(card);
    std::cout << displayDeck();

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

void Player::useAbility(Card& actionCard) {
    switch (actionCard.getSymbol()) {
        case Skip:
            game.nextTurn();
            break;
        case Reverse:
            if (game.getPlayerCount() == 2) {
                game.nextTurn();
            } else {
                game.reverseTurnDirection();
            }
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

void Player::maybeSayUno() {
    std::random_device rand;
    std::mt19937 g(rand());
    std::uniform_int_distribution<> dist(1, 5);

    int didNotSayUno = dist(g);
    if (didNotSayUno == 1) {
        std::cout << "Player " << playerNo << " didn't say Uno! Draw 2 cards...\n";
        drawCards(2);
        std::cout << displayDeck();
    } 
}

void Player::gameWon() {
    game.gameWon(playerNo);
}

void Player::runTurn() {
    std::cout << "Player " << playerNo << "'s turn\n";
    std::cout << "Card on top: " << game.getDiscardPile().back().displayCard() << "\n";
    std::cout << displayDeck();

    int cardIndex = pickCardIndex();
    if (cardIndex != -1) {
        useCard(deck[cardIndex]);
    } else {
        drawCardNoMatch();
    }

    std::cout << "\n";
}

Uno::Uno(int playerCount) : playerCount(playerCount) {}

int Uno::getPlayerCount() {
    return playerCount;
}

std::vector<Player> Uno::getPlayers() { 
    return players; 
}

std::vector<Card>& Uno::getDrawPile() { 
    return drawPile; 
}

std::vector<Card>& Uno::getDiscardPile() { 
    return discardPile; 
}

void Uno::reverseTurnDirection() {
    turnChange *= -1;
}

void Uno::nextTurn() {
    currentTurnIndex = (currentTurnIndex + turnChange + playerCount) % playerCount;
}

void Uno::nextPlayerDrawCards(int cardCount) {
    int nextTurnIndex = (currentTurnIndex + turnChange + playerCount) % playerCount;
    players[nextTurnIndex].drawCards(cardCount);
}

void Uno::shuffleCards(std::vector<Card>& cards) {
    std::random_device rand;
    std::mt19937 g(rand());
    std::shuffle(cards.begin(), cards.end(), g);
}

void Uno::createPlayers() {
    for (int i = 1; i <= playerCount; i++) {
        players.push_back(Player(i, *this));
    }
}

std::vector<Card> Uno::createCards() {
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

void Uno::setupCards(std::vector<Card> cards) {
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

void Uno::specialFirstDiscard(Card& actionCard) {
    std::cout << "Special first card: " << actionCard.displayCard() << "\n\n";

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

void Uno::setupGame() {
    createPlayers();
    setupCards(createCards());
}

void Uno::newDrawPile() {
    std::swap(discardPile, drawPile);

    discardPile.push_back(drawPile.back());
    drawPile.pop_back();

    shuffleCards(drawPile);
}

void Uno::gameWon(int playerNo) {
    gameOver = true;
    winner = playerNo;
}

void Uno::runGame() {
    setupGame();

    while (!gameOver) {
        players[currentTurnIndex].runTurn();
        nextTurn();
    }

    std::cout << "Player " << winner << " wins!\n";
}