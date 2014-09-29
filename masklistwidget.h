#ifndef masklistwidget_H
#define masklistwidget_H

#include <QTreeWidget>
#include <QString>

class MaskListWidgetPrivate;
class MaskListWidget : public QTreeWidget {
	Q_OBJECT
public:
	friend class MaskListWidgetPrivate;
	MaskListWidget();
	virtual ~MaskListWidget();
	void setPath(const QString &path);
	void refresh();
	QString currentMaskPath();
	QStringList allMaskPaths();
private slots:
	void slot_item_clicked();
signals:
	void maskClicked();
private:
	MaskListWidgetPrivate *d;
};

#endif
