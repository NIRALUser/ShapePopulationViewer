
#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QResizeEvent>

/**
 * Extension of the QDockWidget class to handle resizing of children of children of the widget effectively.  Basically everything about QDockWidget remains
 * unchanged except it's resizeEvent function is overridden.  A constructor and instance variable is added to faciliate.
 * @brief The ViewportWidget class
 */
class ViewportWidget : public QDockWidget {
private:
    QWidget *scrollAreaContents;
protected:
    /**
     * Overriden resizeEvent handler.  Basically, make the super call, then make a standardized resize to the instance variable.  (In our case this is the contents
     * of the scroll area whose parent is the dock widget).
     * @brief resizeEvent
     * @param event
     */
    void resizeEvent(QResizeEvent *event)  {
        QDockWidget::resizeEvent(event);
        QSize dockSize = size();
        this->scrollAreaContents->resize(dockSize.width()-20,dockSize.height());
    }

public:
    /**
     * Additional constructor, calls superconstructor and initializes an instance variable (in our case the contents of the scroll area whose parent is the dock widget).
     * @brief ViewportWidget
     * @param parent
     * @param areaConts
     */
    ViewportWidget(QWidget *parent, QWidget *areaConts) : QDockWidget(parent) {
        this->scrollAreaContents = areaConts;
    }
    /**
     * Clarity function returning the size of the dock widget.
     * @brief size
     * @return
     */
    QSize size() {
        return QDockWidget::size();
    }
};
