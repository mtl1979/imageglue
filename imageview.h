#include <qwidget.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlineedit.h>

class MainWindow;
class Preview;

class ImageView : public QWidget
{
	Q_OBJECT
public:
	ImageView(QWidget * parent = NULL, Qt::WindowFlags flags = Qt::WindowFlags());
	~ImageView();

	void Load(const QString &image);

protected:
	friend class MainWindow;
	friend class Preview;

	QImage *image;
	QLineEdit *offsetX, *offsetY;
	QLineEdit *cropTopX, *cropTopY;
	QLineEdit *cropBottomX, *cropBottomY;
	QLineEdit *scale;

private:
	void resizeEvent(QResizeEvent *);

	QLabel *label1;
};