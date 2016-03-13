#include <QtWidgets/qapplication.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qtranslator.h>

#ifdef _WIN32
#  include <windows.h>
#  include <shlwapi.h>
#else
#  include <unistd.h>
#endif

#include "mainwindow.h"
#include "util.h"
QString gAppDir;
#ifdef _WIN32
QString gDataDir;
#endif

#ifndef _WIN32
void
SetWorkingDirectory(const char *app)
{
	const char * wdir = strrchr(app, '/');

	if (wdir)
	{
		char * chd = new char[wdir - app + 1]; // figure out length, and make a new string of that length
		if (chd)
		{
			strncpy(chd, app, wdir - app);
			chd[wdir - app] = 0;
			PRINT("Setting working directory to: %s\n", chd);
			if (chdir(chd) != 0)
				PRINT("Setting working directory failed!\n");
			delete [] chd;
		}
	}
}
#else
QString
GetAppDirectory()
{
	// we have to use some Windows API to get our path...
	wchar_t * name = new wchar_t[MAX_PATH];	// maximum size for Win32 filenames
	Q_CHECK_PTR(name);
	if (GetModuleFileName(NULL,				/* current apps module */
							name,			/* buffer */
							MAX_PATH		/* buffer length */
							) != 0)
	{
		qDebug("Module filename: %ls", name);
		PathRemoveFileSpec(name);
		if (SetCurrentDirectory(name) == 0)
		{
			GetCurrentDirectory(MAX_PATH, name);
			qDebug("Current directory: %ls", name);
		}
		else
			qDebug("Application directory: %ls", name);
	}
	QString qname = QString::fromWCharArray(name);
	delete [] name;
	name = NULL; // <postmaster@raasu.org> 20021027
	return qname;
}
#endif

int
main( int argc, char**argv )
{
	QApplication app( argc, argv );
	QTranslator qtr( 0 );
	QTranslator qtr2( 0 );

	// Set our working directory

#ifdef _WIN32
	QString appdir = GetAppDirectory();
	QString datadir = qgetenv("APPDATA");
	QDir dir(datadir);
	dir.mkdir("ImageGlue");
	datadir = MakePath(datadir, "ImageGlue");
	gDataDir = datadir;
	gAppDir = appdir;
	// Set our working directory
	QDir::setCurrent(datadir);
#endif

	// Load language file
	QString langfile;
#ifndef _WIN32
	langfile = "imageglue.lng";
	// Set our working directory
	SetWorkingDirectory(argv[0]);
	gAppDir = QDir::currentPath();
#else
	langfile = MakePath(gDataDir, "imageglue.lng");
#endif
	QFile lang(langfile);
	QString lfile;
	QString ldir = MakePath(gAppDir, "translations");

	if (!QDir(ldir).exists())
		ldir = gAppDir;

	if (!lang.exists())
	{
NoTranslation:
		lfile = QFileDialog::getOpenFileName(NULL, app.translate("main", "Open translation file..."), ldir, "imageglue_*.qm");
		// Save selected language's translator filename
		if (!lfile.isEmpty() && lang.open(QIODevice::WriteOnly) )
		{
			QByteArray clang = lfile.toUtf8();
			lang.write(clang);
			lang.close();
		}
	}

	// (Re-)load translator filename
	if ( lang.open(QIODevice::ReadOnly) )
	{
		// file opened successfully
		QByteArray plang = lang.readLine();
		lfile = QString::fromUtf8(plang);
		lang.close();
	}

	// Install translator ;)
	if (!lfile.isEmpty() && QFile::exists(lfile) && qtr.load(lfile))
		app.installTranslator(&qtr);
	else
		goto NoTranslation;

	// Qt's own translator file
	QFileInfo qfi(lfile);
	QString qt_lang = QString::null;
	QString qtdir = qgetenv("QTDIR");
	langfile = qfi.fileName().replace(QRegExp("imageglue"), "qt");

	if (qtdir != QString::null)
	{
		QString tr_dir = MakePath(qtdir, "translations");
		qt_lang = MakePath(tr_dir, langfile);
		if (!QFile::exists(qt_lang))
			qt_lang = QString::null;
	}

	// Try using same directory as Image Splitter's translations
	if (qt_lang == QString::null)
	{
		qt_lang = MakePath(qfi.absolutePath(), langfile);
	}

	if (QFile::exists(qt_lang) && qtr2.load(qt_lang))
	{
		app.installTranslator( &qtr2 );
	}

	MainWindow *window = new MainWindow();
	Q_CHECK_PTR(window);

	window->show();

	return app.exec();
}
