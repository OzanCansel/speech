#include <QTextEdit>

class send_event_text_edit : public QTextEdit
{
    Q_OBJECT

protected:

    void keyPressEvent( QKeyEvent* ) override ;

signals:

    void return_pressed();

};
