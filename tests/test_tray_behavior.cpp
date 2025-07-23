#include <QtTest>
#include <QApplication>
#include <QSettings>
#include "../mainwindow.h"
#include "../def.h"

class TestTrayBehavior : public QObject
{
    Q_OBJECT

private slots:
    void testQuitBehaviorWithTrayEnabled();
    void testQuitBehaviorWithTrayDisabled();
    void cleanup();

private:
    void clearSettings();
};

void TestTrayBehavior::clearSettings()
{
    QSettings settings;
    settings.clear();
}

void TestTrayBehavior::testQuitBehaviorWithTrayEnabled()
{
    clearSettings();
    
    // Set tray enabled in settings
    QSettings settings;
    settings.setValue(SETTINGS_TRAY_ENABLED, true);
    
    // Create main window which should call setTray()
    MainWindow window;
    
    // Check that quit on last window closed is disabled when tray is enabled
    QVERIFY(!QApplication::instance()->quitOnLastWindowClosed());
}

void TestTrayBehavior::testQuitBehaviorWithTrayDisabled()
{
    clearSettings();
    
    // Set tray disabled in settings  
    QSettings settings;
    settings.setValue(SETTINGS_TRAY_ENABLED, false);
    
    // Create main window which should call setTray()
    MainWindow window;
    
    // Check that quit on last window closed is enabled when tray is disabled
    QVERIFY(QApplication::instance()->quitOnLastWindowClosed());
}

void TestTrayBehavior::cleanup()
{
    clearSettings();
}

QTEST_MAIN(TestTrayBehavior)
#include "test_tray_behavior.moc"
