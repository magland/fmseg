#ifndef fbsegmainwidget_H
#define fbsegmainwidget_H

#include <QString>
#include <QWidget>

class FBSegMainWidgetPrivate;
class FBSegMainWidget : public QWidget {
public:
	friend class FBSegMainWidgetPrivate;
	FBSegMainWidget();
	virtual ~FBSegMainWidget();
	void setInputPath(const QString &path);
	void setOutputPath(const QString &path);
	void refresh();
private slots:
private:
	FBSegMainWidgetPrivate *d;
};

#endif
