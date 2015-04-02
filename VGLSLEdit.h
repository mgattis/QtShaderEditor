#ifndef VGLSLEDIT_H
#define VGLSLEDIT_H

#include "log.h"
#include "qtil.h"

#include <QAction>
#include <QFileDialog>
#include <QGridLayout>
#include <QMenuBar>
#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QWidget>

class CGLSLHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

public:
	CGLSLHighlighter( QTextDocument *parent = NULL );

protected:
	void highlightBlock( const QString &text );

private:
	QVector< HighlightingRule > highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat directiveFormat;
	QTextCharFormat extensionBehaviourFormat;
	QTextCharFormat macroFormat;

	QTextCharFormat reservedNameFormat;
	QTextCharFormat keywordFormat;
	QTextCharFormat typeFormat;

	// Official order: invariant-qualifier
	// interpolation-qualifier layout-qualifier
	// other-storage-qualifier precision-qualifier

	QTextCharFormat interpolationQualifiersFormat;
	QTextCharFormat storageQualifiersFormat;
	QTextCharFormat memoryQualifiersFormat;
	QTextCharFormat memoryLayoutQualifiersFormat;
	QTextCharFormat precisionFormat;

	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat numericalFormat;
	QTextCharFormat functionFormat;
};

class VGLSLEdit : public QTextEdit
{
	Q_OBJECT

public:
	VGLSLEdit( QWidget *parent = NULL );
	~VGLSLEdit();

protected:
	QGridLayout *windowLayout;
		QMenuBar *menuBar;
			QMenu *menuFile;
				QAction *actionClose;
		QTextEdit *textEdit;

public:
	void setFile( const QString &path );
	void save( void );

protected:
	QString filePath;
	CGLSLHighlighter *highlighter;

signals:

public slots:
	void dummy( void ) { if( !this->isWindowModified() ) this->setWindowModified( true ); }
};

#endif // VGLSLEDIT_H
