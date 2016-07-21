#include "viewCurrentPost.h"
#include "ui_viewCurrentPost.h"

viewCurrentPost::viewCurrentPost(QWidget *parent) :
QDialog(parent),
ui(new Ui::viewCurrentPost)
{
	ui->setupUi(this);
}

viewCurrentPost::~viewCurrentPost()
{
	delete ui;
}

QString viewCurrentPost::getMassage()
{
	return ui->messageEdit->toPlainText();
}

QString viewCurrentPost::getSourceURL()
{
	return ui->sourceURLEdit->text();
}

void viewCurrentPost::recieveCurrentImage(QString pid, QString mediaURL)
{
	//get MEDIA
	ui->webView->setUrl(mediaURL);
	this->pid = pid;
}
void viewCurrentPost::on_toBlackListButton_clicked()
{
	//Сохранение pid последнего опубликованного изображения
	QSqlQuery q;
	q.exec("INSERT INTO medialacklist(media) VALUES('"+pid+"');");
	this->reject();
}
