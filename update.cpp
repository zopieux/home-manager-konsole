#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KMainWindow>
#include <KSharedConfig>
#include <KToolBar>
#include <KXmlGuiWindow>
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDockWidget>

#define SET(name)                                                          \
  do {                                                                     \
    if (parser.isSet(QStringLiteral(#name))) {                             \
      const bool was = !(name)->isHidden(),                                \
                 now = IsTrue(parser.value(QStringLiteral(#name)));        \
      if (was != now) {                                                    \
        report << "  " #name ": " << ToBool(was) << " --> " << ToBool(now) \
               << "\n";                                                    \
        (name)->setHidden(!now);                                           \
        save = true;                                                       \
      }                                                                    \
    }                                                                      \
  } while (0)

QString ToBool(bool e) {
  QString s;
  QDebug d(&s);
  d << e;
  return s;
}

bool IsTrue(const QString& e) {
  return e == QStringLiteral("yes") || e == QStringLiteral("true") ||
         e == QStringLiteral("1");
}

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  KLocalizedString::setApplicationDomain("dummy");

  QCommandLineParser parser;
  parser.addPositionalArgument(QStringLiteral("file"),
                               QStringLiteral("Path to config file to update"));
  parser.addOption({QStringLiteral("main"),
                    QStringLiteral("“Main Toolbar”, visible yes/no"),
                    QStringLiteral("main")});
  parser.addOption({QStringLiteral("session"),
                    QStringLiteral("“Session Toolbar”, visible yes/no"),
                    QStringLiteral("session")});
  parser.addOption(
      {QStringLiteral("qc"),
       QStringLiteral("“Show Quick Commands” (dock), visible yes/no"),
       QStringLiteral("qc")});
  parser.addOption({QStringLiteral("ssh"),
                    QStringLiteral("“Show SSH Manager” (dock), visible yes/no"),
                    QStringLiteral("ssh")});
  parser.setApplicationDescription(
      QStringLiteral("Updater for Konsole config"));
  parser.addHelpOption();
  parser.process(app);

  const QString& configToUpdate = parser.positionalArguments().at(0);

  KXmlGuiWindow w;

  auto* main = w.toolBar(QStringLiteral("mainToolBar"));
  auto* session = w.toolBar(QStringLiteral("sessionToolbar"));

  auto* qc = new QDockWidget(&w);
  qc->setObjectName(QStringLiteral("QuickCommandsDock"));
  qc->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea |
                      Qt::DockWidgetArea::RightDockWidgetArea);
  w.addDockWidget(Qt::LeftDockWidgetArea, qc);

  auto* ssh = new QDockWidget(&w);
  ssh->setObjectName(QStringLiteral("SSHManagerDock"));
  ssh->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea |
                       Qt::DockWidgetArea::RightDockWidgetArea);
  w.addDockWidget(Qt::LeftDockWidgetArea, ssh);

  QTextStream report(stdout);
  report << "Changes made to " << configToUpdate << ":\n";

  auto cg =
      KSharedConfig::openConfig(configToUpdate, KConfig::OpenFlag::SimpleConfig)
          ->group("MainWindow");
  w.applyMainWindowSettings(cg);
  bool save = false;
  SET(main);
  SET(session);
  SET(ssh);
  SET(qc);
  if (save) {
    w.saveMainWindowSettings(cg);
    cg.sync();
  } else {
    report << "  (none)\n";
  }
  return 0;
}