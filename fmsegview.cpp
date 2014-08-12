#include "fmsegview.h"
#include <QPainter>
#include <QColor>
#include <QPoint>
#include <QDebug>
#include "seg_update_mask.h"
#include <QList>

class FMSegViewPrivate {
public:
	FMSegView *q;
	Array2D m_array;
	Array2D m_array_filtered;
	Array2D m_mask;
	Array2D m_preview_mask;
	bool m_need_to_update_the_image;
	QImage m_the_image;
	QRect m_target_rect;
	float m_window_min,m_window_max;
	float m_selection_threshold;
	int m_median_filter_radius;
	QPoint m_last_index;
	
	QColor get_color(float X);
	QPoint point_to_index(QPoint pt);
	void on_mouse_click(QPoint index);
	void on_mouse_move(QPoint index);
	Array2D compute_mask_boundary(const Array2D &mask);
	Array2D compute_median_filter(const Array2D &array,int rad);
};

FMSegView::FMSegView() : QWidget()
{
	d=new FMSegViewPrivate;
	d->q=this;
	d->m_selection_threshold=100;
	d->m_median_filter_radius=1;
	d->m_need_to_update_the_image=false;
	d->m_window_min=d->m_window_max=0;
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
}

FMSegView::~FMSegView()
{
	delete d;
}
void FMSegView::setArray(const Array2D &X) {
	d->m_array=X;
	if (d->m_window_max==0) {
		d->m_window_min=0;
		d->m_window_max=X.max()*0.6;
	}
	d->m_array_filtered=d->compute_median_filter(d->m_array,d->m_median_filter_radius);
	refresh();
}
void FMSegView::setMask(const Array2D &X) {
	d->m_mask=X;
	refresh();
}
void FMSegView::refresh() {
	d->m_need_to_update_the_image=true;
	repaint();
}

void FMSegView::paintEvent(QPaintEvent * event) {
	QPainter painter(this);
	if (d->m_need_to_update_the_image) {
		Array2D mask_boundary=d->compute_mask_boundary(d->m_mask);
		Array2D preview_mask_boundary=d->compute_mask_boundary(d->m_preview_mask);
		d->m_need_to_update_the_image=false;
		d->m_the_image=QImage(1,1,QImage::Format_RGB32);
		if ((d->m_array.N1()>0)&&(d->m_array.N2()>0)) {
			d->m_the_image=QImage(d->m_array.N1(),d->m_array.N2(),QImage::Format_RGB32);
			for (qint32 y=0; y<d->m_array.N2(); y++)			
			for (qint32 x=0; x<d->m_array.N1(); x++) {
				QColor col=d->get_color(d->m_array_filtered.getValue(x,y));
				if (preview_mask_boundary.getValue(x,y)) {
					col=qRgb(255,200,200);
				}
				if (mask_boundary.getValue(x,y)) {
					col=qRgb(0,0,255);
				}
				d->m_the_image.setPixel(x,y,qRgb(col.red(),col.green(),col.blue()));
			}
		}
	}
	int iwidth=d->m_the_image.width();
	int iheight=d->m_the_image.height();
	if (width()*iheight<iwidth*height()) { //width is the limiting direction
		int target_height=(int)(width()*iheight/(double)iwidth);
		d->m_target_rect=QRect(0,(height()-target_height)/2,width(),target_height);
	}
	else { //height is the limiting direction
		int target_width=(int)(height()*iwidth/(double)iheight);
		d->m_target_rect=QRect((width()-target_width)/2,0,target_width,height());
	}
	painter.drawImage(d->m_target_rect,d->m_the_image,QRect(0,0,d->m_the_image.width(),d->m_the_image.height()));
}

void FMSegView::mousePressEvent(QMouseEvent *event) {
	if ((event->button()&Qt::LeftButton)||(event->button()&Qt::RightButton)) {
		QPoint index=d->point_to_index(event->pos());
		d->on_mouse_click(index);
	}
}
void FMSegView::mouseMoveEvent(QMouseEvent *event) {
	QPoint index=d->point_to_index(event->pos());
	d->m_last_index=index;
	d->on_mouse_move(index);
	setFocus(Qt::MouseFocusReason);
}

