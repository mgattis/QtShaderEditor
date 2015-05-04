#ifndef VNEWPROJECTDIALOG_H
#define VNEWPROJECTDIALOG_H

#include <QCheckBox>
#include <QCoreApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QTextEdit>
#include <QVBoxLayout>

class VNewProjectDialog : public QDialog
{
	Q_OBJECT

public:
	VNewProjectDialog( QWidget *parent = NULL );
	~VNewProjectDialog();

protected:
	QVBoxLayout *windowLayout;
		QGroupBox *generalBox;
			QGridLayout *generalLayout;
				QLabel *projectNameLabel;
				QLineEdit *projectNameEdit;
				QLabel *projectPathLabel;
				QLineEdit *projectPathEdit;
				QPushButton *projectPathButton;
		QDialogButtonBox *buttonBox;

protected slots:
	//void wasAccepted( void );
};

#endif // VNEWPROJECTDIALOG_H
