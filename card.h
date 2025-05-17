enum CardType {
    Number,
    Action
};

enum ActionType {
    Skip,
    Reverse,
    Draw2,
    Wild,
    WildDraw4
};

enum Colour {
    Red,
    Blue,
    Yellow,
    Green,
    Unknown
};

class Card {
    protected:
        const CardType type;
        Colour colour;

    public:
        Card(Colour colour, CardType type) : colour(colour), type(type) {}

        virtual bool doesMatch(Card cardOnPile) = 0;
        Colour getColour() { return colour; }
        CardType getType() { return type; }
};

class NumberCard : public Card {
    private:
        const int number;

    public:
        NumberCard(Colour colour, int number) : Card(colour, Number), number(number) {}

        int getNumber() { return number; }

        bool doesMatch(NumberCard cardOnPile) {
            if (getColour() == cardOnPile.getColour()) { 
                return true; 
            } else if (getNumber() == cardOnPile.getNumber()) {
                return true;
            } else {
                return false;
            }
        }

        bool doesMatch(ActionCard cardOnPile) {
            if (getColour() == cardOnPile.getColour()) { 
                return true; 
            } else {
                return false;
            }
        }
};

class ActionCard : public Card {
    private:
        const ActionType actionType;

    public:
        // Skip, Reverse, Draw2
        ActionCard(Colour colour, ActionType actionType) : Card(colour, Action), actionType(actionType) {}

        // Wild, WildDraw4
        ActionCard(ActionType actionType) : Card(Unknown, Action), actionType(actionType) {}

        int getActionType() { return actionType; };

        void setColour(Colour colour) {
            if (this->colour == Unknown) {
                this->colour = colour;
            }
        }

        bool doesMatch(NumberCard cardOnPile) {
            if (getColour() == Unknown) { 
                return true; 
            } else if (getColour() == cardOnPile.getColour()) {
                return true;
            } else {
                return false;
            }
        }

        bool doesMatch(ActionCard cardOnPile) {
            if (getColour() == Unknown) { 
                return true; 
            } else if (getColour() == cardOnPile.getColour()) {
                return true;
            } else if (getActionType() == cardOnPile.getActionType()){
                return true;
            } else {
                return false;
            }
        }
};