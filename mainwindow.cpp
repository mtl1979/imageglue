#include "mainwindow.h"
#include "imageview.h"
#include "preview.h"
#include "global.h"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>

#include <qevent.h>
#include <qmimedata.h>

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	preview = NULL;
	setWindowTitle("ImageGlue");
	setAcceptDrops(true);
	//
	QWidget* widget = new QWidget(this);
	Q_CHECK_PTR(widget);

	QGridLayout *gridLayout = new QGridLayout();
	Q_CHECK_PTR(gridLayout);
	gridLayout->setGeometry(geometry());

	widget->setLayout(gridLayout);
	setCentralWidget(widget);

	QLabel *label1 = new QLabel();
	Q_CHECK_PTR(label1);
	label1->setText(tr("Images:"));
	gridLayout->addWidget(label1, 0, 0);

	list = new QListWidget();
	Q_CHECK_PTR(list);
	connect(list, SIGNAL(itemSelectionChanged()), this, SLOT(ListSelectionChanged()));
	gridLayout->addWidget(list, 1, 0, 1, 3);

	QPushButton *add = new QPushButton();
	Q_CHECK_PTR(add);
	add->setText(tr("Add"));
	gridLayout->addWidget(add, 2, 0);
	connect(add, SIGNAL(clicked()), this, SLOT(AddImage()));

	fPreview = new QPushButton();
	Q_CHECK_PTR(fPreview);
	fPreview->setText(tr("Preview"));
	fPreview->setEnabled(false);
	gridLayout->addWidget(fPreview, 2, 1);
	connect(fPreview, SIGNAL(clicked()), this, SLOT(PreviewImage()));

	fRemove = new QPushButton();
	Q_CHECK_PTR(fRemove);
	fRemove->setText(tr("Remove"));
	fRemove->setEnabled(false);
	gridLayout->addWidget(fRemove, 2, 2);
	connect(fRemove, SIGNAL(clicked()), this, SLOT(RemoveImage()));

	LoadSettings();
}

void
MainWindow::LoadSettings()
{
	QString filename = QString::null;
	lastdir = QString::null;
	QFile qf("imageglue.ini");
	if (qf.open(QIODevice::ReadOnly))
	{
		QByteArray temp(256, 0);
		if (qf.readLine(temp.data(), 255) > 0)
		{
			lastdir = QString::fromUtf8(temp);
			lastdir.replace("\n", "");
		}
		qf.close();
	}
}

void
MainWindow::SaveSettings()
{
	QFile qf("imageglue.ini");
	if (qf.open(QIODevice::WriteOnly))
	{
		QByteArray temp = lastdir.toUtf8();
		qf.write(temp);
		qf.close();
	}
}

void
MainWindow::AddImage()
{
	QString file = QFileDialog::getOpenFileName(NULL, tr("Open image..."), lastdir, "Image files (*.jpg;*.jpeg;*.png;*.gif;*.bmp)");
	if (file != QString::null)
	{
		AddFile(file);
	}
}

void
MainWindow::AddFile(const QString & file)
{
	QFileInfo info(file);
	lastdir = info.path();
	QString nfile = QDir::toNativeSeparators(file);
	QImage image;
	if (image.load(nfile))
	{
		ImageView * view = new ImageView();
		if (view)
		{
			QListWidgetItem *item = new QListWidgetItem(nfile);
			if (item)
			{
				QVariant val;
				val.setValue(view);
				item->setData(Qt::UserRole, val);
				list->addItem(item);
				if (fPreview->isEnabled() == false)
					fPreview->setEnabled(true);
				view->Load(nfile);
				view->show();
			}
		}
	}
}

void
MainWindow::RemoveImage()
{
	QListWidgetItem *current = list->currentItem();
	if (current)
	{
		ImageView * view = current->data(Qt::UserRole).value<ImageView *>();
		view->close();
		list->takeItem(list->row(current));
		if (list->count() == 0)
		{
			fPreview->setEnabled(false);
			fRemove->setEnabled(false);
		}
	}
}

void
MainWindow::PreviewImage()
{
	if (list->count() == 0)
		return;

	int maxw = 0, maxh = 0;
	for (int x = 0; x < list->count(); x++)
	{
		ImageView * view = list->item(x)->data(Qt::UserRole).value<ImageView *>();
		int top = view->offsetY->text().toInt();
		int left = view->offsetX->text().toInt();
		int croptop = view->cropTopY->text().toInt();
		int cropleft = view->cropTopX->text().toInt();
		int cropbottom = view->cropBottomY->text().toInt();
		int cropright = view->cropBottomX->text().toInt();
		int width = view->image->width();
		int height = view->image->height();
		width += left - cropleft - cropright;
		height += top - croptop - cropbottom;
		if (width > maxw)
			maxw = width;
		if (height > maxh)
			maxh = height;
	}
	if (preview == NULL)
		preview = new Preview();

	if (preview)
	{
		preview->SetImageSize(maxw, maxh);
		preview->ShowPreview(this);
		preview->show();
	}
}

MainWindow::~MainWindow()
{
	SaveSettings();
}

void
MainWindow::closeEvent(QCloseEvent *e)
{
	SaveSettings();
	qApp->closeAllWindows();
	e->accept();
}

void
MainWindow::dragEnterEvent(QDragEnterEvent * e)
{
	if (e->mimeData()->hasUrls())
		e->acceptProposedAction();
}

void
MainWindow::dropEvent(QDropEvent *e)
{
	int numFiles = 0;
	if (e->mimeData()->hasUrls())
	{
		for (int x = 0; x < e->mimeData()->urls().count(); x++)
		{
			QImage image;
			QString file = e->mimeData()->urls().at(x).toLocalFile();
			if (image.load(file))
			{
				AddFile(file);
				numFiles++;
			}
		}
	}
	if (numFiles > 0)
		e->acceptProposedAction();

	QMainWindow::dropEvent(e);
}

void
MainWindow::ListSelectionChanged()
{
	fRemove->setEnabled(list->currentItem() != NULL);
}
