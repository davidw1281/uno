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
        CardType type;
        Colour colour;

    public:
        virtual bool doesMatch(Card cardOnPile) = 0;
        Colour getColour() { return this->colour; }
        CardType getType() { return this->type; }
};

class NumberCard : public Card {
    private:
        int number;

    public:
        NumberCard(Colour colour, int number) {
            this->type = Number;
            this->colour = colour;
            this->number = number;
        }

        int getNumber() { return this->number; }

        bool doesMatch(NumberCard cardOnPile) {
            if (this->getColour() == cardOnPile.getColour()) { 
                return true; 
            } else if (this->getNumber() == cardOnPile.getNumber()) {
                return true;
            } else {
                return false;
            }
        }

        bool doesMatch(ActionCard cardOnPile) {
            if (this->getColour() == cardOnPile.getColour()) { 
                return true; 
            } else {
                return false;
            }
        }
};

class ActionCard : public Card {
    private:
        ActionType actionType;

    public:
        ActionCard(Colour colour, ActionType actionType) {
            this->type = Action;
            this->colour = colour;
            this->actionType = actionType;
        }

        int getActionType() { return this->actionType; };

        void setColour(Colour colour) {
            if (this->actionType == Wild || this->actionType == WildDraw4) {
                this->colour = colour;
            }
        }

        bool doesMatch(NumberCard cardOnPile) {
            if (this->getColour() == Unknown) { 
                return true; 
            } else if (this->getColour() == cardOnPile.getColour()) {
                return true;
            } else {
                return false;
            }
        }

        bool doesMatch(ActionCard cardOnPile) {
            if (this->getColour() == Unknown) { 
                return true; 
            } else if (this->getColour() == cardOnPile.getColour()) {
                return true;
            } else if (this->getActionType() == cardOnPile.getActionType()){
                return true;
            } else {
                return false;
            }
        }
};