#ifndef WEBPAGEPARSER
#define WEBPAGEPARSER
#include <QObject>
#include <QtWebKit>
#include <QtWebKitWidgets>
#include <QDebug>
class WebPageParser : public QObject

{
	Q_OBJECT

public:
	WebPageParser(const QUrl &url); // конструктор, получающий в качестве единственного аргумента ссылку на страницу.

public slots:
	void readHTML(); // Функция, извлекающая html код страницы по окончании загрузки

private slots:
	void loadProgress(int progress);  // Функция, получающая текущее состояние загрузки (от 0 до 100)

private:
	QWebPage page; // Наша страница %)

signals:
	QString sendData(QString data);
};
#endif // WEBPAGEPARSER

