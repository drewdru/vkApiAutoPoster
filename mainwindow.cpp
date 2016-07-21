#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Settings.h"
#include "viewCurrentPost.h"

#include <QDebug>
#include <QMutex>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{	
	ui->setupUi(this);
	this->setWindowTitle("FantasyInPictures");
	this->setWindowIcon(QIcon(":/img/icon.png"));
	ui->tabWidget->hide();

	trIcon = new QSystemTrayIcon();
	trIcon->setIcon(QIcon(":/img/icon.png"));
	trIcon->setToolTip("Подождите до завершения");

	QFile file("./db.sqlite");
	bool dbClear = false;
	if(file.exists() || file.size() == 0 )
		dbClear = true;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("db.sqlite");
	if (!db.open()) {
		QMessageBox::information(0, "db.sqlite",\
			db.lastError().text());
		return;
	}
	if (dbClear)
		creatNewDB();

	QSqlQuery owners;
	while(true)
	{
		owners.exec("SELECT owner, name FROM owners");
		if(!owners.first())
		{
			QMessageBox::information(0, "БД пуста",\
				"Добавьте сообщество или страницу!");
			this->on_settings_clicked();
		}
		else
			break;
	}
	do
	{
		ui->postOwnerId->addItem(owners.value("name").toString(),\
			owners.value("owner").toString());
		ui->movOwnerID->addItem(owners.value("name").toString(),\
			owners.value("owner").toString());
        ui->postponeOwnerId->addItem(owners.value("name").toString(),\
            owners.value("owner").toString());
	}
	while(owners.next());
    connect(ui->postOwnerId,SIGNAL(currentIndexChanged(int)),ui->movOwnerID,SLOT(setCurrentIndex(int)));
    connect(ui->postOwnerId,SIGNAL(currentIndexChanged(int)),ui->postponeOwnerId,SLOT(setCurrentIndex(int)));
    connect(ui->movOwnerID,SIGNAL(currentIndexChanged(int)),ui->postponeOwnerId,SLOT(setCurrentIndex(int)));
    connect(ui->movOwnerID,SIGNAL(currentIndexChanged(int)),ui->postOwnerId,SLOT(setCurrentIndex(int)));
    connect(ui->postponeOwnerId,SIGNAL(currentIndexChanged(int)),ui->postOwnerId,SLOT(setCurrentIndex(int)));
    connect(ui->postponeOwnerId,SIGNAL(currentIndexChanged(int)),ui->movOwnerID,SLOT(setCurrentIndex(int)));
    //Запрос доступа
	BaseURL.setUrl("https://oauth.vk.com/authorize?"
        "client_id=4916113&"
        "scope=photos,wall,groups,offline&"
        "redirect_uri=https://oauth.vk.com/blank.html&"
        "display=mobile&"
        "v=5.31&"
        "response_type=token");
	ui->webView->setUrl(BaseURL);

	ui->postponeStart->setDateTime(QDateTime::currentDateTime());
	ui->postponeStart->setDisplayFormat("dd.MM.yyyy hh:mm");
	ui->postponeEnd->setDateTime(QDateTime::fromTime_t(ui->postponeStart->dateTime().toTime_t()+3600));
	ui->postponeEnd->setDisplayFormat("dd.MM.yyyy hh:mm");
}

MainWindow::~MainWindow()
{
	deleteWebParse();
	delete ui;
}

void MainWindow::on_exit_clicked()
{
	this->close();
}

void MainWindow::on_webView_urlChanged(const QUrl &arg1)
{
	//Если пользователь залогинен
	if (arg1.url().split("?")[0].contains("blank.html"))
	{
		ui->webView->hide();
		ui->tabWidget->show();
		deleteWebParse();
	}
}

void MainWindow::on_settings_clicked()
{
	Settings a;
    //a.setWebViewCookie(ui->webView->;);
    /*QUrl vk;
    vk.setUrl("vk.com");
    view = new QWebView(this);
    jar = new QNetworkCookieJar;
    view->page()->networkAccessManager()->setCookieJar(jar);*/
    qDebug()<<ui->webView->page()->networkAccessManager()->cookieJar();
    a.setCookie(ui->webView->page()->networkAccessManager()->cookieJar());
	a.exec();
}

