#ifndef fmsegwindow_H
#define fmsegwindow_H

#include <QWidget>

class FMSegWindowPrivate;
class FMSegWindow : public QWidget {
	Q_OBJECT
public:
	friend class FMSegWindowPrivate;
	FMSegWindow();
	virtual ~FMSegWindow();
	void setSessionPath(const QString &path);
	void refresh();
private slots:
	void slot_scan_clicked();
	void slot_save();
	void slot_update();
	void slot_compute();
	void slot_clear_mask();
	void slot_compile_results();
private:
	FMSegWindowPrivate *d;
};

#endif
