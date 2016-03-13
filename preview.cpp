#include "preview.h"
#include "mainwindow.h"
#include "imageview.h"
#include "global.h"

#include <qerrormessage.h>
#include <qfiledialog.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpainter.h>
#include <qpushbutton.h>

Preview::Preview(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
	image = NULL;
	setWindowTitle(tr("Preview"));
	//
	QGridLayout * layout = new QGridLayout(this);
	Q_CHECK_PTR(layout);

	img = new QLabel();
	Q_CHECK_PTR(img);
	img->setMaximumSize(800, 600);
	layout->addWidget(img, 0, 0, 1, 3);

	QPushButton *save = new QPushButton();
	Q_CHECK_PTR(save);
	save->setText(tr("Save"));
	layout->addWidget(save, 1, 1, 1, 1);
	connect(save, SIGNAL(clicked()), this, SLOT(SaveImage()));
	//
}

Preview::~Preview()
{
}

void Preview::SetImageSize(int w, int h)
{
	_width = w;
	_height = h;
}

void Preview::ShowPreview(MainWindow * window)
{
	if (image != NULL)
		delete image;

	image = new QImage(_width, _height, QImage::Format_ARGB32);

	if (image)
	{
		QPainter painter(image);
		painter.fillRect(0, 0, _width, _height, Qt::white);

		for (int x = 0; x < window->list->count(); x++)
		{
			ImageView * view = window->list->item(x)->data(Qt::UserRole).value<ImageView *>();
			int top = view->offsetY->text().toInt();
			int left = view->offsetX->text().toInt();
			int croptop = view->cropTopY->text().toInt();
			int cropleft = view->cropTopX->text().toInt();
			int cropbottom = view->cropBottomY->text().toInt();
			int cropright = view->cropBottomX->text().toInt();
			int iwidth = view->image->width();
			int iheight = view->image->height();

			painter.drawImage(left, top, *(view->image), cropleft, croptop, iwidth - cropleft - cropright, iheight - croptop - cropbottom);
		}

		QImage preview = image->scaled(img->width(), img->height(), Qt::KeepAspectRatio);
		img->setPixmap(QPixmap::fromImage(preview));
	}
}

void Preview::SaveImage()
{
	QString file = QFileDialog::getSaveFileName(NULL, tr("Save image..."), lastdir, "Image files (*.jpg;*.jpeg;*.png;*.gif;*.bmp)");
	if (file != QString::null)
	{
		QFileInfo info(file);
		lastdir = info.path();

		if (!image->save(file))
		{
			QErrorMessage * error = new QErrorMessage();
			Q_CHECK_PTR(error);
			error->showMessage(tr("Couldn't save image!"));
		}
	}
}

void Preview::resizeEvent(QResizeEvent *e)
{
	if (image)
	{
		QImage preview = image->scaled(img->width(), img->height(), Qt::KeepAspectRatio);
		img->setPixmap(QPixmap::fromImage(preview));
	}
	QWidget::resizeEvent(e);
}
