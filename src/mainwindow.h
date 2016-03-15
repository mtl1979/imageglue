#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = NULL, Qt::WindowFlags flags = Qt::WindowFlags());
	~MainWindow();

private slots:
	void AddImage();
	void RemoveImage();
	void PreviewImage();
	void ListSelectionChanged();

protected:
	friend class Preview;
	QListWidget *list;

private:
	void LoadSettings();
	void SaveSettings();
	void AddFile(const QString &);
	void closeEvent(QCloseEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

	Preview *preview;
	QPushButton *fPreview, *fRemove;
};
