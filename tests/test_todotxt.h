#ifndef TEST_TODOTXT_H
#define TEST_TODOTXT_H

#include <QObject>
#include <QTest>
#include <QTemporaryDir>
#include <QSettings>
#include "../todotxt.h"

class TestTodoTxt : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // Basic functionality tests
    void testConstructorDestructor();
    void testSetDirectory();
    void testParseEmptyFile();
    void testParseBasicTodos();
    void testParseCompletedTodos();
    void testParsePriorities();
    void testParseProjectsAndContexts();
    void testParseDates();
    
    // Todo manipulation tests
    void testAddTodo();
    void testUpdateTodo();
    void testMarkComplete();
    void testMarkIncomplete();
    void testRemoveTodo();
    void testArchive();
    
    // Filtering and searching tests
    void testGetActive();
    void testGetAll();
    void testIsInactive();
    void testThresholdHide();
    void testThresholdDates();
    void testProjectContextFiltering();
    
    // Date and priority handling
    void testPrettyPrint();
    void testDueIn();
    void testGetToday();
    void testGetRelativeDate();
    void testLessThan();
    
    // Relative time and repeat functionality
    void testRelativeThresholdDates();
    void testRelativeDueDates();
    void testRepeatDaily();
    void testRepeatWeekly();
    void testRepeatMonthly();
    void testRepeatYearly();
    void testRepeatBusinessDays();
    void testRelativeTimeFormat();
    void testMixedRelativeAndAbsoluteDates();
    
    // Undo/Redo functionality
    void testUndoRedo();
    void testUndoRedoPossible();
    void testUndoRedoMultipleOperations();
    void testUndoRedoAfterFileChange();
    
    // File operations
    void testSlurpAndWrite();
    void testFilePathGeneration();
    void testRefresh();
    
    // URL extraction
    void testGetURL();
    
    // Edge cases and error handling
    void testCorruptedTodoFormat();
    void testLargeTodoFile();
    void testSpecialCharacters();
    void testEmptyLines();
    void testBoundaryDates();

private:
    QTemporaryDir *tempDir;
    todotxt *todo;
    QString testDataDir;
    
    void setupTestTodoFile(const QString &content);
    void verifyTodoFileContent(const QString &expectedContent);
    void createTestFile(const QString &filename, const QString &content);
};

#endif // TEST_TODOTXT_H
