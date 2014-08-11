#ifndef fmsegview_H
#define fmsegview_H

#include "array2d.h"
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

class FMSegViewPrivate;
class FMSegView : public QWidget {
public:
	friend class FMSegViewPrivate;
	FMSegView();
	virtual ~FMSegView();
	void setArray(const Array2D &X);
	void setMask(const Array2D &X);
	void refresh();
protected:
	void paintEvent(QPaintEvent * event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
private:
	FMSegViewPrivate *d;
};

#endif