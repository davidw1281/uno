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

        std::string displayCard() {
            std::vector<std::string> colours = { "Red", "Blue", "Yellow", "Green", "Unknown" };

            switch (symbol) {
                case Wild:
                    return "Wild " + colours[colour];

                case WildDraw4:
                    return "Wild +4 " + colours[colour];

                case Skip:
                    return colours[colour] + " Skip";

                case Reverse:
                    return colours[colour] + " Reverse";

                case Draw2:
                    return colours[colour] + " +2";

                default:
                    return colours[colour] + " " + std::to_string(symbol);
            }
        }
};