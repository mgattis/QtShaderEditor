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
	VGLSLEdit( const QString &initialData = QString() , QWidget *parent = NULL );
	~VGLSLEdit();

public:
	void load( const QString &path );
	void save( void );

protected:
	QString filePath;
	CGLSLHighlighter *highlighter;

signals:

public slots:
	inline void setModified( void ) { setModified( true ); }
	inline void setUnmodified( void ) { setModified( false ); }
	void setModified( bool modified ) { if( modified != this->isWindowModified() ) this->setWindowModified( modified ); }
};

#endif // VGLSLEDIT_H
