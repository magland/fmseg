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
	float m_window_min;
	float m_window_max;
	float m_selection_threshold;
};

FMSegWidget::FMSegWidget() 
{
	d=new FMSegWidgetPrivate;
	d->q=this;
	d->m_widget=new QWidget;
	d->m_layout=new QVBoxLayout;
	d->m_window_min=0;
	d->m_window_max=0;
	d->m_selection_threshold=100;
	
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
	d->m_views.clear();
	for (int z=0; z<d->m_array.N3(); z++) {
		Array2D slice=d->m_array.dataXY(z);
		Array2D Mslice=d->m_mask.dataXY(z);
		FMSegView *W=new FMSegView;
		W->setArray(slice);
		W->setMask(Mslice);
		W->setWindowRange(d->m_window_min,d->m_window_max);
		W->setSelectionThreshold(d->m_selection_threshold);
		d->m_layout->addWidget(W);
		W->setMinimumSize(512,512);
		d->m_views << W;
	}
}
void FMSegWidget::resizeEvent(QResizeEvent *event) {
	
}
Array3D FMSegWidget::getMask() {
	int N1=d->m_array.N1();
	int N2=d->m_array.N2();
	int N3=d->m_array.N3();
	Array3D ret(N1,N2,N3);
	for (int z=0; z<N3; z++) {
		if (z<d->m_views.count()) {
			FMSegView *W=d->m_views[z];
			Array2D Mslice=W->getMask();
			ret.setDataXY(Mslice,z);
		}
	}
	return ret;
}
void FMSegWidget::setWindowRange(float min,float max) {
	d->m_window_min=min;
	d->m_window_max=max;
	for (int z=0; z<d->m_views.count(); z++) {
		d->m_views[z]->setWindowRange(d->m_window_min,d->m_window_max);
	}
}

void FMSegWidget::setSelectionThreshold(float threshold) {
	d->m_selection_threshold=threshold;
	for (int z=0; z<d->m_views.count(); z++) {
		d->m_views[z]->setSelectionThreshold(d->m_selection_threshold);
	}
}
