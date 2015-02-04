#ifndef QTSE_H
#define QTSE_H

#include <QAction>
#include <QSplitter>
#include <QHeaderView>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMainWindow>
#include <QMenuBar>
#include <QWidget>
#include <QTextEdit>
#include <QList>

class QtSE : public QMainWindow
{
    Q_OBJECT

public:
	QtSE( QWidget *parent = NULL );
    ~QtSE();

protected:
	QMenu *menuFile;
		QAction *actionQuit;

	QMenu *menuHelp;
		QAction *actionAbout;

	QSplitter *windowSplitter;
		QTabWidget *itemsTab;
			QTreeWidget *projectTree;
		QTabWidget *textTab;
			QList< QTextEdit* > textEdits;
};

#endif // QTSE_H
