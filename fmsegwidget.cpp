#include "fmsegwidget.h"
#include <QVBoxLayout>
#include "fmsegview.h"
#include <QLabel>

class FMSegWidgetPrivate {
public:
	FMSegWidget *q;
	
	Array3D m_array;
	Array3D m_mask;
	QWidget *m_widget;
	QVBoxLayout *m_layout;
	QGridLayout *m_seg_layout;
	FMSegView *m_XZ_view;
	FMSegView *m_YZ_view;
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
	d->m_seg_layout=new QGridLayout;
	d->m_window_min=0;
	d->m_window_max=0;
	d->m_selection_threshold=100;
	d->m_XZ_view=new FMSegView; d->m_XZ_view->setMinimumSize(512,256);
	d->m_YZ_view=new FMSegView; d->m_YZ_view->setMinimumSize(512,256);

	QGridLayout *glayout=new QGridLayout;
	
	d->m_layout->addLayout(glayout);
	glayout->addWidget(new QLabel("Reslice XZ:"),0,0);
	glayout->addWidget(new QLabel("Reslice YZ:"),0,1);
	glayout->addWidget(d->m_XZ_view,1,0);
	glayout->addWidget(d->m_YZ_view,1,1);
	d->m_layout->addWidget(new QLabel("Slices for segmentation:"));
	d->m_layout->addLayout(d->m_seg_layout);
	d->m_layout->addStretch();
	
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
	d->m_widget->setMinimumSize(512,256*3+256*X.N3());
}
void FMSegWidget::setMask(const Array3D &X) {
	d->m_mask=X;
}
Array2D do_interp(const Array2D &X,int factor) {
	Array2D Y; Y.allocate(X.N1(),X.N2()*factor);
	for (int y=0; y<Y.N2(); y++)
	for (int x=0; x<Y.N1(); x++) {
		Y.setValue(X.value(x,y/factor),x,y);
	}
	return Y;
}
void FMSegWidget::refresh() {
	d->m_XZ_view->setArray(do_interp(d->m_array.dataXZ(d->m_array.N2()/2),3));	
	d->m_YZ_view->setArray(do_interp(d->m_array.dataYZ(d->m_array.N1()/2),3));
	d->m_XZ_view->refresh(); d->m_YZ_view->refresh();
	
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
		d->m_seg_layout->addWidget(W,z/3,z%3);
		W->setMinimumSize(512,512);
		d->m_views << W;
	}
	d->m_XZ_view->setWindowRange(d->m_window_min,d->m_window_max);
	d->m_YZ_view->setWindowRange(d->m_window_min,d->m_window_max);
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
	d->m_XZ_view->setWindowRange(d->m_window_min,d->m_window_max);
	d->m_YZ_view->setWindowRange(d->m_window_min,d->m_window_max);
}

void FMSegWidget::setSelectionThreshold(float threshold) {
	d->m_selection_threshold=threshold;
	for (int z=0; z<d->m_views.count(); z++) {
		d->m_views[z]->setSelectionThreshold(d->m_selection_threshold);
	}
}