void MainWindow::deleteWebParse()
{
	for (int i = 0; i < deleteWParser.size(); ++i)
		delete deleteWParser.at(i);
	deleteWParser.clear();
}

void MainWindow::on_autoPostButton_clicked()
{
	//this->hide();
	trIcon->show();
	trIcon->showMessage("Автопост",\
		"Подождите до завершения");
	//Получение access токена
	QUrl url = ui->webView->url();
	accessToken = url.toString().split("#access_token=")[1]\
		.split("&")[0];
	//Получение списка альбомов
	url.setUrl("https://api.vk.com/method/photos.getAlbums?version=5.35"
		"&owner_id="+ui->postOwnerId->currentData().toString()\
		+"&access_token="+accessToken);

	WebPageParser* webParser = new WebPageParser(url);
	deleteWParser.append(webParser);
	connect(webParser, SIGNAL(sendData(QString)), this,SLOT(recieveAlbums(QString)));
	//webParser->deleteLater();
}

void MainWindow::recieveAlbums(QString str)
{
	//получение чёрного списка альбомов
	QList<size_t> BlackListAid;
	QSqlQuery q;
	q.exec("SELECT aid FROM blacklist;");
	while(q.next())
		BlackListAid.append(q.value("aid").toInt());
	q.exec("SELECT aid FROM lockedalbums;");
	while(q.next())
		BlackListAid.append(q.value("aid").toInt());

	//Получение списка альбомов для выборки изображений
	QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();
	QJsonArray jsonArray = jsonObject["response"].toArray();
	QList<size_t> aidList;
	foreach (const QJsonValue & value, jsonArray)
	{
		QJsonObject obj = value.toObject();
		if(!BlackListAid.contains(obj["aid"].toInt()))
			aidList.append(obj["aid"].toInt());
	}
	//Получение списка изображений
	lastAlbum = aidList.at(qrand() % aidList.size());
	QUrl url;
	url.setUrl("https://api.vk.com/method/photos.get?version=5.35&"
        "owner_id="+ui->postOwnerId->currentData().toString()\
		+"&album_id="+QString::number(lastAlbum)\
		+"&access_token="+accessToken);

	WebPageParser* webParser = new WebPageParser(url);
	deleteWParser.append(webParser);
	connect(webParser, SIGNAL(sendData(QString)), this,SLOT(recieveImages(QString)));
	//webParser->deleteLater();
}

void MainWindow::recieveImages(QString str)
{
	//Получение списка изображений, которые уже публиковались
	QStringList LastPid;
	QSqlQuery q;
    q.exec("SELECT media FROM lastmedia;");
	while(q.next())
		LastPid.append(q.value("media").toString());

	//Получение изображений, которые могут быть опубликованы
	QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();
	QJsonArray jsonArray = jsonObject["response"].toArray();
	QStringList imagesList;
	QStringList imagesURLList;
	foreach (const QJsonValue & value, jsonArray)
	{		
		QJsonObject obj = value.toObject();
		QString testStr = "photo" +\
			ui->postOwnerId->currentData().toString() +\
			"_" + QString::number(obj["pid"].toInt());
		QString testImage = obj["src_big"].toString();
		if(!LastPid.contains(testStr))
		{
			imagesList.append(testStr);
			imagesURLList.append(testImage);
		}
	}
	//Если в альбоме не осталось не опубликованных изображений
	if (imagesList.length() == 0)
	{
		q.exec("INSERT INTO lockedalbums(aid) VALUES('"+QString::number(lastAlbum)+"');");
		this->trIcon->hide();
		on_autoPostButton_clicked();
		return;
	}

	size_t count = qrand() % imagesList.size();
	QString pid = imagesList.at(count);
	QString mediaURL = imagesURLList.at(count);

	viewCurrentPost showPost;
	showPost.show();
	connect(this,SIGNAL(sendCurrentImage(QString,QString)),&showPost,SLOT(recieveCurrentImage(QString,QString)));
	emit sendCurrentImage(pid,mediaURL);
	if(showPost.exec() == QDialog::Rejected)
	{
		this->trIcon->hide();
		qApp->processEvents();
		on_autoPostButton_clicked();
		return;
	}
	//Сохранение pid последнего опубликованного изображения
    q.exec("INSERT INTO lastmedia(media,ownerid) VALUES('"+pid\
        +"','"+ui->postOwnerId->currentData().toString()+"');");
	//опубликовать запись
	///publish_date	дата публикации записи в формате unixtime. Если параметр указан, публикация записи будет отложена до указанного времени.
	QUrl url;
	url.setUrl("https://api.vk.com/method/wall.post?owner_id="\
		+ui->postOwnerId->currentData().toString()\
		+"&from_group=1"
		"&message="+showPost.getMassage()\
		+"&attachments="+pid\
		+","+showPost.getSourceURL()\
		+"&publish_date="+QString::number(publishTime)\
		+"&access_token="+accessToken);
	qDebug()<<url;
	WebPageParser* webParser = new WebPageParser(url);
	deleteWParser.append(webParser);
	connect(webParser, SIGNAL(sendData(QString)), this,SLOT(recievePostID(QString)));
}

