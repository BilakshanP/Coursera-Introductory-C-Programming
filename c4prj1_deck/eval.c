#include "eval.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int card_ptr_comp(const void *vp1, const void *vp2) {
    /*You want to sort the hand by value, so you need
    a comparison function to pass to quicksort.
    Quicksort sorts into ascending order, but you
    want descending order, so you will want to
    return
       something < 0  if card1 > card2
       0              if card1 == card2
       something > 0  if card1 < card2
    If two cards have the same value, compare them by
    suit in the same order as the enum suit_t:
       club < diamond < heart < spade
    Note that vp1 and vp2 are passed as const void *
    because that is the type that qsort demands.
    They will should each be assigned to variables
    of type
     const card_t * const * cp1
    before using them (this is much like sorting
    an array of strings from your readings).
    To learn more about using the C library function qsort,
    we suggest reviewing the course reading
    "Sorting Functions" in the "Function Pointers"
    lesson and consulting "man qsort"
    to read about the comparison function.*/
    const card_t *const *card1 = vp1;
    const card_t *const *card2 = vp2;
    if ((**card1).value > (**card2).value) {
        return -1;
    } else if ((**card1).value < (**card2).value) {
        return 1;
    } else {
        if ((**card1).suit > (**card2).suit) {
            return -1;
        } else if ((**card1).suit < (**card2).suit) {
            return 1;
        }
    }

    return 0;
}

suit_t flush_suit(deck_t *hand) {
    /* This function looks at the hand and determines
     if a flush (at least 5 cards of one suit) exists.
     If so, it returns the suit of the cards comprising
     the flush.  If not, it returns NUM_SUITS.
     For example:
      Given Ks Qs 0s 9h 8s 7s, it would return SPADES.
      Given Kd Qd 0s 9h 8c 7c, it would return NUM_SUITS.*/
    size_t hand_size = hand->n_cards;
    card_t **card_total = hand->cards;
    int s = 0;
    int h = 0;
    int c = 0;
    int d = 0;

    for (int i = 0; i < hand_size; i++) {
        card_t current_card = **(card_total + i);
        switch (current_card.suit) {
            case SPADES:
                s++;
                break;
            case HEARTS:
                h++;
                break;
            case DIAMONDS:
                d++;
                break;
            case CLUBS:
                c++;
                break;
            default:
                break;
        }
    }
    if (s >= 5) {
        return SPADES;
    }
    if (h >= 5) {
        return HEARTS;
    }
    if (d >= 5) {
        return DIAMONDS;
    }
    if (c >= 5) {
        return CLUBS;
    }
    return NUM_SUITS;
}

unsigned get_largest_element(unsigned *arr, size_t n) {
    /*This function returns the largest element in an array
    of unsigned integers.  This should be familiar
    from the videos you watched.*/
    unsigned largest = arr[0];
    for (int i = 0; i < n; i++) {
        if (arr[i] > largest) {
            largest = arr[i];
        }
    }
    return largest;
}

size_t get_match_index(unsigned *match_counts, size_t n, unsigned n_of_akind) {
    /*This function returns the index in the array (match_counts) whose
    value is n_of_a kind.  The array has n elements.  The array match_counts
    may have multiple values equal to n_of_akind.  You should return
    the LOWEST index whose value is n_of_akind [which also guarantees
    it corresponds to the largest valued cards, since they will be sorted].
    (Once you figure out the best n_of_akind above,
     you will use this to locate that group of cards
     in the hand).
     Note that it is guaranteed that n_of_akind is in match_counts.
     If not, you should abort as this is evidence of an error.*/

    size_t index = -1;
    for (size_t i = 0; i < n; i++) {
        if (match_counts[i] == n_of_akind) {
            index = i;
            break;
        }
    }
    assert(index != -1);
    return index;
}

