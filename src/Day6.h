#ifndef AOC2020_DAY6_H
#define AOC2020_DAY6_H

/// Day 6: Custom Customs
/// 
/// Search through lists of questionnaire answers.
/// Each questionnaire is 26 questions, so a yes to question 'b' would
/// be represented by the presence of 'b' in the output line.
/// People are one to a line, and parties are separated by a blank line.

#include <stdlib.h>
#include <stdio.h>

/// Return the number of letters that are present for any line in a group
int tally_anyone_yeses(FILE* fp);

/// Return the number of letters that are present in every line in a group
int tally_everyone_yeses(FILE* fp);

/// Run both 
int day6(void);
#endif
