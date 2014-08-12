#ifndef fmsegwidget_H
#define fmsegwidget_H

#include <QScrollArea>
#include "array3d.h"

class FMSegWidgetPrivate;
class FMSegWidget : public QScrollArea {
public:
	friend class FMSegWidgetPrivate;
	FMSegWidget();
	virtual ~FMSegWidget();
	void setArray(const Array3D &X);
	void setMask(const Array3D &X);
	void setWindowRange(float min,float max);
	void setSelectionThreshold(float threshold);
	Array3D getMask();
	void refresh();
protected:
	void resizeEvent( QResizeEvent * event );
private:
	FMSegWidgetPrivate *d;
};

#endif
