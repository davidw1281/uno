#include "card.h"
#include <unordered_set>
#include <vector>

class Player {
    private:
        int playerNo;
        std::unordered_set<Card> deck;
        Uno game;
};


class Uno {
    private:
        int playerCount;
        std::vector<Player> players;
        std::vector<Card> drawPile;
        std::vector<Card> discardPile;
        int currentTurn;
        int turnChange;
        bool gameOver;
        int winner;
};