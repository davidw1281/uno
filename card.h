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
    protected:
        Colour colour;
        const Symbol symbol;
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

        bool operator==(const Card& otherCard) const {
            return colour == otherCard.colour
                && symbol == otherCard.symbol
                && cardType == otherCard.cardType;
        }
};