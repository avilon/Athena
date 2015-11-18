#ifndef COMMON
#define COMMON

const int EMPTY = 0;
const int WHITE = 1;
const int BLACK = 2;
const int MAN   = 4;
const int KING  = 8;
const int EDGE  = 16;

const int CHANGE_COLOR = 3;
const int WHITE_MAN = ( WHITE | MAN );
const int BLACK_MAN = ( BLACK | MAN );
const int WHITE_KING = ( WHITE | KING );
const int BLACK_KING = ( BLACK | KING );

#endif // COMMON

