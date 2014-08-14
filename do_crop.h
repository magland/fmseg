#ifndef do_crop_H
#define do_crop_H

#include <QString>
#include <QWidget>

void do_crop(const QString &scan_path);

class do_crop_object : public QObject {
Q_OBJECT
public:
	do_crop_object() {}
	virtual ~do_crop_object() {}
	QString scan_path;
	QWidget *widget;
	
public slots:
	void slot_point_clicked(QPoint index);	
	
};

#endif