QColor FMSegViewPrivate::get_color(float X) {
	float wmin=m_window_min;;
	float wmax=m_window_max;
	float val=abs(X);
	int hold=0;
	if (wmin==wmax) {
		if (val<wmin) hold=0;
		else if (val>wmax) hold=255;
		else hold=127;
	}
	else if (wmax>wmin) {
		if (val<wmin) val=wmin;
		if (val>wmax) val=wmax;
		hold=(int)((val-wmin)/(wmax-wmin)*255);
	}
	return qRgb(hold,hold,hold);	
}

QPoint FMSegViewPrivate::point_to_index(QPoint pt) {
	if (m_target_rect.width()==0) return QPoint(0,0);
	if (m_target_rect.height()==0) return QPoint(0,0);
	float pctx=((float)pt.x()-m_target_rect.x())/(m_target_rect.width());
	float pcty=((float)pt.y()-m_target_rect.y())/(m_target_rect.height());
	int retx=(int)(pctx*m_array.N1());
	int rety=(int)(pcty*m_array.N2());
	if (retx<0) retx=0;
	if (retx>=m_array.N1()) retx=m_array.N1()-1;
	if (rety<0) rety=0;
	if (rety>=m_array.N2()) rety=m_array.N2()-1;
	return QPoint(retx,/*m_array.N2()-1-*/rety);
}
void FMSegViewPrivate::on_mouse_click(QPoint index) {
	float threshold=m_selection_threshold;
	seg_update_mask(m_array_filtered,m_mask,index,threshold);
	q->refresh();
}
void FMSegViewPrivate::on_mouse_move(QPoint index) {
	float threshold=m_selection_threshold;
	m_preview_mask=m_mask;
	seg_update_mask(m_array_filtered,m_preview_mask,index,threshold);
	q->refresh();
}
Array2D FMSegViewPrivate::compute_mask_boundary(const Array2D &mask) {
	Array2D B;
	int N1=mask.N1();
	int N2=mask.N2();
	B.allocate(N1,N2);
	for (int y=0; y<N2; y++)
	for (int x=0; x<N1; x++) {
		if (mask.getValue(x,y)) {
			for (int dy=-1; dy<=1; dy++)
			for (int dx=-1; dx<=1; dx++) {
				if ((!mask.getValue(x+dx,y+dy))&&(!B.getValue(x,y))) {
					B.setValue(1,x,y);
				}
			}
		}
	}
	return B;
}
Array2D FMSegViewPrivate::compute_median_filter(const Array2D &array,int radius) {
	Array2D ret; ret.allocate(array.N1(),array.N2());
	for (int y=0; y<array.N2(); y++)
	for (int x=0; x<array.N1(); x++) {
		QList<float> list;
		for (int dy=-radius; dy<=radius; dy++)
		for (int dx=-radius; dx<=radius; dx++) {
			list << array.getValue(x+dx,y+dy);
		}
		qSort(list);
		ret.setValue(list[list.count()/2],x,y);
	}
	return ret;
}
void FMSegView::keyPressEvent(QKeyEvent *event) {
	if (event->key()==61) { //plus
		d->m_selection_threshold+=10;
		d->on_mouse_move(d->m_last_index);
	}
	else if (event->key()==45) { //minus
		d->m_selection_threshold-=10;
		d->on_mouse_move(d->m_last_index);
	}
	else if (event->key()==Qt::Key_C) {
		d->m_mask.allocate(d->m_array.N1(),d->m_array.N2());
		refresh();
	}
	else {
		qDebug()  << event->key();
	}
}
void FMSegView::setMedianFilterRadius(int rad) {
	d->m_median_filter_radius=rad;
	d->m_array_filtered=d->compute_median_filter(d->m_array,d->m_median_filter_radius);
	refresh();
}
Array2D FMSegView::getMask() {
	return d->m_mask;
}
void FMSegView::setWindowRange(float min,float max) {
	d->m_window_min=min;
	d->m_window_max=max;
	refresh();
}
void FMSegView::setSelectionThreshold(float threshold) {
	d->m_selection_threshold=threshold;
	refresh();	
}
