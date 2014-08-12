#ifndef scanlistwidget_H
#define scanlistwidget_H

#include <QListWidget>
#include <QString>

class ScanListWidgetPrivate;
class ScanListWidget : public QListWidget {
public:
	friend class ScanListWidgetPrivate;
	ScanListWidget();
	virtual ~ScanListWidget();
	void setPath(const QString &path);
private:
	ScanListWidgetPrivate *d;
};

#endif
