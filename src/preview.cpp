#include "preview.h"
#include "mainwindow.h"
#include "imageview.h"
#include "global.h"

#include <qaction.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qdrag.h>
#include <qevent.h>
#include <qerrormessage.h>
#include <qfiledialog.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qimage.h>
#include <qmimedata.h>
#include <qpainter.h>
#include <qpushbutton.h>

Preview::Preview(QWidget *parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
	image = NULL;
	dragging = false;
	setWindowTitle(tr("Preview"));
	setWindowIcon(QIcon(":/ImageGlue.png"));
	//
	QGridLayout * layout = new QGridLayout(this);
	Q_CHECK_PTR(layout);

	img = new QLabel();
	Q_CHECK_PTR(img);
	img->setMaximumSize(800, 600);
	img->installEventFilter(this);
	img->setContextMenuPolicy(Qt::ActionsContextMenu);
	layout->addWidget(img, 0, 0, 1, 3);

	QAction *copyAct = new QAction(tr("Copy"), NULL);
	Q_CHECK_PTR(copyAct);
	copyAct->setShortcut(Qt::ControlModifier + Qt::Key_C);
	connect(copyAct, SIGNAL(triggered()), this, SLOT(CopyImage()));
	img->addAction(copyAct);

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
			QLocale loc;
			ImageView * view = window->list->item(x)->data(Qt::UserRole).value<ImageView *>();
			int top = view->offsetY->text().toInt();
			int left = view->offsetX->text().toInt();
			int croptop = view->cropTopY->text().toInt();
			int cropleft = view->cropTopX->text().toInt();
			int cropbottom = view->cropBottomY->text().toInt();
			int cropright = view->cropBottomX->text().toInt();
			double scale = loc.toDouble(view->scale->text());
			int iwidth = view->image->width();
			int iheight = view->image->height();

			QImage sub;

			if (cropleft != 0 || cropright != 0 || croptop != 0 || cropbottom != 0)
				sub = view->image->copy(cropleft, croptop, iwidth - cropleft - cropright, iheight - croptop - cropbottom);
			else
				sub = *view->image;

			if (scale != 0.0)
			{
				sub = sub.scaled(lrint((double)sub.width()*scale), lrint((double)sub.height()*scale), Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}

			painter.drawImage(left, top, sub);
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

bool
Preview::eventFilter(QObject *o, QEvent *e)
{
	if (o == img)
	{
		switch (e->type())
		{
		case QEvent::MouseButtonPress:
			mousePressEvent((QMouseEvent *)e);
			return true;
		case QEvent::MouseMove:
			mouseMoveEvent((QMouseEvent *)e);
			return true;
		case QEvent::MouseButtonRelease:
			mouseReleaseEvent((QMouseEvent *)e);
			return true;
		}
	}
	return QWidget::eventFilter(o, e);
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

void Preview::mousePressEvent(QMouseEvent *e)
{
	if (e->button() & Qt::LeftButton)
	{
		qDebug("Started dragging...\n");
		dragging = true;
		startPos = e->pos();
	}
	QWidget::mousePressEvent(e);
}

void Preview::mouseMoveEvent(QMouseEvent *e)
{
	if (dragging)
	{
		QPoint currPos = e->pos();
		if ((startPos - currPos).manhattanLength() > QApplication::startDragDistance())
			startDrag();
	}
	QWidget::mouseMoveEvent(e);
}

void Preview::mouseReleaseEvent(QMouseEvent *e)
{
	if (dragging)
	{
		qDebug("Stopped dragging...\n");
		dragging = false;
	}
	QWidget::mouseReleaseEvent(e);
}

void Preview::startDrag()
{
	if (image)
	{
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData;
		mimeData->setImageData(*image);
		drag->setMimeData(mimeData);

		drag->exec(Qt::CopyAction);
	}
}

void Preview::CopyImage()
{
	if (image)
	{
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setImage(*image);
	}
}
