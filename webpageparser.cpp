#include "webpageparser.h"
WebPageParser::WebPageParser(const QUrl &url)
{
	page.mainFrame()->load(url); // url - адрес страницы, после вызова этого метода начинается загрузка страницы
	connect(&page, SIGNAL(loadFinished(bool)), this, SLOT(readHTML())); // сигнал loadFinished вызывается по окончании загрузки интернет страницы
	connect(&page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int))); // сигнал loadProgress вызывается при каждом изменении состоянии загрузки
}

void WebPageParser::loadProgress(int progress)
{
	qDebug() << "progress = " << progress; // выводим текущее состояние загрузки в qDebug()
}

void WebPageParser::readHTML()
{
	emit sendData(page.mainFrame()->toPlainText());
}
