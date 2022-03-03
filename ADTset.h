/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#ifndef SET_ADT_H
#define SET_ADT_H

#include "ADTsetDef.h"

void set_init(set s);

bool set_find_elem(set s, int num);

void set_remove_elem(set s, int num);

void set_add_elem(set s, int num);

void set_union(set dest, set first, set second);

void set_intersection(set dest, set first, set second);

bool is_superset(set first, set second);

#endif