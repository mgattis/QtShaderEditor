#ifndef VPREFERENCES_H
#define VPREFERENCES_H

#include <QCheckBox>
#include <QCoreApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QTextEdit>
#include <QVBoxLayout>

class VPreferences : public QDialog
{
	Q_OBJECT

public:
	VPreferences( QWidget *parent = NULL );
	~VPreferences();

protected:
	QVBoxLayout *windowLayout;
		QGroupBox *generalBox;
			QVBoxLayout *generalLayout;
				QCheckBox *loadLastProjectOnStart;
				QCheckBox *refreshSceneOnSave;
		QGroupBox *logBox;
			QVBoxLayout *logLayout;
				QCheckBox *logToFile;
				QCheckBox *clearLogFileOnStart;
				QCheckBox *alwaysScrollToEndOnAppend;
		QDialogButtonBox *buttonBox;

protected slots:
	void wasAccepted( void );
};

#endif // VPREFERENCES_H
