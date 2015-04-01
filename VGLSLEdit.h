#ifndef VGLSLEDIT_H
#define VGLSLEDIT_H

#include "qtil.h"

#include <QAction>
#include <QGridLayout>
#include <QMenuBar>
#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QWidget>

class CGLSLHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	CGLSLHighlighter( QTextDocument *parent = NULL );

protected:
	void highlightBlock( const QString &text );

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
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

class VGLSLEdit : public QWidget
{
	Q_OBJECT

public:
	explicit VGLSLEdit( QWidget *parent = NULL );
	~VGLSLEdit();

protected:
	QGridLayout *windowLayout;
		QMenuBar *menuBar;
			QMenu *menuFile;
				QAction *actionClose;
		QTextEdit *textEdit;

public:
	void setFile( const QString &path );

protected:
	CGLSLHighlighter *highlighter;

signals:

public slots:
};

#endif // VGLSLEDIT_H
