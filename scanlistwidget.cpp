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
	QStringList file_list=QDir(d->m_path).entryList(QStringList("*"),QDir::Files,QDir::Name);
	foreach (QString file,file_list) {
		QTreeWidgetItem *it=new QTreeWidgetItem;
		it->setText(0,file);
		it->setData(0,Qt::UserRole,d->m_path+"/"+file);
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
