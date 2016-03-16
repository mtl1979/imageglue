#include <qlabel.h>
#include <qwidget.h>

class MainWindow;

class Preview : public QWidget
{
	Q_OBJECT

public:
	Preview(QWidget * parent = NULL, Qt::WindowFlags flags = Qt::WindowFlags());
	~Preview();

	void SetImageSize(int w, int h);
	void ShowPreview(MainWindow * window);

private slots:
	void SaveImage();
	void CopyImage();

private:
	bool eventFilter(QObject *, QEvent *);

	void resizeEvent(QResizeEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	void startDrag();

	int _width, _height;
	bool dragging;
	QPoint startPos;
	QImage * image;
	QLabel * img;
};
