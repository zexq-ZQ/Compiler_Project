/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#include "ADTset.h"

/**
 ****** Initializes the set ********
 * - initialize each element to be 0
 */
void set_init(set s)
{
    for (int i = 0; i < SET_SIZE; i++)
    {
        s[i] = 0;
    }
}

/**
 ************* Find element **************
 * - checks the presence of element in set
 */
bool set_find_elem(set s, int num)
{
    return ((s[num / ULL_NUM_BITS] & (((1ULL << (ULL_NUM_BITS - 1)) >> (num % ULL_NUM_BITS)))) != 0);
}

/**
 ******** Remove Element ********
 * Remove an element from the set
 */
void set_remove_elem(set s, int num)
{
    s[num / ULL_NUM_BITS] &= (~((1ULL << (ULL_NUM_BITS - 1)) >> (num % ULL_NUM_BITS)));
}

/**
 ****** Add Element ****** 
 * Add element to the set
 */
void set_add_elem(set s, int num)
{
    s[num / ULL_NUM_BITS] |= ((1ULL << (ULL_NUM_BITS - 1)) >> (num % ULL_NUM_BITS));
}

/**
 ********** Set union ********** 
 * - Finds the union of two sets
 * - dest is union value
 */
void set_union(set dest, set first, set second) {
    for (int j = 0; j < SET_SIZE; j++)
    {
        dest[j] = first[j] | second[j];
    }
}

/**
 ********** Set intersection ********** 
 * - Finds the intersection of two sets
 * - dest is intersection value
 */
void set_intersection(set dest, set first, set second) {
    for (int j = 0; j < SET_SIZE; j++) {
        dest[j] = first[j] & second[j];
    }
}

/**
 ***************** Superset *****************
 * Checks if a set is superset of another set
 *
 */
bool is_superset(set first, set second)
{
    for (int j = 0; j < SET_SIZE; j++) {
        if ((first[j] & second[j]) != second[j]) {
            return false;
        }
    }
    return true;
}