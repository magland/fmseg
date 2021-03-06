#ifndef fmsegview_H
#define fmsegview_H

#include "array2d.h"
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>

class FMSegViewPrivate;
class FMSegView : public QWidget {
	Q_OBJECT
public:
	friend class FMSegViewPrivate;
	FMSegView();
	virtual ~FMSegView();
	void setMedianFilterRadius(int rad);
	void setArray(const Array2D &X);
	void setMask(const Array2D &X);
	void setWindowRange(float min,float max);
	void setSelectionThreshold(float threshold);
	Array2D getMask();
	void refresh();
signals:
	void pointClicked(QPoint index);
protected:
	void paintEvent(QPaintEvent * event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void leaveEvent(QEvent *event);
	void keyPressEvent(QKeyEvent *event);	
private:
	FMSegViewPrivate *d;
};

#endif
