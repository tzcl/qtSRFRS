#include "flashcard.h"

// need to have front/back (text/images/audio)

SRFRS::Flashcard::Flashcard() :
    _frontText(""),
    _backText("")
{

}

SRFRS::Flashcard::Flashcard(QString front = "", QString back = "") :
    _frontText(front),
    _backText(back)
{

}
