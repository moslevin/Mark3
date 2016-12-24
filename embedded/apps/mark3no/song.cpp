#include "song.h"

#include "mark3cfg.h"
#include "drvSound.h"

// Jingle bells!
SongNote_t g_astJingleNotes[] = {
// -- Dashing though the
    { TONE(G, 4), NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_QUARTER, 128 },
    { TONE(D, 5), NOTE_QUARTER, 128 },
    { TONE(C, 5), NOTE_QUARTER, 128 },

// -- snow in a
    { TONE(G, 4), NOTE_DOTTED_HALF, 128 },

    { TONE(G, 4), NOTE_SIXTEENTH, 128 },
    { TONE(G, 4), NOTE_SIXTEENTH, 0 },
    { TONE(G, 4), NOTE_SIXTEENTH, 128 },
    { TONE(G, 4), NOTE_SIXTEENTH, 0 },

// -- One horse open
    { TONE(G, 4), NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_QUARTER, 128 },
    { TONE(D, 5), NOTE_QUARTER, 128 },
    { TONE(C, 5), NOTE_QUARTER, 128 },

// -- Sleigh
    { TONE(A, 4), NOTE_DOTTED_HALF, 128 },
    { TONE(A, 4), NOTE_QUARTER, 0 },

// -- over the fields we
    { TONE(A, 4), NOTE_QUARTER, 128 },
    { TONE(F, 5), NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_QUARTER, 128 },
    { TONE(D, 5), NOTE_QUARTER, 128 },

// -- go
    { TONE(B, 4), NOTE_DOTTED_HALF, 128 },
    { TONE(B, 4), NOTE_QUARTER, 0 },

// -- laughing all the
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },
    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },
    { TONE(D, 5), NOTE_EIGHTH, 128 },
    { TONE(D, 5), NOTE_EIGHTH, 0 },

// -- way
    { TONE(E, 5), NOTE_DOTTED_HALF, 128 },
    { TONE(E, 5), NOTE_QUARTER, 0 },

// -- bells on bobtail
    { TONE(G, 4), NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_QUARTER, 128 },
    { TONE(D, 5), NOTE_QUARTER, 128 },
    { TONE(C, 5), NOTE_QUARTER, 128 },

// -- ring
    { TONE(G, 4), NOTE_DOTTED_HALF, 128 },
    { TONE(G, 4), NOTE_QUARTER, 0 },

// -- making spirigs
    { TONE(G, 4), NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_QUARTER, 128 },
    { TONE(D, 5), NOTE_QUARTER, 128 },
    { TONE(C, 5), NOTE_QUARTER, 128 },

// -- bright, what
    { TONE(A, 4), NOTE_HALF + NOTE_EIGHTH, 128 },
    { TONE(A, 4), NOTE_EIGHTH, 0 },

    { TONE(A, 4), NOTE_EIGHTH, 128 },
    { TONE(A, 4), NOTE_EIGHTH, 0 },

// -- fun it is to
    { TONE(A, 4), NOTE_EIGHTH, 128 },
    { TONE(A, 4), NOTE_EIGHTH, 0 },
    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },
    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },
    { TONE(D, 5), NOTE_EIGHTH, 128 },
    { TONE(D, 5), NOTE_EIGHTH, 0 },

// -- ride and sing a
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },

// -- sleighing song to
    { TONE(A, 5), NOTE_QUARTER, 128 },
    { TONE(G, 5), NOTE_QUARTER, 128 },
    { TONE(F, 5), NOTE_QUARTER, 128 },
    { TONE(D, 5), NOTE_QUARTER, 128 },

// -- nite, oh!
    { TONE(C, 5), NOTE_HALF, 128 },
    { TONE(G, 5), NOTE_HALF, 128 },

// Chorus
// 1 -- jingle bells
    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH + NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

// 2 -- jingle bells
    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH + NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

// 3 -- jingle all the
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },

    { TONE(C, 5), NOTE_QUARTER + NOTE_EIGHTH, 128 },
    { TONE(D, 5), NOTE_EIGHTH, 128 },

// 4 -- way
    { TONE(E, 5), NOTE_WHOLE, 128 },

// 5 -- oh what fun it
    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },

    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },

    { TONE(F, 5), NOTE_QUARTER, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },
    { TONE(F, 5), NOTE_SIXTEENTH, 128 },
    { TONE(F, 5), NOTE_SIXTEENTH, 0 },

// 6 -- is to ride

    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_SIXTEENTH, 128 },
    { TONE(E, 5), NOTE_SIXTEENTH, 0 },
    { TONE(E, 5), NOTE_SIXTEENTH, 128 },
    { TONE(E, 5), NOTE_SIXTEENTH, 0 },

// 7 -- one horse open

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(D, 5), NOTE_EIGHTH, 128 },
    { TONE(D, 5), NOTE_EIGHTH, 0 },

    { TONE(D, 5), NOTE_EIGHTH, 128 },
    { TONE(D, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

// 8 -- sleigh - hey!
    { TONE(D, 5), NOTE_HALF, 128 },
    { TONE(G, 5), NOTE_HALF, 128 },

// 1 -- jingle bells
    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH + NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

// 2 -- jingle bells
    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH + NOTE_QUARTER, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

// 3 -- jingle all the
    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },

    { TONE(C, 5), NOTE_QUARTER + NOTE_EIGHTH, 128 },
    { TONE(D, 5), NOTE_EIGHTH, 128 },

// 4 -- way
    { TONE(E, 5), NOTE_WHOLE, 128 },

// 5 -- oh what fun it
    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },

    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },

    { TONE(F, 5), NOTE_QUARTER, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },
    { TONE(F, 5), NOTE_SIXTEENTH, 128 },
    { TONE(F, 5), NOTE_SIXTEENTH, 0 },

// 6 -- is to ride

    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_EIGHTH, 128 },
    { TONE(E, 5), NOTE_EIGHTH, 0 },

    { TONE(E, 5), NOTE_SIXTEENTH, 128 },
    { TONE(E, 5), NOTE_SIXTEENTH, 0 },
    { TONE(E, 5), NOTE_SIXTEENTH, 128 },
    { TONE(E, 5), NOTE_SIXTEENTH, 0 },

// 7 -- one horse open

    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },

    { TONE(G, 5), NOTE_EIGHTH, 128 },
    { TONE(G, 5), NOTE_EIGHTH, 0 },

    { TONE(F, 5), NOTE_EIGHTH, 128 },
    { TONE(F, 5), NOTE_EIGHTH, 0 },

    { TONE(D, 5), NOTE_EIGHTH, 128 },
    { TONE(D, 5), NOTE_EIGHTH, 0 },

// 8 -- sleigh
    { TONE(C, 5), NOTE_WHOLE, 128 },

    {0, 0, 0}
};
