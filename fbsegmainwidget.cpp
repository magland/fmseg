#include "fbsegmainwidget.h"
#include <QHBoxLayout>
#include "fmsegwidget.h"
#include "scanlistwidget.h"

class FBSegMainWidgetPrivate {
public:
	FBSegMainWidget *q;
	QString m_input_path;
	QString m_output_path;
	FMSegWidget *m_seg_widget;
	ScanListWidget *m_scan_list_widget;
};

FBSegMainWidget::FBSegMainWidget() 
{
	d=new FBSegMainWidgetPrivate;
	d->q=this;
	d->m_seg_widget=new FMSegWidget;	
	d->m_scan_list_widget=new ScanListWidget;
	
	QHBoxLayout *layout=new QHBoxLayout;
	setLayout(layout);
	
	layout->addWidget(d->m_scan_list_widget);
	layout->addWidget(d->m_seg_widget);
	
	d->m_scan_list_widget->setFixedWidth(250);
}

FBSegMainWidget::~FBSegMainWidget()
{
	delete d;
}
void FBSegMainWidget::setInputPath(const QString &path) {
	d->m_input_path=path;
}
void FBSegMainWidget::setOutputPath(const QString &path) {
	d->m_output_path=path;
}
void FBSegMainWidget::refresh() {
	
}

