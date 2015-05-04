#ifndef QTSE_H
#define QTSE_H

#include "CPath.h"
#include "StdRedirector.h"
#include "VDraggableTabBar.h"
#include "VGLSLEdit.h"
#include "VGLView.h"
#include "VJsonForm.h"
#include "VLogger.h"
#include "VNewProjectDialog.h"
#include "VPreferences.h"
#include "VTabWidgetArea.h"

#include <QAction>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QIcon>
#include <QInputDialog>
#include <QFileSystemModel>
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
#include <QSettings>

class QtSE : public QMainWindow
{
    Q_OBJECT

public:
#if 1
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
#endif

public:
	QtSE( QWidget *parent = NULL );
    ~QtSE();

protected:
	void closeEvent( QCloseEvent *event );

protected:
	QMenu *menuFile;
		QAction *actionNew;
		QAction *actionOpen;
		QMenu *menuRecentProjects;
		QAction *actionSave;
		QAction *actionSaveAll;
		QAction *actionCloseProject;
		QAction *actionQuit;

	QMenu *menuView;
		QAction *actionReloadProject;
		QAction *actionSplitHorizontally;
		QAction *actionSplitVertically;
		QAction *actionSplitCollapse;

	QMenu *menuSettings;
		QAction *actionPreferences;

	QMenu *menuHelp;
		QAction *actionAbout;
		QAction *actionAboutQt;

	QSplitter *windowSplitter;
		QSplitter *viewSplitter;
			VGLView *viewWidget;
			QTabWidget *itemsTab;
				QTreeWidget *projectTree;
				QTreeView *fsProjectTree;
					QFileSystemModel *fsModel;
		QSplitter *editSplitter;
			VTabWidgetArea *tabArea;
			VLogger *logger;

protected:
	QString jsonProjectName;
	QMap< QString , QWidget* > openFiles;
	//CProjectTreeItem *activeProjectItem;
	QFileInfo fsContextInfo;

	StdRedirector<> *redirector;

protected:
	//void addArrayToSave( QJsonArray &array , const VJsonFormItem *item );
	void open( const QString path );
	bool save( QWidget *widget ); // Returns true if the file was modified, not if it was saved

	void updateRecentProjects( const QString &path = QString() , bool remove = false );
	bool makeRelativePath( QString &path );

protected slots:
	void newProject( void );
	void open( void );
	void openRecent( void );
	void save( void );
	void saveAll( void );
	void closeProject( void );

	void reloadProject( void );

	void preferences( void );

	void about( void );

	// Managed widget slots
	
	void projectTreeContextMenu( QPoint point );
	void itemsTabTreeContextMenu( QPoint point );

	void fsProjectTreeContextMenu( QPoint point );
	void fsProjectTreeItemClicked( QModelIndex index );
	//void fsProjectTreeItemDoubleClicked( QTreeWidgetItem *item , int column );

	inline void split( Qt::Orientation orientation ) { tabArea->split( orientation ); }
	inline void splitHorizontally( void ) { tabArea->split( Qt::Vertical ); }
	inline void splitVertically( void ) { tabArea->split( Qt::Horizontal ); }
	inline void splitCollapse( void ) { tabArea->splitCollapse(); }

	void tabWidgetDeleted( QWidget *widget );

	// Managed file slots

	void loadProject( const QString &path = QString() );
	//void generateProjectTree( const QString &path , QTreeWidgetItem *dirItem );

	void addProjectFile( QFileInfo &info , const QString &itemName , const QString &typeName );
	void addFolder( void );
	void addStage( void );
	void addFramebuffer( void );
	void addShader( void );
	void addModel( void );
	void addTexture( void );

	void renameItem( void );
	void deleteItem( void );
};

#endif // QTSE_H
