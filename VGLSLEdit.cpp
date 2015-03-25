#include "VGLSLEdit.h"

CGLSLHighlighter::CGLSLHighlighter( QTextDocument *parent ) : QSyntaxHighlighter( parent )
{
	HighlightingRule rule;

	// Directive

	directiveFormat.setForeground( Qt::blue );
	QStringList directivePatterns;
	directivePatterns << "\\b#version\\b" << "\\b#extension\\b" << "\\b#line\\b";

	foreach( const QString &pattern , directivePatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Extension Behaviour

	extensionBehaviourFormat.setForeground( Qt::blue );
	QStringList extensionBehaviourPatterns;
	extensionBehaviourPatterns << "\\benable\\b" << "\\brequire\\b" << "\\bwarn\\b"
		<< "\\bdisable\\b";

	foreach( const QString &pattern , extensionBehaviourPatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Macro

	macroFormat.setForeground( Qt::blue );
	QStringList macroPatterns;
	macroPatterns << "\\b__FILE__\\b" << "\\b__LINE__\\b" << "\\b__VERSION__\\b"
		<< "\\bGL_core_profile\\b" << "\\bGL_compatibility_profile\\b";

	foreach( const QString &pattern , macroPatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Reserved Names

	reservedNameFormat.setForeground( Qt::blue );
	QStringList reservedNamePatterns;
	reservedNamePatterns << "\\bgl_\S*\\b";

	foreach( const QString &pattern , reservedNamePatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Keyword

	keywordFormat.setForeground( Qt::yellow );
	//keywordFormat.setFontWeight( QFont::Bold );
	QStringList keywordPatterns;
	keywordPatterns << "\\bif\\b" << "\\belse\\b" << "\\bswitch\\b"
		<< "\\bcase\\b" << "\\bfor\\b" << "\\bwhile\\b"
		<< "\\bdo\\b" << "\\bbreak\\b" << "\\bcontinue\\b"
		<< "\\breturn\\b" << "\\bdiscard\\b" << "\\bcentroid\\b"
		<< "\\bconst\\b" << "\\binvariant\\b" << "\\battribute\\b"
		<< "\\bvarying\\b" << "\\bshared\\b" << "\\bsample\\b"
		<< "\\bbinding\\b" << "\\bprecision\\b";

	foreach( const QString &pattern , keywordPatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	// Type Format

	typeFormat.setForeground( Qt::yellow );
	QStringList typePatterns;
	typePatterns << "\\bvoid\\b" << "\\bbool\\b" << "\\bint\\b"
		<< "\\buint\\b" << "\\bfloat\\b" << "\\bdouble\\b"

		<< "\\b[biud]?vec[2-4]\\b" << "\\bmat[2-4](x[2-4])?\\b"

		<< "\\b[iu]?sampler[1-3]D\\b" << "\\b[iu]?samplerCube(Array)?\\b"
		<< "\\b[iu]?sampler2DRect\\b" << "\\b[iu]?sampler[12]DArray\\b"
		<< "\\b[iu]?samplerBuffer\\b" << "\\b[iu]?sampler2DMS(Array)?\\b"

		<< "\\bsampler[12]DShadow\\b" << "\\bsamplerCube(Array)?Shadow\\b"
		<< "\\bsampler2DRectShadow\\b" << "\\bsampler[12]DArrayShadow\\b"

		<< "\\b[iu]?image[1-3]D\\b" << "\\b[iu]?imageCube(Array)?\\b"
		<< "\\b[iu]?image2DRect\\b" << "\\b[iu]?image[12]DArray\\b"
		<< "\\b[iu]?imageBuffer\\b" << "\\b[iu]?image2DMS(Array)?\\b";

	foreach( const QString &pattern , typePatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Interpolation Qualifiers

	interpolationQualifiersFormat.setForeground( Qt::yellow );
	QStringList interpolationQualifiersPatterns;
	interpolationQualifiersPatterns << "\\bflat\\b" << "\\bnoperspective\\b" << "\\bsmooth\\b";

	foreach( const QString &pattern , interpolationQualifiersPatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Storage Qualifiers

	storageQualifiersFormat.setForeground( Qt::yellow );
	QStringList storageQualifiersFormat;
	storageQualifiersFormat << "\\bin\\b" << "\\bout\\b" << "\\buniform\\b" << "\\bbuffer\\b";

	foreach( const QString &pattern , storageQualifiersFormat )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Memory Qualifiers

	memoryQualifiersFormat.setForeground( Qt::yellow );
	QStringList memoryQualifiersPatterns;
	memoryQualifiersPatterns << "\\bcoherent\\b" << "\\bvolatile\\b" << "\\brestrict\\b"
		<< "\\breadonly\\b" << "\\bwriteonly\\b";

	foreach( const QString &pattern , memoryQualifiersPatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Memory Layout Qualifiers

	memoryLayoutQualifiersFormat.setForeground( Qt::yellow );
	QStringList memoryLayoutQualifiersPatterns;
	memoryLayoutQualifiersPatterns << "\\bshared\\b" << "\\bpacked\\b" << "\\bstd140\\b"
		<< "\\bstd430\\b";

	foreach( const QString &pattern , memoryLayoutQualifiersPatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// Precision

	precisionFormat.setForeground( Qt::blue );
	QStringList precisionPatterns;
	precisionPatterns << "\\blowp\\b" << "\\bmediump\\b" << "\\bhighp\\b";

	foreach( const QString &pattern , precisionPatterns )
	{
		rule.pattern = QRegExp( pattern );
		rule.format = typeFormat;
		highlightingRules.append( rule );
	}

	// First we define a keyword rule which recognizes the most common C++ keywords.
	// We give the keywordFormat a bold, dark blue font. For each keyword, we assign
	// the keyword and the specified format to a HighlightingRule object and append
	// the object to our list of rules.

	// Class

	/*
	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);
	*/

	// Quotation

	quotationFormat.setForeground( Qt::magenta );
	rule.pattern = QRegExp( "\".*\"" );
	rule.format = quotationFormat;
	highlightingRules.append( rule );

	// Numerical

	numericalFormat.setForeground( Qt::magenta );
	rule.pattern = QRegExp( "\\b[+-]?[0-9]*\\.?[0-9]+\\b" );
	rule.format = quotationFormat;
	highlightingRules.append( rule );

	// Functions

	/*
	functionFormat.setFontItalic(true);
	functionFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);
	*/

	singleLineCommentFormat.setForeground( Qt::green );
	rule.pattern = QRegExp( "//[^\n]*" );
	rule.format = singleLineCommentFormat;
	highlightingRules.append( rule );

	multiLineCommentFormat.setForeground( Qt::green );

	commentStartExpression = QRegExp( "/\\*" );
	commentEndExpression = QRegExp( "\\*/" );
}

void CGLSLHighlighter::highlightBlock( const QString &text )
{
	foreach( const HighlightingRule &rule , highlightingRules )
	{
		QRegExp expression( rule.pattern );
		int index = expression.indexIn( text );
		while( index >= 0 )
		{
			int length = expression.matchedLength();
			setFormat( index , length , rule.format );
			index = expression.indexIn( text , index + length );
		}
	}

	// Format multiline comments

	setCurrentBlockState( 0 );

	int startIndex = 0;
	if( previousBlockState() != 1 )
		startIndex = commentStartExpression.indexIn( text );

	while( startIndex >= 0 )
	{
		int endIndex = commentEndExpression.indexIn( text , startIndex );
		int commentLength;

		if( endIndex == -1 )
		{
			setCurrentBlockState( 1 );
			commentLength = text.length() - startIndex;
		}
		else
			commentLength = endIndex - startIndex + commentEndExpression.matchedLength();

		setFormat( startIndex , commentLength , multiLineCommentFormat );
		startIndex = commentStartExpression.indexIn( text , startIndex + commentLength );
	}
}

VGLSLEdit::VGLSLEdit( QWidget *parent ) : QWidget( parent )
{
	menuBar = new QMenuBar( NULL );
		menuFile = menuBar->addMenu( "File" );
			actionClose = menuFile->addAction( "Close" , this , SLOT(close()) );

	QFont font;
	font.setFamily( "monospace" );
	font.setFixedPitch( true );
	font.setPointSize( 9 );

	textEdit = new QTextEdit( NULL );
	textEdit->setFont( font );
	textEdit->setTabStopWidth( 4 * UTIL_getFontWidth( " " , font ) );
	//textEdit->setText( "#include <test>\n\n// test\n\nvoid thing;\n\nvoid main( int argc , char *argv[] )\n{\n/*\nThis is a long comment\n*/\nstd::cout << \"test\";\n}\n" );
	textEdit->setText( "in vec3 fromPrevious;\nin uvec2 fromRange;\n \nconst int foo = 5;\nconst uvec2 range = uvec2(2, 5);\nuniform vec2 pairs;\n \nuniform sampler2D tex;\n \nvoid main()\n{\n\tfoo; //constant expressions are dynamically uniform.\n \n\tuint value = 21; //'value' is dynamically uniform.\n\tvalue = range.x; //still dynamically uniform.\n\tvalue = range.y + fromRange.y; //not dynamically uniform; current contents come from a non-dynamically uniform source.\n\tvalue = 4; //dynamically uniform again.\n\tif(fromPrevious.y < 3.14)\n\t\tvalue = 12;\n\tvalue; //NOT dynamically uniform. Current contents depend on 'fromPrevious', an input variable.\n \n\tfloat number = abs(pairs.x); //'number' is dynamically uniform.\n\tnumber = sin(pairs.y); //still dynamically uniform.\n\tnumber = cos(fromPrevious.x); //not dynamically uniform.\n \n\tvec4 colors = texture(tex, pairs.xy); //dynamically uniform, even though it comes from a texture.\n\t\t\t\t\t\t\t\t\t\t//It uses the same texture coordinate, thus getting the same texel every time.\n\tcolors = texture(tex, fromPrevious.xy); //not dynamically uniform.\n \n\tfor(int i = range.x; i < range.y; ++i)\n\t{\n\t\t\t //loop initialized with, compared against, and incremented by dynamically uniform expressions.\n\t\ti; //Therefore, 'i' is dynamically uniform, even though it changes.\n\t}\n \n\tfor(int i = fromRange.x; i < fromRange.y; ++i)\n\t{\n\t\ti; //'i' is not dynamically uniform; 'fromRange' is not dynamically uniform.\n\t}\n}\n" );

	highlighter = new CGLSLHighlighter( textEdit->document() );

	windowLayout = new QGridLayout( NULL );
	//windowLayout->setSpacing( 0 );
	windowLayout->setMenuBar( menuBar );
	windowLayout->addWidget( textEdit , 0 , 0 , 1 , 1 );
	delete this->layout();
	this->setLayout( windowLayout );
	//this->setContentsMargins( 0 , 0 , 0 , 0 );
}

VGLSLEdit::~VGLSLEdit()
{
	// Nothing to do
}
