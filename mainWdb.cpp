#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::creatNewDB()
{
	QSqlQuery q;
	q.exec("CREATE TABLE blacklist("
		"id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"aid integer);");
	q.exec("CREATE TABLE medialacklist("
		"id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"media text);");
	q.exec("CREATE TABLE lockedalbums("
		"id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"aid integer);");
	q.exec("CREATE TABLE owners("
		"id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
		"owner integer, "
		"name text);");
    q.exec("CREATE TABLE lastmedia("
		"id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
        "media text,"
        "ownerid integer, "
		"FOREIGN KEY(ownerid) REFERENCES owners(owner)"
		");");
    q.exec("CREATE TABLE lastpost("
        "id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
        "postid text,"
        "ownerid integer, "
        "FOREIGN KEY(ownerid) REFERENCES owners(owner)"
        ");");
}
