#include "imageview.h"

#include <qgridlayout.h>
#include <qicon.h>
#include <qlineedit.h>
#include <qvalidator.h>

ImageView::ImageView(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
	QLocale loc;
	image = NULL;
	setWindowTitle(tr("Image Viewer"));
	setWindowIcon(QIcon(":/ImageGlue.png"));
	resize(640, 400);
	//

	QGridLayout *gridLayout = new QGridLayout(this);
	Q_CHECK_PTR(gridLayout);

	label1 = new QLabel();
	Q_CHECK_PTR(label1);
	label1->setText(tr("No file!"));
	gridLayout->addWidget(label1, 0, 0, 1, 4);

	QLabel * label2 = new QLabel();
	Q_CHECK_PTR(label2);
	label2->setText(tr("Offset:"));
	label2->setMaximumHeight(20);
	gridLayout->addWidget(label2, 1, 0);

	QLabel * label3 = new QLabel();
	Q_CHECK_PTR(label3);
	label3->setText(tr("X"));
	label3->setMaximumHeight(20);
	gridLayout->addWidget(label3, 2, 0);

	offsetX = new QLineEdit();
	Q_CHECK_PTR(offsetX);
	offsetX->setMaxLength(5);
	offsetX->setText("0");
	offsetX->setValidator(new QIntValidator());
	offsetX->setMaximumHeight(20);
	gridLayout->addWidget(offsetX, 2, 1);

	QLabel * label4 = new QLabel();
	Q_CHECK_PTR(label4);
	label4->setText(tr("Y"));
	label4->setMaximumHeight(20);
	gridLayout->addWidget(label4, 2, 2);

	offsetY = new QLineEdit();
	Q_CHECK_PTR(offsetY);
	offsetY->setMaxLength(5);
	offsetY->setText("0");
	offsetY->setValidator(new QIntValidator());
	offsetY->setMaximumHeight(20);
	gridLayout->addWidget(offsetY, 2, 3);

	QLabel * label5 = new QLabel();
	Q_CHECK_PTR(label5);
	label5->setText(tr("Crop:"));
	label5->setMaximumHeight(20);
	gridLayout->addWidget(label5, 3, 0);

	//

	QLabel * label6 = new QLabel();
	Q_CHECK_PTR(label6);
	label6->setText(tr("Top"));
	label6->setMaximumHeight(20);
	gridLayout->addWidget(label6, 4, 0);

	QLabel * label7 = new QLabel();
	Q_CHECK_PTR(label7);
	label7->setText(tr("X"));
	label7->setMaximumHeight(20);
	gridLayout->addWidget(label7, 5, 0);

	cropTopX = new QLineEdit();
	Q_CHECK_PTR(cropTopX);
	cropTopX->setMaxLength(5);
	cropTopX->setText("0");
	cropTopX->setValidator(new QIntValidator());
	cropTopX->setMaximumHeight(20);
	gridLayout->addWidget(cropTopX, 5, 1);

	QLabel * label8 = new QLabel();
	Q_CHECK_PTR(label8);
	label8->setText(tr("Y"));
	label8->setMaximumHeight(20);
	gridLayout->addWidget(label8, 5, 2);

	cropTopY = new QLineEdit();
	Q_CHECK_PTR(cropTopY);
	cropTopY->setMaxLength(5);
	cropTopY->setText("0");
	cropTopY->setValidator(new QIntValidator());
	cropTopY->setMaximumHeight(20);
	gridLayout->addWidget(cropTopY, 5, 3);

//

	QLabel * label9 = new QLabel();
	Q_CHECK_PTR(label9);
	label9->setText(tr("Bottom"));
	label9->setMaximumHeight(20);
	gridLayout->addWidget(label9, 6, 0);

	QLabel * label10 = new QLabel();
	Q_CHECK_PTR(label10);
	label10->setText(tr("X"));
	label10->setMaximumHeight(20);
	gridLayout->addWidget(label10, 7, 0);

	cropBottomX = new QLineEdit();
	Q_CHECK_PTR(cropBottomX);
	cropBottomX->setMaxLength(5);
	cropBottomX->setText("0");
	cropBottomX->setValidator(new QIntValidator());
	cropBottomX->setMaximumHeight(20);
	gridLayout->addWidget(cropBottomX, 7, 1);

	QLabel * label11 = new QLabel();
	Q_CHECK_PTR(label11);
	label11->setText(tr("Y"));
	label11->setMaximumHeight(20);
	gridLayout->addWidget(label11, 7, 2);

	cropBottomY = new QLineEdit();
	Q_CHECK_PTR(cropBottomY);
	cropBottomY->setMaxLength(5);
	cropBottomY->setText("0");
	cropBottomY->setValidator(new QIntValidator());
	cropBottomY->setMaximumHeight(20);
	gridLayout->addWidget(cropBottomY, 7, 3);

	QLabel * label12 = new QLabel();
	Q_CHECK_PTR(label12);
	label12->setText(tr("Scale"));
	label12->setMaximumHeight(20);
	gridLayout->addWidget(label12, 8, 0, 1, 2);

	scale = new QLineEdit();
	Q_CHECK_PTR(scale);
	scale->setMaxLength(5);
	scale->setText(QString("0") + loc.decimalPoint() + QString("000"));
	scale->setValidator(new QDoubleValidator(0.001, 9.999, 3));
	scale->setMaximumHeight(20);
	gridLayout->addWidget(scale, 8, 2, 1, 2);

}

ImageView::~ImageView()
{
}

void
ImageView::Load(const QString &file)
{
	if (image == NULL)
		image = new QImage();

	if (image && image->load(file))
	{
		QImage preview = image->scaled(QSize(label1->width(), label1->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		label1->setPixmap(QPixmap::fromImage(preview));
		setWindowTitle(file);
	}
}

void
ImageView::resizeEvent(QResizeEvent *e)
{
	if (image)
	{
		QImage preview = image->scaled(QSize(label1->width(), label1->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		label1->setPixmap(QPixmap::fromImage(preview));
	}

	QWidget::resizeEvent(e);
}