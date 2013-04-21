
#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QResizeEvent>

class ViewportWidget : public QDockWidget {
private:
    QWidget *scrollAreaContents;
protected:
    void resizeEvent(QResizeEvent *event)  {
        QDockWidget::resizeEvent(event);
        QSize dockSize = size();
        this->scrollAreaContents->resize(dockSize.width()-20,dockSize.height());
    }

public:
    //QWidget *scrollAreaContents;
    ViewportWidget(QWidget *parent, QWidget *areaConts) : QDockWidget(parent) {
        this->scrollAreaContents = areaConts;
    }
    QSize size() {
        return QDockWidget::size();
    }
};
