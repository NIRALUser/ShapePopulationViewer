#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QResizeEvent>

/**
 * Extension of the QDockWidget class to handle resizing of children of children of the widget effectively.  Basically everything about QDockWidget remains
 * unchanged except it's resizeEvent function is overridden.  A constructor and instance variable is added to faciliate.  The instance variable is a general
 * QWidget.  On any call to QDockWidget's resizeEvent, the QWidget will be resized in a regular manner.
 * @brief The ViewportWidget class
 */
class ViewportWidget : public QDockWidget {
private:
    QWidget *scrollAreaContents;
protected:
    /**
     * Overriden resizeEvent handler.  Basically, make the super call, then make a standardized resize to the instance variable.  (In our case this is the contents
     * of the scroll area whose parent is the dock widget). The width will be 20 units less than that of the dock widget, and the height will be equal to that of the
     * dock widget.
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
     * Additional constructor, calls superconstructor and initializes an instance variable (in our case the contents of the scroll area whose parent is the dock widget),
     * which will be the object the viewport also resizes on any resizeEvent.
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
     * @return size of the dockWidget
     */
    QSize size() {
        return QDockWidget::size();
    }
};
