#include "cards.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void assert_card_valid(card_t c) {
    /*This function should use assert() to check
    that the card passed in has valid values.
    In particular, its value should be between
    2 and VALUE_ACE (inclusive of both),
    and its suit should be between SPADES
    and CLUBS (inclusive of both).*/
    assert(c.value <= VALUE_ACE && c.value >= 2);
    assert(c.suit >= SPADES && c.suit <= CLUBS);
}

const char* ranking_to_string(hand_ranking_t r) {
    /*This function should convert the
    hand_ranking_t enumerated value passed
    in to a string that describes it.  Remember
    that Drew showed you a nice way to do this
    with emacs keyboard macros!*/
    switch (r) {
        case STRAIGHT_FLUSH:
            return "STRAIGHT_FLUSH";
        case FOUR_OF_A_KIND:
            return "FOUR_OF_A_KIND";
        case FULL_HOUSE:
            return "FULL_HOUSE";
        case FLUSH:
            return "FLUSH";
        case STRAIGHT:
            return "STRAIGHT";
        case THREE_OF_A_KIND:
            return "THREE_OF_A_KIND";
        case TWO_PAIR:
            return "TWO_PAIR";
        case PAIR:
            return "PAIR";
        default:
            return "NOTHING";
    }
}

char value_letter(card_t c) {
    /*This function should return the character that textually represents
    the value of the passed-in card.  For values 2-9, this should
    be that digit.  For 10, it should be '0', and for Jack, Queen, King, and Ace,
    it should be 'J', 'Q', 'K', and 'A' respectively.
    Hint: remember everything is a number.
    For example, the character '0' has the decimal value 48,
    and the character '5' has the decimal value 53, so you could represent
    '5' as '0' + 5.*/
    char x = '?';
    if (c.value >= 2 && c.value <= 9) {
        x = '0' + c.value;
    } else {
        switch (c.value) {
            case 10:
                x = '0';
                break;
            case VALUE_JACK:
                x = 'J';
                break;
            case VALUE_QUEEN:
                x = 'Q';
                break;
            case VALUE_KING:
                x = 'K';
                break;
            case VALUE_ACE:
                x = 'A';
            default:
                break;
        }
    }
    return x;
}

char suit_letter(card_t c) {
    /*This function should return the letter that textually represents
    the suit of the card passed in ('s', 'h', 'd', or 'c' for SPADES,
    HEARTS, DIAMONDS, or CLUBS).*/
    char x = '?';
    switch (c.suit) {
        case SPADES:
            x = 's';
            break;
        case HEARTS:
            x = 'h';
            break;
        case DIAMONDS:
            x = 'd';
            break;
        case CLUBS:
            x = 'c';
            break;
        default:
            break;
    }
    return x;
}

void print_card(card_t c) {
    /*This function should print out the textual
    representation of the card (hint: use the functions
    you previously wrote). It should print
    the value first, then the suit.  For example,
    As  (for Ace of spades)
    0d  (for 10 of diamonds)
    Kc  (for King of clubs) etc.
    This function should not print any additional
    spaces or newlines after the card's text.*/
    char value_card = value_letter(c);
    char suit_card = suit_letter(c);
    printf("%c%c", value_card, suit_card);
}

card_t card_from_letters(char value_let, char suit_let) {
    /*This function should make and return a
    card_t whose value and suit correspond
    to the letters passed in. If the values passed
    in are invalid, you should use assert()
    or print an error message and exit(EXIT_FAILURE).*/
    card_t temp;
    switch (value_let) {
        case '0':
            temp.value = 10;
            break;
        case 'J':
            temp.value = VALUE_JACK;
            break;
        case 'Q':
            temp.value = VALUE_QUEEN;
            break;
        case 'K':
            temp.value = VALUE_KING;
            break;
        case 'A':
            temp.value = VALUE_ACE;
            break;
        default:
            temp.value = value_let - '0';
            break;
    }
    switch (suit_let) {
        case 's':
            temp.suit = SPADES;
            break;
        case 'h':
            temp.suit = HEARTS;
            break;
        case 'd':
            temp.suit = DIAMONDS;
            break;
        case 'c':
            temp.suit = CLUBS;
            break;
        default:
            temp.suit = NUM_SUITS;
            break;
    }
    assert_card_valid(temp);
    return temp;
}

card_t card_from_num(unsigned c) {
    /*This function should take a number from 0 (inclusive)
    to 52 (exclusive) and map it uniquely to
    a card value/suit combination.  Exactly
    how you map the numbers to values/suits
    is up to you, but you must guarantee
    that each valid value/suit combination
    corresponds to exactly one input value
    in the range [0,52). Hint: you may want to use the mod
    operator % to find the remainder of a number divided by 13.
    (In Course 3, this function will be used
    to make a deck of cards by iterating
    over that range and calling it once
    for each value--you just need
    to learn about arrays first so you
    have a place to put all those
    cards.)*/
    card_t temp;
    unsigned div_num = c / 13;
    unsigned reminder = c % 13;
    temp.value = reminder + 2;

    switch (div_num) {
        case 0:
            temp.suit = SPADES;
            break;
        case 1:
            temp.suit = HEARTS;
            break;
        case 2:
            temp.suit = DIAMONDS;
            break;
        case 3:
            temp.suit = CLUBS;
            break;
        default:
            temp.suit = NUM_SUITS;
            break;
    }
    return temp;
}
