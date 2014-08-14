#include "do_crop.h"
#include "read_3d_array.h"
#include "fmsegview.h"
#include <QFileInfo>
#include <QDebug>

void do_crop(const QString &scan_path) {
	Array3D X=read_3d_array(scan_path);
	
	Array2D X0=X.dataXY(X.N3()/2);
	FMSegView *W=new FMSegView;
	W->setArray(X0);
	W->show();
	
	do_crop_object *OO=new do_crop_object;
	OO->scan_path=scan_path;
	OO->widget=W;

	QObject::connect(W,SIGNAL(pointClicked(QPoint)),OO,SLOT(slot_point_clicked(QPoint)));	
}

Array3D do_the_crop(const Array3D &X,QPoint index,int M1,int M2) {
	Array3D Y;
	Y.allocate(M1,M2,X.N3());

	int xx=index.x()-M1/2;
	int yy=index.y()-M2/2;
	
	for (int z=0; z<X.N3(); z++)
	for (int y=0; y<X.N2(); y++)
	for (int x=0; x<X.N1(); x++) {
		Y.setValue(X.value(x+xx,y+yy,z),x,y,z);
	}
	
	return Y;
}
void do_crop_object::slot_point_clicked(QPoint index) {
	QString session_path=QFileInfo(QFileInfo(scan_path).path()).path();	
	
	Array3D X=read_3d_array(scan_path);
	Array3D CC=do_the_crop(X,index,128,128);
	QString crop_path=session_path+"/crops/"+QFileInfo(scan_path).completeBaseName()+".crop.mda";
	qDebug() << "Writing..." << crop_path;
	CC.write(crop_path);
	
	//to migrate the old stuff -- to be removed later
	/*
	QString mask_path=session_path+"/masks/"+QFileInfo(scan_path).completeBaseName()+".mask.mda";	
	QString mask_old_path=session_path+"/masks-old/"+QFileInfo(scan_path).completeBaseName()+".mask.mda";	
	if ((!QFile::exists(mask_path))&&(QFile::exists(mask_old_path))) {
		Array3D MM=read_3d_array(mask_old_path);
		Array3D MCC=do_the_crop(MM,index,128,128);
		MCC.write(mask_path);
	}
	*/
	
	delete(widget);
}


