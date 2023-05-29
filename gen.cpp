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
#include <QSettings>
#include <QTemporaryFile>
#include <iostream>

#define SET(name)                                                      \
  do {                                                                 \
    if (parser.isSet(QStringLiteral(#name))) {                         \
      (name)->setHidden(!IsTrue(parser.value(QStringLiteral(#name)))); \
    }                                                                  \
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
  parser.addOption({QStringLiteral("main"),
                    QStringLiteral("“Main Toolbar”, visible yes/no"),
                    QStringLiteral("main"), QStringLiteral("no")});
  parser.addOption({QStringLiteral("session"),
                    QStringLiteral("“Session Toolbar”, visible yes/no"),
                    QStringLiteral("session"), QStringLiteral("no")});
  parser.addOption(
      {QStringLiteral("qc"),
       QStringLiteral("“Show Quick Commands” (dock), visible yes/no"),
       QStringLiteral("qc"), QStringLiteral("no")});
  parser.addOption({QStringLiteral("ssh"),
                    QStringLiteral("“Show SSH Manager” (dock), visible yes/no"),
                    QStringLiteral("ssh"), QStringLiteral("no")});
  parser.setApplicationDescription(
      QStringLiteral("Generator of Konsole config State"));
  parser.addHelpOption();
  parser.process(app);

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

  QString out;
  {
    QTemporaryFile tmp;
    if (!tmp.open()) return 1;
    {
      auto cg = KSharedConfig::openConfig(tmp.fileName(),
                                          KConfig::OpenFlag::SimpleConfig)
                    ->group("X");
      SET(main);
      SET(session);
      SET(ssh);
      SET(qc);
      w.saveMainWindowSettings(cg);
      cg.sync();
    }
    tmp.seek(0);
    QSettings settings(tmp.fileName(), QSettings::Format::IniFormat);
    settings.beginGroup(QStringLiteral("X"));
    out = settings.value(QStringLiteral("State")).toString();
  }
  std::cout << out.toStdString() << "\n";
  return 0;
}
