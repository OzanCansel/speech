#include "send_event_text_edit.h"
#include <QKeyEvent>

void send_event_text_edit::keyPressEvent( QKeyEvent* event )
{
    if ( event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier )
    {
        emit return_pressed();
    }
    else
    {
        QTextEdit::keyPressEvent(event);
    }
}