ssize_t find_secondary_pair(deck_t *hand,
                            unsigned *match_counts,
                            size_t match_idx) {
    /* When you have a hand with 3 of a kind or a pair,
     you will want to look and see if there is another
     pair to make the hand into a full house or
     or two pairs.  This function takes in
     the hand, the match counts from before, and
     the index where the original match (3 of a kind
     or pair) was found. It should find
     the index of a card meeting the following conditions:
       - Its match count is > 1 [so there is at least a pair of them]
       - The card's value is not the same as the value of the
         card at match_idx (so it is not part of the original
         three of a kind/pair)
       - It is the lowest index meeting the first two conditions
         (which will be the start of that pair, and the highest
          value pair other than the original match).
     If no such index can be found, this function should
     return -1.*/

    ssize_t index = -1;
    card_t **card_in_hand = hand->cards;
    size_t size = hand->n_cards;
    unsigned current_value = (**(card_in_hand + match_idx)).value;
    unsigned match_temp;
    unsigned value_temp;
    for (size_t i = 0; i < size; i++) {
        match_temp = match_counts[i];
        value_temp = (**(card_in_hand + i)).value;
        if ((match_temp) > 1 && (value_temp != current_value)) {
            index = i;
            return index;
        }
    }
    return -1;
}
int is_ace_low_straight_at(deck_t *hand, size_t index, suit_t fs) {
    card_t **hand_card = hand->cards;
    size_t size = hand->n_cards;
    if ((**(hand_card + index)).value != VALUE_ACE) {
        return 0;
    }
    if (fs == NUM_SUITS) {
        int count = 1;
        size_t idx_temp = 0;
        for (size_t i = index + 1; i < size; i++) {
            if ((**(hand_card + i)).value == 5) {
                idx_temp = i;
                break;
            }
        }
        if ((idx_temp == 0) || idx_temp > 3) {
            return 0;
        }
        count++;

        unsigned current_card_value = (**(hand_card + idx_temp)).value;
        for (size_t m = idx_temp; m < size; m++) {
            if ((**(hand_card + m)).value == current_card_value) {
                continue;
            }
            if ((**(hand_card + m)).value == current_card_value - 1) {
                current_card_value = (**(hand_card + m)).value;
                count++;
            }
        }
        if (count >= 5) {
            return 1;
        } else {
            return 0;
        }
    }
    if ((**(hand_card + index)).suit != fs) {
        return 0;
    }
    int count2 = 1;
    size_t idx2 = 0;

    for (size_t k = index + 1; k < size; k++) {
        if ((**(hand_card + k)).value == 5 && (**(hand_card + k)).suit == fs) {
            idx2 = k;
            break;
        }
    }
    if ((idx2 == 0) || idx2 > 3) {
        return 0;
    }
    count2++;
    unsigned card_curr_value = (**(hand_card + idx2)).value;
    for (size_t j = idx2 + 1; j < size; j++) {
        if ((**(hand_card + j)).suit != fs) {
            continue;
        }
        if ((**(hand_card + j)).value == card_curr_value - 1) {
            card_curr_value = (**(hand_card + j)).value;
            count2++;
        }
    }
    if (count2 == 5) {
        return 1;
    } else {
        return 0;
    }
}

int is_straight_at(deck_t *hand, size_t index, suit_t fs) {
    /*This function should determine if there is a straight
     starting at index (and only starting at index) in the
     given hand.  If fs is NUM_SUITS, then it should look
     for any straight.  If fs is some other value, then
     it should look for a straight flush in the specified suit.
      This function should return:
      -1 if an Ace-low straight was found at that index (and that index is the Ace)
       0  if no straight was found at that index
       1  if any other straight was found at that index*/
    if (is_ace_low_straight_at(hand, index, fs) == 1) {
        return -1;
    }
    int count = 1;
    card_t **hand_card = hand->cards;
    size_t size = hand->n_cards;
    if (fs == NUM_SUITS) {
        unsigned card_curr_value1 = (**(hand_card + index)).value;
        for (size_t i = index; i < size; i++) {
            if ((**(hand_card + i)).value == card_curr_value1) {
                continue;
            }
            if ((**(hand_card + i)).value == card_curr_value1 - 1) {
                card_curr_value1 = (**(hand_card + i)).value;
                count++;
            }
        }
        if (count >= 5) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if ((**(hand_card + index)).suit != fs) {
            return 0;
        }
        unsigned card_curr_value = (**(hand_card + index)).value;
        for (size_t j = index + 1; j < size; j++) {
            if ((**(hand_card + j)).suit != fs) {
                continue;
            }
            if ((**(hand_card + j)).value == card_curr_value - 1) {
                card_curr_value = (**(hand_card + j)).value;
                count++;
            }
        }
        if (count >= 5) {
            return 1;
        } else {
            return 0;
        }
    }
}

