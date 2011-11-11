#include "testwin.h"

// Qt Header
#include <QDomDocument>
#include <QGraphicsDropShadowEffect>
#include <qdeclarative.h>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QUrl>

// KDE Header
#include <kselectaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kfiledialog.h>
#include <kaction.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kmenu.h>
#include <kconfigdialog.h>

#include <kcombobox.h>
#include <kfontrequester.h>
#include <knuminput.h>
#include <kdeclarative.h>
#include <kicon.h>
#include <kio/netaccess.h>

#include "core/keyboard.h"
#include "core/key.h"
#include "core/specialkey.h"
#include "core/keychar.h"


KTouch::KTouch()
    : m_view(new QDeclarativeView(this))
{
    //setFocusPolicy(Qt::StrongFocus);

    //setAttribute(Qt::WA_InputMethodEnabled, true);
 //   setAttribute(Qt::WA_KeyCompression );

    resetInputContext ();

    // General initialization of the program, common for all start modes
    //init();

    qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");
    qmlRegisterType<AbstractKey>("ktouch", 1, 0, "AbstractKeyModel");
    qmlRegisterType<Key>("ktouch", 1, 0, "KeyModel");
    qmlRegisterType<SpecialKey>("ktouch", 1, 0, "SpecialKeyModel");
    qmlRegisterType<KeyChar>("ktouch", 1, 0, "KeyCharModel");

    KDeclarative kDeclarative;
    kDeclarative.setDeclarativeEngine(m_view->engine());
    kDeclarative.initialize();
    kDeclarative.setupBindings();


    KStandardDirs* dirs = KGlobal::dirs();
    Keyboard* keyboad = new Keyboard(0);
    QFile file(dirs->findResource("appdata", "keyboards/de.xml"));
    file.open(QIODevice::ReadOnly);
    keyboad->loadXML(&file);

    setupGUI();
    setCentralWidget(m_view);
    QString res = KGlobal::dirs()->findResource("appdata", "qml/Keyboard.qml");
    m_view->rootContext()->setContextProperty("keyboardModel", keyboad);
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_view->setSource(QUrl::fromLocalFile(res));


}
// ----------------------------------------------------------------------------

// Free memory of objects that are not owned by the main KTouch object
KTouch::~KTouch() {
}


// *********************************
// *** Protected member function ***
// *********************************

bool KTouch::queryExit() {
    return true;
}
// ----------------------------------------------------------------------------

void KTouch::resizeEvent(QResizeEvent * event) {
}
// ----------------------------------------------------------------------------


// *******************************
// *** Private member function ***
// *******************************


// Initialises the program during a normal startup
void KTouch::init() {
    kDebug() << "[KTouch::init]  populating file lists...";
    updateFileLists();  // create lists with default lecture/keyboard/examination files/colour scheme files
    kDebug() << "[KTouch::init]  " << m_lectureFiles.count() << " lectures available";
    kDebug() << "[KTouch::init]  " << m_keyboardFiles.count() << " keyboard layouts available";
    kDebug() << "[KTouch::init]  " << m_examinationFiles.count() << " examination files available";

}
// ----------------------------------------------------------------------------


// This function populates the file lists with the installed training, keyboard and
// examination files.
void KTouch::updateFileLists() {
    // first search for all installed new format keyboard files
    KStandardDirs *dirs = KGlobal::dirs();
    m_keyboardFiles = dirs->findAllResources("data","ktouch/*.keyboard.xml",KStandardDirs::Recursive);

    // extract titles from keyboard files and store them in the
    // m_keyboardTitles string list
    m_keyboardTitles.clear();
    kDebug() << "Keyboard files in XML format:";
    for (QStringList::const_iterator cit = m_keyboardFiles.constBegin();
        cit != m_keyboardFiles.constEnd(); ++cit) {
        KUrl url = (*cit);
        QString target;
        // try to read language code and keyboard name from file
        if (!KIO::NetAccess::download(url, target, this)) {
            continue;
        }
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( QIODevice::ReadOnly ) ) {
            continue;
        }
        QDomDocument doc;
        doc.setContent( &infile );
        QDomElement root = doc.documentElement();
        if (root.isNull() || root.tagName() != "KTouchKeyboard") {
            continue;
        }
        // get the title
        QString title;
        QDomElement e = root.firstChildElement("Title");
        if (!e.isNull()) {
            title = i18nc("Keyboard Layout Name", e.firstChild().nodeValue().toUtf8());
        }
        else {
            title = i18n("untitled keyboard layout");
        }
        QString langid;
        // retrieve the language id
        e = root.firstChildElement("Language");
        if (!e.isNull()) {
            langid = e.firstChild().nodeValue();
        }
        // compose title for main menu
        if (!langid.isEmpty()) {
            title = QString("%1 (%2)").arg(title).arg(langid);
        }
        m_keyboardTitles.append( title );
        kDebug() << "  " << m_keyboardTitles.back() << "\t [" << url << "]";
    }

    // now sort the files and titles accordingly
    // sort_lists(m_keyboardTitles, m_keyboardFiles);
    // and add the number keypad to the front

    // Now lets find the lecture files.
    // TODO : search in i18n() directories
    QStringList files;
    QStringList lectureFiles = dirs->findAllResources("data","ktouch/Lecture/*.ktouch.xml",KStandardDirs::Recursive,files);
    // Now extract the titles of the lecture files and populate the string lists used in the program
    m_lectureFiles.clear();
    m_lectureTitles.clear();

    // Now find predefined files with colour schemes
    QStringList colour_schemes = dirs->findAllResources("data","ktouch/*.colour_scheme");
    // TODO : read in colour schemes and populate QValueList<KTouchColorScheme>
}
// ----------------------------------------------------------------------------


