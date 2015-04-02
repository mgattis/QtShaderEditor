#ifndef QTSE_H
#define QTSE_H

#include "CPath.h"
#include "StdRedirector.h"
#include "VDraggableTabBar.h"
#include "VGLSLEdit.h"
#include "VGLView.h"
#include "VJsonForm.h"
#include "VLogger.h"
#include "VTabWidgetArea.h"

#include <QAction>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QInputDialog>

#include <QStack>
#include <QSplitter>
#include <QHeaderView>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMainWindow>
#include <QMenuBar>
#include <QMutex>
#include <QTextEdit>
#include <QListWidget>
#include <QWidget>
#include <QMdiArea>
#include <QTextEdit>
#include <QList>
#include <QScrollBar>

static QTextEdit *g_coutEdit;
static QListWidget *g_coutList;

class QtSE : public QMainWindow
{
    Q_OBJECT

public:
	class CProjectTreeItem : public QTreeWidgetItem
	{
	public:
		enum Type
		{
			invalid = 0,
			dir,
			stage,
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

		inline bool isDir( void ) const { return partType == dir; }
		inline bool isStage( void ) const { return partType == stage; }
		inline bool isFramebuffer( void ) const { return partType == framebuffer; }
		inline bool isShader( void ) const { return partType == shader; }
		inline bool isTexture( void ) const { return partType == texture; }
		inline bool isModel( void ) const { return partType == model; }

		void getRelativePath( QString &relativePath ) const;
		QString getFullPath( const QString &basePath ) const;
		
	protected:
		Type partType;
	};

public:
	QtSE( QWidget *parent = NULL );
    ~QtSE();

protected:
	QMenu *menuFile;
		QAction *actionLoad;
		QAction *actionSave;
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
				QTreeWidget *fsProjectTree;
		QSplitter *editSplitter;
			VTabWidgetArea *tabArea;
			VLogger *coutEdit;

protected:
	CPath jsonProjectName;
	QMap< QString , QWidget* > openFiles;
	CProjectTreeItem *activeProjectItem;

	StdRedirector<> *redirector;

protected:
	VJsonForm* makeVJsonForm( void );

	void addArrayToSave( QJsonArray &array , const VJsonFormItem *item );

protected slots:
	void open( void );
	void save( void );

	void about( void );

	// Managed widget slots
	
	void projectTreeContextMenu( QPoint point );
	void fsProjectTreeContextMenu( QPoint point );
	void itemsTabTreeContextMenu( QPoint point );

	void fsProjectTreeItemDoubleClicked( QTreeWidgetItem *item , int column );

	inline void split( Qt::Orientation orientation ) { tabArea->split( orientation ); }
	inline void splitHorizontally( void ) { tabArea->split( Qt::Vertical ); }
	inline void splitVertically( void ) { tabArea->split( Qt::Horizontal ); }
	inline void splitCollapse( void ) { tabArea->splitCollapse(); }

	void tabWidgetDeleted( QWidget *widget );

	//void tabWidgetTabAdded( QWidget *widget );
	//void tabWidgetTabDestroyed( QObject *object );
	//void tabWidgetCountChanged( int count );
	//void tabCloseRequested( int index );
	//void removeTabWidgetFromLayout( QObject *tabWidget ) { removeTabWidgetFromLayout( dynamic_cast< VDraggableTabWidget* >( tabWidget ) ); }
	//void removeTabWidgetFromLayout( QObject *tabWidget ) { removeTabWidgetFromLayout( (VDraggableTabWidget*)tabWidget ); }
	//void removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget );

	void fsRefresh( void );

	// Managed file slots

	void loadProject( const QString &path = QString() );
	void generateProjectTree( const QString &path , QTreeWidgetItem *dirItem );

	void addFolder( void );
	void addStage( void );
	void addFramebuffer( void );
	void addShader( void );
	void addModel( void );
	void addTexture( void );
	void deleteItem( void );
	bool deleteItem( CProjectTreeItem *curItem /* = NULL */ );
};

#endif // QTSE_H
