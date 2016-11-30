#include "mainwindow.h"
#include "imageview.h"
#include "preview.h"
#include "global.h"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcolordialog.h>
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
	setWindowIcon(QIcon(":/ImageGlue.png"));
	setAcceptDrops(true);
	fFillColor = QColor(Qt::white);
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
	gridLayout->addWidget(list, 1, 0, 1, 4);

	QPushButton *add = new QPushButton();
	Q_CHECK_PTR(add);
	add->setText(tr("Add"));
	gridLayout->addWidget(add, 2, 0);
	connect(add, SIGNAL(clicked()), this, SLOT(AddImage()));

	QPushButton *background = new QPushButton();
	Q_CHECK_PTR(background);
	background->setText(tr("Background"));
	gridLayout->addWidget(background, 2, 1);
	connect(background, SIGNAL(clicked()), this, SLOT(SetBackground()));

	fPreview = new QPushButton();
	Q_CHECK_PTR(fPreview);
	fPreview->setText(tr("Preview"));
	fPreview->setEnabled(false);
	gridLayout->addWidget(fPreview, 2, 2);
	connect(fPreview, SIGNAL(clicked()), this, SLOT(PreviewImage()));

	fRemove = new QPushButton();
	Q_CHECK_PTR(fRemove);
	fRemove->setText(tr("Remove"));
	fRemove->setEnabled(false);
	gridLayout->addWidget(fRemove, 2, 3);
	connect(fRemove, SIGNAL(clicked()), this, SLOT(RemoveImage()));

	LoadSettings();
}

void
MainWindow::LoadSettings()
{
	QString filename = QString::null;
	gLastdir = QString::null;
	QFile qf("imageglue.ini");
	if (qf.open(QIODevice::ReadOnly))
	{
		QByteArray temp(256, 0);
		if (qf.readLine(temp.data(), 255) > 0)
		{
			gLastdir = QString::fromUtf8(temp);
			gLastdir.replace("\n", "");
		}
		if (qf.readLine(temp.data(), 255) > 0)
		{
			QString tmp = QString::fromUtf8(temp);
			tmp.replace("\n", "");
			fFillColor = QColor(tmp);
		}
		if (qf.readLine(temp.data(), 255) > 0)
		{
			QString tmp = QString::fromUtf8(temp);
			tmp.replace("\n", "");
			fFillColor.setAlpha(tmp.toInt());
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
		QByteArray temp = gLastdir.toUtf8();
		qf.write(temp);
		qf.write("\n");
		qf.write(fFillColor.name().toUtf8());
		qf.write("\n");
		qf.write(QString::number(fFillColor.alpha()).toUtf8());
		qf.close();
	}
}

void
MainWindow::AddImage()
{
	QString file = QFileDialog::getOpenFileName(NULL, tr("Open image..."), gLastdir, "Image files (*.jpg;*.jpeg;*.png;*.gif;*.bmp)");
	if (file != QString::null)
	{
		AddFile(file);
	}
}

void
MainWindow::AddFile(const QString & file)
{
	QFileInfo info(file);
	gLastdir = info.path();
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
		QLocale loc;
		ImageView * view = list->item(x)->data(Qt::UserRole).value<ImageView *>();
		int top = view->offsetY->text().toInt();
		int left = view->offsetX->text().toInt();
		int croptop = view->cropTopY->text().toInt();
		int cropleft = view->cropTopX->text().toInt();
		int cropbottom = view->cropBottomY->text().toInt();
		int cropright = view->cropBottomX->text().toInt();
		double scale = loc.toDouble(view->scale->text());
		int width = left;
		int height = top;
		int subwidth = view->image->width() - cropleft - cropright;
		int subheight = view->image->height() - croptop - cropbottom;
		if (scale != 0.0)
		{
			subwidth *= scale;
			subheight *= scale;
		}
		width += subwidth;
		height += subheight;
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

void
MainWindow::SetBackground()
{
	QColor fillcolor = QColorDialog::getColor(fFillColor, this, tr("Select fill color..."), QColorDialog::ShowAlphaChannel);
	if (fillcolor.isValid())
	{
		fFillColor = fillcolor;
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
