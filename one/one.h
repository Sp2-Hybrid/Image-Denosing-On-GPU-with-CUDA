#ifndef ONE_H
#define ONE_H

#include <QtWidgets/QMainWindow>
#include "ui_one.h"
#include "test.h"


#include <QFileDialog>

class one : public QMainWindow
{
	Q_OBJECT

public:
	one(QWidget *parent = 0);
	~one();
private slots:
	void on_btn_inp_clicked();
	void on_btn_cla_clicked();
	void on_btn_cla_2_clicked();
	void on_btn_cla_3_clicked();
	void on_btn_cla_4_clicked();
	void on_btn_cla_5_clicked();
	void on_btn_cla_6_clicked();
	void on_btn_cla_7_clicked();
	void on_btn_cla_8_clicked();
	void button_init();
private:
	Ui::oneClass ui;
	QImage img;
	QString img_name;
	int mark;
};

#endif // ONE_H
