#ifndef QTSE_H
#define QTSE_H

#include "VGLView.h"
#include "VTabEditor.h"

#include <QAction>
#include <QSplitter>
#include <QHeaderView>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMainWindow>
#include <QMenuBar>
#include <QTextEdit>
#include <QWidget>
#include <QMdiArea>
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

	QMenu *menuTest;
		QAction *actionTest;

	QMenu *menuHelp;
		QAction *actionAbout;

	QSplitter *windowSplitter;
		QSplitter *viewSplitter;
			VGLView *viewWidget;
			QTabWidget *itemsTab;
				QTreeWidget *projectTree;
		//QMdiArea *editorArea;
		QTabWidget *initialTab;
			//QList< QTextEdit* > textEdits;
};

#endif // QTSE_H
