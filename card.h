#include <iostream>
#include <vector>
#include <string>

enum Colour {
    Red,
    Blue,
    Yellow,
    Green,
    Unknown
};

enum Symbol {
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Skip,
    Reverse,
    Draw2,
    Wild,
    WildDraw4
};

enum CardType {
    Number,
    Action
};


class Card {
    private:
        Colour colour;
        Symbol symbol;
        CardType cardType;

    public:
        Card(Colour colour, Symbol symbol) : colour(colour), symbol(symbol) {
            switch (symbol) {
                case Skip:
                case Reverse:
                case Draw2:
                    cardType = Action;
                    break;

                default:
                    cardType = Number;
            }
        }

        // Wild and WildDraw4
        Card(Symbol symbol) : symbol(symbol) {
            colour = Unknown;
            cardType = Action;
        }

        Colour getColour() { 
            return colour; 
        }

        void setColour(Colour colour) {
            if (this->colour == Unknown) {
                this->colour = colour;
            }
        }

        Symbol getSymbol() { 
            return symbol; 
        }

        CardType getCardType() { 
            return cardType; 
        }

        bool doesMatch(Card cardOnPile) {
            if (colour == Unknown) { 
                return true;
            } else if (colour == cardOnPile.getColour()) {
                return true; 
            } else if (symbol == cardOnPile.getSymbol()) {
                return true;
            } else {
                return false;
            }
        }

        void displayCard() {
            std::vector<std::string> colours = { "Red", "Blue", "Yellow", "Green", "Unknown" };

            switch (symbol) {
                case Wild:
                    std::cout << "Wild" << colours[colour];
                    break;

                case WildDraw4:
                    std::cout << "Wild +4" << colours[colour];
                    break;

                case Skip:
                    std::cout << colours[colour] << " Skip";
                    break;

                case Reverse:
                    std::cout << colours[colour] << " Reverse";
                    break;

                case Draw2:
                    std::cout << colours[colour] << " +2";
                    break;

                default:
                    std::cout << colours[colour] << " " << symbol;
            }
        }
};