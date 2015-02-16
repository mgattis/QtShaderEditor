#ifndef QTSE_H
#define QTSE_H

#include "VDraggableTabBar.h"
#include "VGLView.h"
#include "VJsonForm.h"
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
	class CProjectTreeItem : public QTreeWidgetItem
	{
	public:
		enum Type
		{
			stage = 0,
			framebuffer,
			shader,
			texture,
			model
		};

	public:
		CProjectTreeItem( Type type , QTreeWidget *view , const QStringList &strings ) : QTreeWidgetItem( view , strings )
		{
			partType = type;
		}

		CProjectTreeItem( Type type , QTreeWidgetItem *item , const QStringList &strings ) : QTreeWidgetItem( item , strings )
		{
			partType = type;
		}

	public:
		inline Type getPartType( void ) const { return partType; }
		inline void setPartType( Type type ) { partType = type; }

	protected:
		Type partType;
	};

public:
	QtSE( QWidget *parent = NULL );
    ~QtSE();

protected:
	QMenu *menuFile;
		QAction *actionQuit;

	QMenu *menuView;
		QAction *actionSplitHorizontally;
		QAction *actionSplitVertically;
		QAction *actionSplitCollapse;

	QMenu *menuHelp;
		QAction *actionAbout;

	QSplitter *windowSplitter;
		QSplitter *viewSplitter;
			VGLView *viewWidget;
			QTabWidget *itemsTab;
				QTreeWidget *projectTree;
		QMap< VJsonForm* , VDraggableTabWidget* > tabMap;

protected:
	VJsonForm* makeVJsonForm( void );
	VDraggableTabWidget* makeVDraggableTabWidget( void );

	VDraggableTabWidget* getFirstTabWidget( QSplitter *splitter ) const;

protected slots:
	void treeContextMenu( QPoint point );
	void split( Qt::Orientation orientation );
	inline void splitHorizontally( void ) { split( Qt::Vertical ); }
	inline void splitVertically( void ) { split( Qt::Horizontal ); }
	void splitCollapse( void );

	void tabWidgetTabAdded( QWidget *widget );
	void tabWidgetTabDestroyed( QObject *object );
	void tabWidgetCountChanged( int count );
	void removeTabWidgetFromLayout( QObject *tabWidget ) { removeTabWidgetFromLayout( dynamic_cast< VDraggableTabWidget* >( tabWidget ) ); }
	void removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget );

	void addStage( void );
	void addFramebuffer( void );
	void addShader( void );
	void addModel( void );
	void addTexture( void );
};

#endif // QTSE_H