hand_eval_t build_hand_from_match(deck_t *hand,
                                  unsigned n,
                                  hand_ranking_t what,
                                  size_t idx) {
    hand_eval_t ans;
    ans.ranking = what;
    card_t **card_in_hand = hand->cards;
    size_t hand_size = hand->n_cards;
    unsigned count = n;

    for (unsigned i = 0; i < n; i++) {
        ans.cards[i] = *(card_in_hand + idx + i);
    }
    for (size_t j = 0; j < idx; j++) {
        ans.cards[j + n] = *(card_in_hand + j);
        count++;
        if (count == 5) {
            break;
        }
    }
    if (count < 5) {
        for (size_t k = idx + n; k < hand_size; k++) {
            ans.cards[count] = *(card_in_hand + k);
            count++;
            if (count == 5) {
                break;
            }
        }
    }

    return ans;
}

int compare_hands(deck_t *hand1, deck_t *hand2) {
    /*(a) sort each hand using qsort on the hand's cards
        and your card_ptr_comp [from earlier]
    (b) Call evaluate_hand on each hand, which gives you a hand_eval_t
        for each hand.
    (c) Check if the rankings in the hand_eval_t are the same
        or different.  If they are different, you can just use
        the ranking to determine the winner.
    (d) If they are the same, then you need to look
        at the values in the cards array of each hand_eval_t
        to break the tie. The way that we constructed
        the hand_eval_t's cards array means that
        the cards are already ordered from most significant (at index 0)
        to least significant (at index 4).  You can just
        do a lexicographic comparison on the values in the arrays.
        (Its like comparing strings, but you are comparing values
         of cards---if element 0 is the different, use that difference
         to determine your answer.  If element 0 is the same,
         look at element 1, and so on).*/
    qsort(hand1->cards, hand1->n_cards, sizeof(card_t), card_ptr_comp);
    qsort(hand2->cards, hand2->n_cards, sizeof(card_t), card_ptr_comp);
    hand_eval_t hand11 = evaluate_hand(hand1);
    hand_eval_t hand22 = evaluate_hand(hand2);
    if (hand11.ranking < hand22.ranking)
        return 1;
    else if (hand11.ranking > hand22.ranking)
        return -1;
    else {
        for (size_t i = 0; i < 5; i++) {
            card_t *card1 = hand11.cards[i];
            card_t *card2 = hand22.cards[i];
            if (card1->value > card2->value)
                return 1;
            else if (card1->value < card2->value)
                return -1;
            else
                continue;
        }
    }

    return 0;
}

unsigned *get_match_counts(deck_t *hand) {
    /*Given a hand (deck_t) of cards, this function
     allocates an array of unsigned ints with as
     many elements as there are cards in the hand.
     It then fills in this array with
     the "match counts" of the corresponding cards.
     That is, for each card in the original hand,
     the value in the match count array
     is how many times a card of the same
     value appears in the hand.  For example,
     given
       Ks Kh Qs Qh 0s 9d 9c 9h
     This function would return
       2  2  2  2  1  3  3  3
     because there are 2 kings, 2 queens,
     1 ten, and 3 nines.*/
    size_t len = hand->n_cards;
    card_t **card_in_hand = hand->cards;

    unsigned *match = malloc(len * sizeof(*match));
    for (size_t i = 0; i < len; i++) {
        card_t *card = card_in_hand[i];
        unsigned count = 0;
        for (size_t j = 0; j < len; j++) {
            if (card_in_hand[j]->value == card->value) {
                count++;
            }
        }
        match[i] = count;
    }
    return match;
}

