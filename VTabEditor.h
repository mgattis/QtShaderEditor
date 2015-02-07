#ifndef VTABEDITOR_H
#define VTABEDITOR_H

#include <QTextEdit>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>

class VTabEditor : public QWidget
{
public:
	VTabEditor( QWidget *parent = NULL );
	~VTabEditor();

public:
	QGridLayout *windowLayout;
		QTabBar *tabBar;
		QHBoxLayout *buttonLayout;
			QPushButton *buttonSplit;
			QPushButton *buttonCloseSplit;
			QPushButton *buttonClose;
		QStackedWidget *stackedWidget;

public:
	void addTab( QWindow *window );
};

#endif // VTABEDITOR_H
