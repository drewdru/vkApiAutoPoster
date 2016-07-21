#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>
#include <QNetworkCookieJar>
namespace Ui {
	class Settings;
}

class Settings : public QDialog
{
	Q_OBJECT

	QSqlTableModel* ownerModel;
	QSqlTableModel* lastPostModel;
    QNetworkCookieJar* jar;
public:
	explicit Settings(QWidget *parent = 0);
	~Settings();
    void setCookie(QNetworkCookieJar*jar);
private slots:
    void on_insertOwner_clicked();
    void on_updateOwner_clicked();
	void on_deleteOwner_clicked();
	void on_ownerCancelButton_clicked();
	void on_ownerOKButton_clicked();
    void on_viewImageButton_clicked();

private:
	Ui::Settings *ui;
};

#endif // SETTINGS_H
