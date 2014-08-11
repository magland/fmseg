#include "fmsegwidget.h"
#include <QVBoxLayout>
#include "fmsegview.h"

class FMSegWidgetPrivate {
public:
	FMSegWidget *q;
	
	Array3D m_array;
	Array3D m_mask;
	QWidget *m_widget;
	QVBoxLayout *m_layout;
	QList<FMSegView *> m_views;
};

FMSegWidget::FMSegWidget() 
{
	d=new FMSegWidgetPrivate;
	d->q=this;
	d->m_widget=new QWidget;
	d->m_layout=new QVBoxLayout;
	
	setWidget(d->m_widget);
	setWidgetResizable(true);
	d->m_widget->setLayout(d->m_layout);
}

FMSegWidget::~FMSegWidget()
{
	delete d;
}
void FMSegWidget::setArray(const Array3D &X) {
	d->m_array=X;
	d->m_widget->setMinimumSize(512,512*X.N3());
}
void FMSegWidget::setMask(const Array3D &X) {
	d->m_mask=X;
}
void FMSegWidget::refresh() {
	qDeleteAll(d->m_views);
	for (int z=0; z<d->m_array.N3(); z++) {
		Array2D slice=d->m_array.dataXY(z);
		Array2D Mslice=d->m_mask.dataXY(z);
		FMSegView *W=new FMSegView;
		W->setArray(slice);
		W->setMask(Mslice);
		d->m_layout->addWidget(W);
		W->setMinimumSize(512,512);
	}
}
void FMSegWidget::resizeEvent(QResizeEvent *event) {
	
}

