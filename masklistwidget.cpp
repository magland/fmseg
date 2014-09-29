#include "masklistwidget.h"
#include <QDir>

class MaskListWidgetPrivate {
public:
	MaskListWidget *q;
	QString m_path;
};

MaskListWidget::MaskListWidget() 
{
	d=new MaskListWidgetPrivate;
	d->q=this;
	
	connect(this,SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(slot_item_clicked()));
}

MaskListWidget::~MaskListWidget()
{
	delete d;
}
void MaskListWidget::setPath(const QString &path) {
	d->m_path=path;
}
void MaskListWidget::refresh() {
	clear();
	setHeaderLabels(QStringList("Masks"));
	QStringList file_list=QDir(d->m_path+"/attachments").entryList(QStringList("mask*.mda"),QDir::Files,QDir::Name);
	foreach (QString file,file_list) {
		QTreeWidgetItem *it=new QTreeWidgetItem;
		it->setText(0,file);
		it->setData(0,Qt::UserRole,d->m_path+"/attachments/"+file);
		addTopLevelItem(it);
	}
}
void MaskListWidget::slot_item_clicked() {
	emit maskClicked();
}
QString MaskListWidget::currentMaskPath() {
	QTreeWidgetItem *it=currentItem();
	if (!it) return "";
	return it->data(0,Qt::UserRole).toString();
}
QStringList MaskListWidget::allMaskPaths() {
	QStringList ret;
	for (int i=0; i<topLevelItemCount(); i++) {
		QTreeWidgetItem *it=topLevelItem(i);
		ret << it->data(0,Qt::UserRole).toString();
	}
	return ret;
}
