#include "fmsegwindow.h"
#include "scanlistwidget.h"
#include "fmsegwidget.h"
#include <QHBoxLayout>
#include <QFileInfo>
#include "jniftiimage2.h"
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include "read_3d_array.h"
#include "do_crop.h"

class FMSegWindowPrivate {
public:
	FMSegWindow *q;
	QString m_session_path;
	ScanListWidget *m_SLW;
	FMSegWidget *m_widget;
	QLineEdit *m_window_max_edit;
	QLineEdit *m_selection_threshold_edit;
};

FMSegWindow::FMSegWindow() 
{
	d=new FMSegWindowPrivate;
	d->q=this;
	
	d->m_SLW=new ScanListWidget;
	d->m_widget=new FMSegWidget;
	
	d->m_window_max_edit=new QLineEdit; d->m_window_max_edit->setText("500");
	d->m_selection_threshold_edit=new QLineEdit; d->m_selection_threshold_edit->setText("100");
	QPushButton *update_button=new QPushButton("Update");
	connect(update_button,SIGNAL(clicked()),this,SLOT(slot_update()));
	
	QWidget *controls=new QWidget;
	QVBoxLayout *clayout=new QVBoxLayout;
	{
		QPushButton *BB=new QPushButton("Compute");
		connect(BB,SIGNAL(clicked()),this,SLOT(slot_compute()));
		clayout->addWidget(BB);
	}
	{
		QPushButton *BB=new QPushButton("Save");
		connect(BB,SIGNAL(clicked()),this,SLOT(slot_save()));
		clayout->addWidget(BB);
	}	
	QGridLayout *glayout=new QGridLayout;
	glayout->addWidget(new QLabel("Window max:"),0,0);
	glayout->addWidget(d->m_window_max_edit,0,1);
	glayout->addWidget(new QLabel("Selection threshold:"),1,0);
	glayout->addWidget(d->m_selection_threshold_edit,1,1);
	glayout->addWidget(update_button,2,1);
	clayout->addLayout(glayout);
	{
		QPushButton *BB=new QPushButton("Clear Mask");
		connect(BB,SIGNAL(clicked()),this,SLOT(slot_clear_mask()));
		clayout->addWidget(BB);
	}	
	{
		QPushButton *BB=new QPushButton("Compile Results");
		connect(BB,SIGNAL(clicked()),this,SLOT(slot_compile_results()));
		clayout->addWidget(BB);
	}
	clayout->addStretch();
	controls->setLayout(clayout);	
	
	QVBoxLayout *left_layout=new QVBoxLayout;
	left_layout->addWidget(d->m_SLW);
	left_layout->addWidget(controls);
	controls->setFixedHeight(250);
	controls->setFixedWidth(300);
	d->m_SLW->setFixedWidth(300);
	
	QHBoxLayout *layout=new QHBoxLayout;
	layout->addLayout(left_layout);
	layout->addWidget(d->m_widget);
	setLayout(layout);
	
	connect(d->m_SLW,SIGNAL(scanClicked()),this,SLOT(slot_scan_clicked()));
}

FMSegWindow::~FMSegWindow()
{
	delete d;
}
void FMSegWindow::setSessionPath(const QString &path) {
	d->m_session_path=path;
}
void FMSegWindow::refresh() {
	d->m_SLW->setPath(d->m_session_path);
	d->m_SLW->refresh();
	slot_update();
}

void FMSegWindow::slot_scan_clicked() {
	QString path=d->m_SLW->currentScanPath();
	QString mask_path=path+"/attachments/mask.mda";
	QString crop_path=path+"/attachments/crop.mda";
	
	if (!QFile::exists(crop_path)) {
		do_crop(path);
		return;
	}
	
	Array3D X,M;
	X=read_3d_array(crop_path);
	X.scaleBy(500.0/X.max());
	M.allocate(X.N1(),X.N2(),X.N3());
	
	if (QFile::exists(mask_path)) {
		M=read_3d_array(mask_path);
	}
	
	d->m_widget->setArray(X);
	d->m_widget->setMask(M);
	d->m_widget->refresh();
	d->m_widget->setProperty("input_path",path);
	d->m_widget->setProperty("mask_path",mask_path);
	
}
int compute_mask_count(const Array3D &mask) {
	int count=0;
	for (int z=0; z<mask.N3(); z++)
	for (int y=0; y<mask.N2(); y++)
	for (int x=0; x<mask.N1(); x++) {
		if (mask.value(x,y,z)) count++;
	}
	return count;
}
void FMSegWindow::slot_compute() {
	Array3D mask=d->m_widget->getMask();
	int count=compute_mask_count(mask);
	QMessageBox::information(0,"Compute",QString("Number of pixels in mask: %1").arg(count));
}
void FMSegWindow::slot_save() {
	QString mask_path=d->m_widget->property("mask_path").toString();
	Array3D mask=d->m_widget->getMask();
	mask.write(mask_path);
	QMessageBox::information(0,"Saved mask","Saved mask: "+mask_path);
}
void FMSegWindow::slot_update() {
	double window_max=d->m_window_max_edit->text().toDouble();
	double selection_threshold=d->m_selection_threshold_edit->text().toDouble();
	d->m_widget->setWindowRange(0,window_max);
	d->m_widget->setSelectionThreshold(selection_threshold);
}
void FMSegWindow::slot_clear_mask() {
	Array3D mask=d->m_widget->getMask();
	mask.allocate(mask.N1(),mask.N2(),mask.N3());
	d->m_widget->setMask(mask);
	d->m_widget->refresh();
}
void FMSegWindow::slot_compile_results() {
	QString txt="Scan,Pixel Count\n";
	QStringList paths=d->m_SLW->allScanPaths();
	for (int i=0; i<paths.count(); i++) {
		QString mask_path=paths[i]+"/attachments/mask.mda";
		Array3D mask=read_3d_array(mask_path);
		qDebug() << mask.N1() << mask.N2() << mask.N3();
		int count0=compute_mask_count(mask);
		txt+=QString("%1,%2\n").arg(QFileInfo(paths[i]).completeBaseName()).arg(count0);
	}
	QTextEdit *EE=new QTextEdit;
	EE->setText(txt);
	EE->show();
}

