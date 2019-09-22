#include "send_event_text_edit.h"
#include <QKeyEvent>

void send_event_text_edit::keyPressEvent( QKeyEvent* event )
{
    if ( event->key() == Qt::Key_Return )
    {
        emit return_pressed();
    }
    else
    {
        QTextEdit::keyPressEvent(event);
    }
}
