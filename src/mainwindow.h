#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>

class Preview;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = NULL, Qt::WindowFlags flags = Qt::WindowFlags());
	~MainWindow();

	void AddFile(const QString &);

	const QColor& fillColor() { return fFillColor; }

private slots:
	void AddImage();
	void RemoveImage();
	void PreviewImage();
	void SetBackground();
	void ListSelectionChanged();

protected:
	friend class Preview;
	QListWidget *list;

private:
	void LoadSettings();
	void SaveSettings();
	void closeEvent(QCloseEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

	Preview *preview;
	QCheckBox *fAutoBorder;
	QPushButton *fPreview, *fRemove;
	QColor fFillColor;
};