void MainWindow::recievePostID(QString str)
{
	//ui->webView->setUrl(BaseURL);
	qDebug()<<str;
	this->trIcon->hide();
	if(isPostpone)
	{
		this->trIcon->hide();
		qApp->processEvents();
		emit postponePost();
		return;
	}
	///TODO: получение id последней публикации
}

void MainWindow::on_movButton_clicked()
{
	//свернуть в трей
	this->hide();
	trIcon->show();  //отображаем объект
	trIcon->showMessage("Перемещение","Подождите до завершения перемещения");
	//получение access токена
	QUrl url = ui->webView->url();
	accessToken = url.toString().split("#access_token=")[1]\
		.split("&")[0];
	//получение списка изображений, которые требуется переместить
    url.setUrl("https://api.vk.com/method/photos.get?owner_id="\
        +ui->movOwnerID->currentData().toString()\
        +"&album_id="+ui->movAlbumID->text()\
        +"&version=5.35&access_token="+accessToken);

	WebPageParser* webParser = new WebPageParser(url);
	deleteWParser.append(webParser);
	connect(webParser, SIGNAL(sendData(QString)), this,SLOT(recieveMovImages(QString)));
}

void MainWindow::recieveMovImages(QString str)
{
	//перемещение изображений
	QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();
	QJsonArray jsonArray = jsonObject["response"].toArray();
	foreach (const QJsonValue & pidValue, jsonArray)
	{
		QJsonObject obj = pidValue.toObject();
		//перемещение текущего изображения
		QString q = "https://api.vk.com/method/photos.move?";
		q.append("owner_id="+ui->movOwnerID->currentData().toString()\
			+"&target_album_id="+ui->target_album_idEdit->text()\
			+"&photo_id="+QString::number(obj["pid"].toInt())\
			+"&access_token="+accessToken);
		QUrl url;
		url.setUrl(q);
		//задержка на 6 секунд (требование к частоте запросов)
		QMutex mutex;
		mutex.lock();
		mutex.tryLock(6000);
		mutex.unlock();
	}
	//перемещение завершено
	ui->webView->setUrl(BaseURL);
	this->showMinimized();
	this->trIcon->hide();
	QApplication::beep();
	QApplication::alert(this);
}

void MainWindow::on_postponeButton_clicked()
{
	isPostpone = true;
	publishTime = ui->postponeStart->dateTime().toTime_t();
	timeEnd = ui->postponeEnd->dateTime().toTime_t();
	timeInterval = 3600/ui->numberPerHour->text().toInt();
	emit postponePost();
}

void MainWindow::postponePost()
{
	if(publishTime<timeEnd)
	{
		on_autoPostButton_clicked();
		publishTime+=timeInterval;
	}
	else
	{
		isPostpone = false;
	}
}
