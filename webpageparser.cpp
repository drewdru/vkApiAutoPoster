#include "webpageparser.h"
WebPageParser::WebPageParser(const QUrl &url)
{
	page.mainFrame()->load(url);
	connect(&page, SIGNAL(loadFinished(bool)), this, SLOT(readHTML()));
	connect(&page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
}

void WebPageParser::loadProgress(int progress)
{
	qDebug() << "progress = " << progress;
}

void WebPageParser::readHTML()
{
	emit sendData(page.mainFrame()->toPlainText());
}
