#include "scanlistwidget.h"
#include <QDir>

class ScanListWidgetPrivate {
public:
	ScanListWidget *q;
	QString m_path;
};

ScanListWidget::ScanListWidget() 
{
	d=new ScanListWidgetPrivate;
	d->q=this;
	
	connect(this,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(slot_item_clicked()));
}

ScanListWidget::~ScanListWidget()
{
	delete d;
}
void ScanListWidget::setPath(const QString &path) {
	d->m_path=path;
}
void ScanListWidget::refresh() {
	clear();
	setHeaderLabels(QStringList("Scans"));
	QStringList folder_list=QDir(d->m_path+"/acquisitions").entryList(QStringList("*"),QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);
	foreach (QString folder,folder_list) {
		QTreeWidgetItem *it=new QTreeWidgetItem;
		it->setText(0,folder);
		it->setData(0,Qt::UserRole,d->m_path+"/acquisitions/"+folder);
		addTopLevelItem(it);
	}
}
void ScanListWidget::slot_item_clicked() {
	emit scanClicked();
}
QString ScanListWidget::currentScanPath() {
	QTreeWidgetItem *it=currentItem();
	if (!it) return "";
	return it->data(0,Qt::UserRole).toString();
}
QStringList ScanListWidget::allScanPaths() {
	QStringList ret;
	for (int i=0; i<topLevelItemCount(); i++) {
		QTreeWidgetItem *it=topLevelItem(i);
		ret << it->data(0,Qt::UserRole).toString();
	}
	return ret;
}
