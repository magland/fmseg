#include "scanlistwidget.h"

class ScanListWidgetPrivate {
public:
	ScanListWidget *q;
	QString m_path;
};

ScanListWidget::ScanListWidget() 
{
	d=new ScanListWidgetPrivate;
	d->q=this;
}

ScanListWidget::~ScanListWidget()
{
	delete d;
}
void ScanListWidget::setPath(const QString &path) {
	d->m_path=path;
}

