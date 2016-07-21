#ifndef VIEWCURRENTPOST_H
#define VIEWCURRENTPOST_H

#include <QDialog>
#include <QtSql>
namespace Ui {
	class viewCurrentPost;
}

class viewCurrentPost : public QDialog
{
	Q_OBJECT
	QString pid;
public:
	explicit viewCurrentPost(QWidget *parent = 0);
	~viewCurrentPost();
	QString getMassage();
	QString getSourceURL();
private slots:
	void recieveCurrentImage(QString pid, QString mediaURL);
	void on_toBlackListButton_clicked();

private:
	Ui::viewCurrentPost *ui;
};

#endif // VIEWCURRENTPOST_H