void copy_straight(card_t **to, deck_t *from, size_t ind, suit_t fs, size_t count) {
    assert(fs == NUM_SUITS || from->cards[ind]->suit == fs);
    unsigned nextv = from->cards[ind]->value;
    size_t to_ind = 0;
    while (count > 0) {
        assert(ind < from->n_cards);
        assert(nextv >= 2);
        assert(to_ind < 5);
        if (from->cards[ind]->value == nextv &&
            (fs == NUM_SUITS || from->cards[ind]->suit == fs)) {
            to[to_ind] = from->cards[ind];
            to_ind++;
            count--;
            nextv--;
        }
        ind++;
    }
}

int find_straight(deck_t *hand, suit_t fs, hand_eval_t *ans) {
    if (hand->n_cards < 5) {
        return 0;
    }
    for (size_t i = 0; i <= hand->n_cards - 5; i++) {
        int x = is_straight_at(hand, i, fs);
        if (x != 0) {
            if (x < 0) {
                assert(hand->cards[i]->value == VALUE_ACE &&
                       (fs == NUM_SUITS || hand->cards[i]->suit == fs));
                ans->cards[4] = hand->cards[i];
                size_t cpind = i + 1;
                while (hand->cards[cpind]->value != 5 ||
                       !(fs == NUM_SUITS || hand->cards[cpind]->suit == fs)) {
                    cpind++;
                    assert(cpind < hand->n_cards);
                }
                copy_straight(ans->cards, hand, cpind, fs, 4);
            } else {
                copy_straight(ans->cards, hand, i, fs, 5);
            }
            return 1;
        }
    }
    return 0;
}

hand_eval_t evaluate_hand(deck_t *hand) {
    suit_t fs = flush_suit(hand);
    hand_eval_t ans;
    if (fs != NUM_SUITS) {
        if (find_straight(hand, fs, &ans)) {
            ans.ranking = STRAIGHT_FLUSH;
            return ans;
        }
    }
    unsigned *match_counts = get_match_counts(hand);
    unsigned n_of_a_kind = get_largest_element(match_counts, hand->n_cards);
    assert(n_of_a_kind <= 4);
    size_t match_idx = get_match_index(match_counts, hand->n_cards, n_of_a_kind);
    ssize_t other_pair_idx = find_secondary_pair(hand, match_counts, match_idx);
    free(match_counts);
    if (n_of_a_kind == 4) {
        return build_hand_from_match(hand, 4, FOUR_OF_A_KIND, match_idx);
    } else if (n_of_a_kind == 3 && other_pair_idx >= 0) {
        ans = build_hand_from_match(hand, 3, FULL_HOUSE, match_idx);
        ans.cards[3] = hand->cards[other_pair_idx];
        ans.cards[4] = hand->cards[other_pair_idx + 1];
        return ans;
    } else if (fs != NUM_SUITS) {
        ans.ranking = FLUSH;
        size_t copy_idx = 0;
        for (size_t i = 0; i < hand->n_cards; i++) {
            if (hand->cards[i]->suit == fs) {
                ans.cards[copy_idx] = hand->cards[i];
                copy_idx++;
                if (copy_idx >= 5) {
                    break;
                }
            }
        }
        return ans;
    } else if (find_straight(hand, NUM_SUITS, &ans)) {
        ans.ranking = STRAIGHT;
        return ans;
    } else if (n_of_a_kind == 3) {
        return build_hand_from_match(hand, 3, THREE_OF_A_KIND, match_idx);
    } else if (other_pair_idx >= 0) {
        assert(n_of_a_kind == 2);
        ans = build_hand_from_match(hand, 2, TWO_PAIR, match_idx);
        ans.cards[2] = hand->cards[other_pair_idx];
        ans.cards[3] = hand->cards[other_pair_idx + 1];
        if (match_idx > 0) {
            ans.cards[4] = hand->cards[0];
        } else if (other_pair_idx > 2) {
            ans.cards[4] = hand->cards[2];
        } else {
            ans.cards[4] = hand->cards[4];
        }
        return ans;
    } else if (n_of_a_kind == 2) {
        return build_hand_from_match(hand, 2, PAIR, match_idx);
    }
    return build_hand_from_match(hand, 0, NOTHING, 0);
}
