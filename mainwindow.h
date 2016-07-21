#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QtWebKit>
#include <QtWebKitWidgets>
#include <QWebView>
#include <QtSql>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QMessageBox>
#include "webpageparser.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	QSqlDatabase db;
	QSystemTrayIcon* trIcon;	
	QString accessToken;
	QUrl BaseURL;
	size_t lastAlbum;
	QList<WebPageParser*> deleteWParser;
	size_t publishTime = 0;
	size_t timeEnd;
	size_t timeInterval;
	bool isPostpone = false;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void recieveAlbums(QString);
	void recieveImages(QString);
	void recievePostID(QString);
	void recieveMovImages(QString);
	void on_autoPostButton_clicked();
private slots:
	void creatNewDB();
	void on_webView_urlChanged(const QUrl &arg1);
	void on_movButton_clicked();	
	void on_exit_clicked();
	void on_settings_clicked();
	void on_postponeButton_clicked();
	void  postponePost();
signals:
	void sendCurrentImage(QString pid, QString mediaURL);

private:
	Ui::MainWindow *ui;
	void deleteWebParse();
};

#endif // MAINWINDOW_H
