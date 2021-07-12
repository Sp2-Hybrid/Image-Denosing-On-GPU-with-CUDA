#include "one.h"
extern "C" float Test_GPU(string name,int choice);

one::one(QWidget *parent)
	: QMainWindow(parent)
	
{
	ui.setupUi(this);
	img_name="";
	mark=0;
	button_init();
}

one::~one()
{
	
}

void one::button_init(){
	this->ui.btn_inp->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);font :9pt}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla_2->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla_3->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla_4->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla_5->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla_6->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla_8->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
	this->ui.btn_cla_7->setStyleSheet("QPushButton{border-color: rgb(255, 255, 255);}"
		"QPushButton:hover{font:11pt;color: rgb(0,0,255);}"
		"QPushButton:pressed{font:9pt;padding-left:3px;padding-top:3px;}");
}
//点击输入图片
void one::on_btn_inp_clicked(){
	this->img_name="";
	this->img_name=QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
	string name=img_name.toStdString();
	Mat inpImage=imread(name);
	cvtColor(inpImage, inpImage, CV_BGR2GRAY);
	imshow("Original Image",inpImage);

	//string tmpname=name;
	//int i;
	//for(i=0;i<tmpname.length();i++){
	//	if(tmpname[i]=='.'){
	//		break;
	//	}
	//}
	//tmpname.insert(i-1,"original");
	//imwrite(tmpname,inpImage);
	this->mark=1;
	
}
//点击经典核回归去噪CPU
void one::on_btn_cla_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	int time=Test_CPU(name);
	QString tmp=QString::number(time);
	tmp +="ms";
	this->ui.lbl_deb->setText(tmp);
	
}
//点击经典核回归去噪GPU
void one::on_btn_cla_2_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb_2->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	int time =Test_GPU(name,1);
	QString tmp=QString::number(time);
	tmp+="ms";
	this->ui.lbl_deb_2->setText(tmp);
	
}
//点击自适应核回归去噪CPU
void one::on_btn_cla_3_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb_3->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	int time =Test_self_CPU(name);
	QString tmp=QString::number(time);
	tmp+="ms";
	this->ui.lbl_deb_3->setText(tmp);
	
}
//点击自适应核回归去噪GPU
void one::on_btn_cla_4_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb_4->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	int time =Test_GPU(name,2);
	QString tmp=QString::number(time);
	tmp+="ms";
	this->ui.lbl_deb_4->setText(tmp);
	
}
//点击均值滤波去噪CPU
void one::on_btn_cla_5_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb_5->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	int time =Avera_CPU(name);
	QString tmp=QString::number(time);
	tmp+="ms";
	this->ui.lbl_deb_5->setText(tmp);
}
//点击均值滤波去噪GPU
void one::on_btn_cla_6_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb_6->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	float time =Test_GPU(name,3);
	QString tmp=QString::number(time);
	tmp+="ms";
	this->ui.lbl_deb_6->setText(tmp);
}
//点击中值滤波去噪CPU
void one::on_btn_cla_7_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb_7->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	int time =Middl_CPU(name);
	QString tmp=QString::number(time);
	tmp+="ms";
	this->ui.lbl_deb_7->setText(tmp);
}
//点击中值滤波去噪GPU
void one::on_btn_cla_8_clicked(){
	if(mark==0){
		QString tmp="No Image";
		this->ui.lbl_deb_8->setText(tmp);
		return;
	}
	string name=img_name.toStdString();
	int time =Test_GPU(name,4);
	QString tmp=QString::number(time);
	tmp+="ms";
	this->ui.lbl_deb_8->setText(tmp);
}