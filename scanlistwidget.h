#ifndef scanlistwidget_H
#define scanlistwidget_H

#include <QTreeWidget>
#include <QString>

class ScanListWidgetPrivate;
class ScanListWidget : public QTreeWidget {
	Q_OBJECT
public:
	friend class ScanListWidgetPrivate;
	ScanListWidget();
	virtual ~ScanListWidget();
	void setPath(const QString &path);
	void refresh();
	QString currentScanPath();
private slots:
	void slot_item_clicked();
signals:
	void scanClicked();
private:
	ScanListWidgetPrivate *d;
};

#endif
