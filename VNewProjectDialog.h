#ifndef VNEWPROJECTDIALOG_H
#define VNEWPROJECTDIALOG_H

#include <QApplication>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

class VNewProjectDialog : public QDialog
{
	Q_OBJECT

public:
	VNewProjectDialog( QWidget *parent = NULL );
	~VNewProjectDialog();

public:
	QString getProjectPath( void );

protected:
	QVBoxLayout *windowLayout;
		QGroupBox *infoBox;
			QGridLayout *infoLayout;
				QLabel *projectNameLabel;
				QLineEdit *projectNameEdit;
				QLabel *projectPathLabel;
				QLineEdit *projectPathEdit;
				QPushButton *projectPathButton;
				QCheckBox *projectDefaultPathCheck;
		QDialogButtonBox *buttonBox;

protected slots:
	void browse( void );
	void wasAccepted( void );
};

#endif // VNEWPROJECTDIALOG_H
