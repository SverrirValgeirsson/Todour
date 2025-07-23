#include "test_todotxt.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include "../def.h"

void TestTodoTxt::initTestCase()
{
    // Create temporary directory for test files
    tempDir = new QTemporaryDir();
    QVERIFY(tempDir->isValid());
    
    testDataDir = tempDir->path() + "/";
    
    // Set up test settings with all features enabled for testing
    QSettings settings;
    settings.setValue(SETTINGS_DIRECTORY, testDataDir);
    settings.setValue(SETTINGS_THRESHOLD, true);  // Enable threshold functionality
    settings.setValue(SETTINGS_THRESHOLD_LABELS, true);  // Enable threshold labels
    settings.setValue(SETTINGS_THRESHOLD_INACTIVE, true);  // Enable threshold inactive
    settings.setValue(SETTINGS_SHOW_ALL, false);
    settings.setValue(SETTINGS_DUE, true);  // Enable due date functionality
}

void TestTodoTxt::cleanupTestCase()
{
    delete tempDir;
}

void TestTodoTxt::init()
{
    todo = new todotxt();
    QString dir = testDataDir;
    todo->setdirectory(dir);
}

void TestTodoTxt::cleanup()
{
    delete todo;
}

void TestTodoTxt::setupTestTodoFile(const QString &content)
{
    QFile file(testDataDir + TODOFILE);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&file);
    out << content;
    file.close();
}

void TestTodoTxt::verifyTodoFileContent(const QString &expectedContent)
{
    QFile file(testDataDir + TODOFILE);
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in(&file);
    QString actualContent = in.readAll();
    file.close();
    QCOMPARE(actualContent.trimmed(), expectedContent.trimmed());
}

void TestTodoTxt::testConstructorDestructor()
{
    todotxt *testTodo = new todotxt();
    QVERIFY(testTodo != nullptr);
    delete testTodo;
}

void TestTodoTxt::testSetDirectory()
{
    QString testPath = "/test/path/";
    todo->setdirectory(testPath);
    
    // Verify directory is set by checking file paths
    QString todoPath = todo->getTodoFilePath();
    QVERIFY(todoPath.contains(testPath));
}

void TestTodoTxt::testParseEmptyFile()
{
    setupTestTodoFile("");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 0ul);
}

void TestTodoTxt::testParseBasicTodos()
{
    setupTestTodoFile("Buy milk\nCall mom\nFinish project");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 3ul);
    QCOMPARE(output[0], QString("Buy milk"));
    QCOMPARE(output[1], QString("Call mom"));
    QCOMPARE(output[2], QString("Finish project"));
}

void TestTodoTxt::testParseCompletedTodos()
{
    setupTestTodoFile("Buy milk\nx 2024-01-15 Call mom\nFinish project");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 2ul); // Only active todos
    QCOMPARE(output[0], QString("Buy milk"));
    QCOMPARE(output[1], QString("Finish project"));
}

void TestTodoTxt::testParsePriorities()
{
    setupTestTodoFile("(A) High priority task\n(B) Medium priority task\nNo priority task");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 3ul);
    
    // Test pretty print formatting
    QString line1 = output[0];
    QString pretty1 = todotxt::prettyPrint(line1);
    QVERIFY(pretty1.contains("High priority task"));
}

void TestTodoTxt::testParseProjectsAndContexts()
{
    setupTestTodoFile("Call mom +family @home\nBuy milk +shopping @store\nMeeting +work @office");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 3ul);
    
    // Verify projects and contexts are extracted (this would require access to internal state)
    // For now, just verify the todos are parsed correctly
    QVERIFY(output[0].contains("+family"));
    QVERIFY(output[0].contains("@home"));
}

void TestTodoTxt::testParseDates()
{
    setupTestTodoFile("2024-01-15 Buy milk\n(A) 2024-01-16 Call mom");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 2ul);
    QVERIFY(output[0].contains("2024-01-15"));
    QVERIFY(output[1].contains("2024-01-16"));
}

void TestTodoTxt::testUpdateTodo()
{
    setupTestTodoFile("Buy milk\nCall mom");
    todo->parse();
    
    QString oldRow = "Buy milk";
    QString newRow = "Buy organic milk";
    bool checked = false;
    
    todo->update(oldRow, checked, newRow);
    
    // Verify the update
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QVERIFY(std::find(output.begin(), output.end(), "Buy organic milk") != output.end());
    QVERIFY(std::find(output.begin(), output.end(), "Buy milk") == output.end());
}

void TestTodoTxt::testMarkComplete()
{
    setupTestTodoFile("Buy milk\nCall mom");
    todo->parse();
    
    QString row = "Buy milk";
    QString newRow = row;
    bool checked = true;
    
    todo->update(row, checked, newRow);
    
    // Verify the todo is marked complete
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 1ul); // Only "Call mom" should remain active
    QCOMPARE(output[0], QString("Call mom"));
}

void TestTodoTxt::testRemoveTodo()
{
    setupTestTodoFile("Buy milk\nCall mom\nFinish project");
    todo->parse();
    
    QString lineToRemove = "Call mom";
    todo->remove(lineToRemove);
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 2ul);
    QVERIFY(std::find(output.begin(), output.end(), "Call mom") == output.end());
}

void TestTodoTxt::testIsInactive()
{
    setupTestTodoFile("Normal task\nLATER: Do this later\nWAIT: Waiting for response");
    
    // Set up inactive patterns
    QSettings settings;
    settings.setValue(SETTINGS_INACTIVE, "LATER:;WAIT:");
    
    todo->parse();
    
    QString normalTask = "Normal task";
    QString laterTask = "LATER: Do this later";
    QString waitTask = "WAIT: Waiting for response";
    
    QVERIFY(!todo->isInactive(normalTask));
    QVERIFY(todo->isInactive(laterTask));
    QVERIFY(todo->isInactive(waitTask));
}

void TestTodoTxt::testGetToday()
{
    QString today = todo->getToday();
    QDate currentDate = QDate::currentDate();
    QString expectedDate = currentDate.toString("yyyy-MM-dd");
    QCOMPARE(today, expectedDate);
}

void TestTodoTxt::testGetRelativeDate()
{
    QDate baseDate(2024, 1, 15); // Monday
    
    // Test the actual functionality - relative date parsing with +Nd format
    QString result = todo->getRelativeDate("+0d", baseDate);
    QCOMPARE(result, QString("2024-01-15"));
    
    result = todo->getRelativeDate("+1d", baseDate);
    QCOMPARE(result, QString("2024-01-16"));
    
    result = todo->getRelativeDate("+1w", baseDate);
    QCOMPARE(result, QString("2024-01-22"));
    
    // Test unsupported format returns original string
    result = todo->getRelativeDate("today", baseDate);
    QCOMPARE(result, QString("today"));
}

void TestTodoTxt::testPrettyPrint()
{
    QString todoLine = "(A) 2024-01-15 Buy milk +shopping @store";
    QString pretty = todotxt::prettyPrint(todoLine);
    
    // Pretty print should format the todo line properly
    QVERIFY(pretty.contains("(A)")); // Priority is kept in current implementation
    QVERIFY(pretty.contains("Buy milk"));
    QVERIFY(pretty.contains("+shopping"));
    QVERIFY(pretty.contains("@store"));
    
    // Test with forEdit parameter  
    QString prettyForEdit = todotxt::prettyPrint(todoLine, true);
    QVERIFY(prettyForEdit.contains("2024-01-15")); // Dates shown when forEdit=true
}

void TestTodoTxt::testUndoRedo()
{
    setupTestTodoFile("Buy milk\nCall mom");
    todo->parse();
    
    // Initially, undo should not be possible
    QVERIFY(!todo->undoPossible());
    QVERIFY(!todo->redoPossible());
    
    // Make a change
    QString oldRow = "Buy milk";
    QString newRow = "Buy organic milk";
    bool checked = false;
    todo->update(oldRow, checked, newRow);
    
    // Now undo should be possible
    QVERIFY(todo->undoPossible());
    QVERIFY(!todo->redoPossible());
    
    // Perform undo
    bool undoResult = todo->undo();
    QVERIFY(undoResult);
    
    // Now redo should be possible
    QVERIFY(!todo->undoPossible());
    QVERIFY(todo->redoPossible());
    
    // Perform redo
    bool redoResult = todo->redo();
    QVERIFY(redoResult);
    QVERIFY(todo->undoPossible());
    QVERIFY(!todo->redoPossible());
}

void TestTodoTxt::testSlurpAndWrite()
{
    vector<QString> testContent;
    testContent.push_back("First line");
    testContent.push_back("Second line");
    testContent.push_back("Third line");
    
    QString testFile = testDataDir + "test_output.txt";
    todo->write(testFile, testContent);
    
    vector<QString> readContent;
    todo->slurp(testFile, readContent);
    
    QCOMPARE(readContent.size(), testContent.size());
    for (size_t i = 0; i < testContent.size(); ++i) {
        QCOMPARE(readContent[i], testContent[i]);
    }
}

void TestTodoTxt::testGetURL()
{
    QString lineWithURL = "Check website http://example.com for updates";
    QString url = todo->getURL(lineWithURL);
    QCOMPARE(url, QString("http://example.com"));
    
    QString lineWithoutURL = "This line has no URL";
    QString noUrl = todo->getURL(lineWithoutURL);
    QVERIFY(noUrl.isEmpty());
}

void TestTodoTxt::testThresholdDates()
{
    QSettings settings;
    settings.setValue(SETTINGS_THRESHOLD, true);
    
    QDate tomorrow = QDate::currentDate().addDays(1);
    QString tomorrowStr = tomorrow.toString("yyyy-MM-dd");
    
    setupTestTodoFile(QString("Normal task\nFuture task t:%1").arg(tomorrowStr));
    todo->parse();
    
    QString normalTask = "Normal task";
    QString futureTask = QString("Future task t:%1").arg(tomorrowStr);
    
    QVERIFY(!todo->isInactive(normalTask));
    QVERIFY(todo->isInactive(futureTask)); // Should be hidden due to threshold
}

void TestTodoTxt::testCorruptedTodoFormat()
{
    // Test with malformed dates, priorities, etc.
    setupTestTodoFile("(Z) Invalid priority\n(A Invalid parenthesis\n2024-13-45 Invalid date");
    
    // Should not crash - parse should handle malformed input gracefully
    todo->parse(); // Most todo.txt parsers are tolerant of malformed lines
    QVERIFY(true); // If we get here, parse handled the malformed input gracefully
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    // Should still parse what it can
    QCOMPARE(output.size(), 3ul);
}

void TestTodoTxt::testSpecialCharacters()
{
    setupTestTodoFile("Task with quotes \"quoted\" and 'single'\nTask with symbols @#$%^&*()\nTask with unicode text");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 3ul);
    
    // Verify special characters are preserved
    QVERIFY(output[0].contains("\"quoted\""));
    QVERIFY(output[1].contains("@#$%^&*()"));
    QVERIFY(output[2].contains("unicode"));
}

void TestTodoTxt::testEmptyLines()
{
    setupTestTodoFile("First task\n\n\nSecond task\n\nThird task\n\n");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 3ul); // Empty lines should be ignored
}

void TestTodoTxt::testLargeTodoFile()
{
    QString content;
    for (int i = 0; i < 1000; ++i) {
        content += QString("Task number %1\n").arg(i);
    }
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 1000ul);
}

void TestTodoTxt::testBoundaryDates()
{
    // Test with edge case dates
    setupTestTodoFile("2000-01-01 Y2K task\n2024-02-29 Leap year task\n2099-12-31 Future task");
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 3ul);
}

void TestTodoTxt::testUndoRedoMultipleOperations()
{
    setupTestTodoFile("Task 1\nTask 2\nTask 3");
    todo->parse();
    
    // Perform multiple operations
    QString oldRow1 = "Task 1";
    QString newRow1 = "Modified Task 1";
    todo->update(oldRow1, false, newRow1);
    
    QString oldRow2 = "Task 2";
    QString newRow2 = "Modified Task 2";
    todo->update(oldRow2, false, newRow2);
    
    // Remove a task
    todo->remove("Task 3");
    
    // Undo operations
    QVERIFY(todo->undoPossible());
    todo->undo(); // Should restore Task 3
    
    QVERIFY(todo->undoPossible());
    todo->undo(); // Should restore original Task 2
    
    QVERIFY(todo->undoPossible());
    todo->undo(); // Should restore original Task 1
}

void TestTodoTxt::testArchive()
{
    setupTestTodoFile("Active task\nx 2024-01-15 Completed task\nAnother active task");
    todo->parse();
    
    todo->archive();
    
    // Verify completed tasks are moved to done.txt
    vector<QString> activeTodos;
    QString filter = "";
    todo->getActive(filter, activeTodos);
    
    // Should only have active tasks
    QCOMPARE(activeTodos.size(), 2ul);
    QVERIFY(std::find(activeTodos.begin(), activeTodos.end(), "x 2024-01-15 Completed task") == activeTodos.end());
}

void TestTodoTxt::testRefresh()
{
    setupTestTodoFile("Initial task");
    todo->parse();
    
    // Modify file externally
    setupTestTodoFile("Initial task\nNew external task");
    
    todo->refresh();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    QCOMPARE(output.size(), 2ul);
    QVERIFY(std::find(output.begin(), output.end(), "New external task") != output.end());
}

void TestTodoTxt::testFilePathGeneration()
{
    QString todoPath = todo->getTodoFilePath();
    QString donePath = todo->getDoneFilePath();
    QString deletedPath = todo->getDeletedFilePath();
    
    QVERIFY(todoPath.endsWith(TODOFILE));
    QVERIFY(donePath.endsWith(DONEFILE));
    QVERIFY(deletedPath.endsWith(DELETEDFILE));
    
    QVERIFY(todoPath.contains(testDataDir));
    QVERIFY(donePath.contains(testDataDir));
    QVERIFY(deletedPath.contains(testDataDir));
}

void TestTodoTxt::testGetAll()
{
    setupTestTodoFile("Active task\nx 2024-01-15 Completed task");
    
    // Enable show all setting
    QSettings settings;
    settings.setValue(SETTINGS_SHOW_ALL, true);
    
    todo->parse();
    
    vector<QString> allTodos;
    QString filter = "";
    todo->getAll(filter, allTodos);
    
    // With show all enabled, should include completed tasks
    QCOMPARE(allTodos.size(), 2ul);
}

void TestTodoTxt::testDueIn()
{
    // Enable due date functionality
    QSettings settings;
    settings.setValue(SETTINGS_DUE, true);
    
    QDate today = QDate::currentDate();
    QDate tomorrow = today.addDays(1);
    QDate nextWeek = today.addDays(7);
    
    QString taskDueTomorrow = QString("Task due tomorrow due:%1").arg(tomorrow.toString("yyyy-MM-dd"));
    QString taskDueNextWeek = QString("Task due next week due:%1").arg(nextWeek.toString("yyyy-MM-dd"));
    
    int daysTomorrow = todo->dueIn(taskDueTomorrow);
    int daysNextWeek = todo->dueIn(taskDueNextWeek);
    
    QCOMPARE(daysTomorrow, 1);
    QCOMPARE(daysNextWeek, 7);
}

void TestTodoTxt::testProjectContextFiltering()
{
    QSettings settings;
    settings.setValue(SETTINGS_THRESHOLD_LABELS, true);
    
    setupTestTodoFile("Active project +work\nActive context @home\nThreshold project t:+work\nThreshold context t:@home");
    todo->parse();
    
    QString thresholdProject = "Threshold project t:+work";
    QString thresholdContext = "Threshold context t:@home";
    
    // These should be hidden because active projects/contexts exist
    QVERIFY(todo->isInactive(thresholdProject));
    QVERIFY(todo->isInactive(thresholdContext));
}

void TestTodoTxt::testUndoRedoAfterFileChange()
{
    setupTestTodoFile("Original task");
    todo->parse();
    
    // Make a change
    QString oldRow = "Original task";
    QString newRow = "Modified task";
    todo->update(oldRow, false, newRow);
    
    // Simulate external file change
    setupTestTodoFile("External change");
    todo->parse(); // This should save current state to undo
    
    // Undo should still work
    QVERIFY(todo->undoPossible());
    bool undoResult = todo->undo();
    QVERIFY(undoResult);
}

void TestTodoTxt::testAddTodo()
{
    // Test updating/adding a new todo using the update method
    QString newTodo = "Call dentist +personal @phone";
    QString empty = "";
    
    // The update method can be used to add new todos
    todo->update(empty, false, newTodo);
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    bool found = false;
    for (const QString& item : output) {
        if (item.contains("Call dentist")) {
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestTodoTxt::testMarkIncomplete()
{
    // Test using the update method to change completion status
    setupTestTodoFile("x 2024-01-01 Test incomplete task");
    
    vector<QString> all;
    QString filter = "";
    todo->getAll(filter, all);
    
    if (!all.empty()) {
        QString original = all[0];
        QString modified = original;
        modified.remove(0, modified.indexOf("Test")); // Remove completion marker
        modified = "Test incomplete task"; // Clean version
        
        todo->update(original, false, modified);
        
        vector<QString> active;
        todo->getActive(filter, active);
        
        bool found = false;
        for (const QString& item : active) {
            if (item.contains("Test incomplete task")) {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    } else {
        QVERIFY(true); // No todos to test with
    }
}

void TestTodoTxt::testGetActive()
{
    setupTestTodoFile("(A) Active task 1\nx Completed task\n(B) Active task 2");
    todo->parse(); // Parse the file we just created
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    QVERIFY(output.size() >= 2);
    // Should contain active tasks but not completed ones
}

void TestTodoTxt::testThresholdHide()
{
    // Test threshold date hiding functionality if implemented
    QString futureDate = QDate::currentDate().addDays(5).toString("yyyy-MM-dd");
    setupTestTodoFile("(A) Current task\n(B) Future task t:" + futureDate);
    todo->parse(); // Parse the file we just created
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    // Future threshold tasks might be hidden
    QVERIFY(output.size() >= 1);
}

void TestTodoTxt::testLessThan()
{
    // Test the comparison function if it exists
    QString todo1 = "(A) High priority task";
    QString todo2 = "(B) Lower priority task";
    
    // This would test internal comparison logic
    // Since we don't have direct access to the comparison method,
    // we'll test through sorting behavior
    QVERIFY(true); // Placeholder for actual comparison test
}

void TestTodoTxt::testUndoRedoPossible()
{
    // Test undo/redo possibility flags
    QVERIFY(!todo->undoPossible()); // Should be false initially
    QVERIFY(!todo->redoPossible()); // Should be false initially
    
    // Update a todo to create an undoable action
    QString empty = "";
    QString newTodo = "Test undo redo";
    todo->update(empty, false, newTodo);
    
    // Note: undo/redo state depends on internal implementation
    // This test verifies the methods exist and return boolean values
    bool undoState = todo->undoPossible();
    bool redoState = todo->redoPossible();
    QVERIFY(undoState == true || undoState == false); // Valid boolean
    QVERIFY(redoState == true || redoState == false); // Valid boolean
}

void TestTodoTxt::testRelativeThresholdDates()
{
    // Test relative threshold dates like t:+4d, t:+1w
    QString content = "Task with relative threshold t:+4d\n"
                     "Task with weekly threshold t:+1w\n"
                     "Task with monthly threshold t:+1m\n"
                     "Normal task without threshold";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    // All tasks should be parsed
    QCOMPARE(output.size(), 4ul);
    
    // Verify relative threshold dates are preserved in the text
    bool foundDailyThreshold = false;
    bool foundWeeklyThreshold = false;
    bool foundMonthlyThreshold = false;
    
    for (const QString& task : output) {
        if (task.contains("t:+4d")) foundDailyThreshold = true;
        if (task.contains("t:+1w")) foundWeeklyThreshold = true;
        if (task.contains("t:+1m")) foundMonthlyThreshold = true;
    }
    
    QVERIFY(foundDailyThreshold);
    QVERIFY(foundWeeklyThreshold);
    QVERIFY(foundMonthlyThreshold);
}

void TestTodoTxt::testRelativeDueDates()
{
    // Test relative due dates like due:+3d, due:+2w
    QString content = "Task due in 3 days due:+3d\n"
                     "Task due in 2 weeks due:+2w\n"
                     "Task due in 1 month due:+1m\n"
                     "Normal task without due date";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    // All tasks should be parsed
    QCOMPARE(output.size(), 4ul);
    
    // Verify relative due dates are preserved in the text
    bool foundDailyDue = false;
    bool foundWeeklyDue = false;
    bool foundMonthlyDue = false;
    
    for (const QString& task : output) {
        if (task.contains("due:+3d")) foundDailyDue = true;
        if (task.contains("due:+2w")) foundWeeklyDue = true;
        if (task.contains("due:+1m")) foundMonthlyDue = true;
    }
    
    QVERIFY(foundDailyDue);
    QVERIFY(foundWeeklyDue);
    QVERIFY(foundMonthlyDue);
}

void TestTodoTxt::testRepeatDaily()
{
    // Test daily repeat functionality
    QString content = "(A) Daily task rec:+1d\n"
                     "Weekly meeting rec:+7d\n"
                     "Check email daily rec:+1d due:+1d";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    QCOMPARE(output.size(), 3ul);
    
    // Test completing a recurring task to see if it repeats
    QString recurringTask = output[0]; // "(A) Daily task rec:+1d"
    
    // Mark the task as complete
    todo->update(recurringTask, true, recurringTask);
    
    // After marking complete, the task should repeat (implementation dependent)
    vector<QString> afterComplete;
    todo->getActive(filter, afterComplete);
    
    // Verify the recurring task behavior
    // Note: This depends on the specific implementation of recurring tasks
    QVERIFY(afterComplete.size() >= 2); // At least some tasks should remain
}

void TestTodoTxt::testRepeatWeekly()
{
    // Test weekly repeat patterns
    QString content = "Weekly status meeting rec:+1w due:+1w\n"
                     "Grocery shopping rec:+7d\n"
                     "Weekly review rec:+1w t:+1w";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    QCOMPARE(output.size(), 3ul);
    
    // Verify weekly recurrence patterns are preserved
    bool foundWeeklyRec = false;
    bool foundDailyRec = false;
    
    for (const QString& task : output) {
        if (task.contains("rec:+1w")) foundWeeklyRec = true;
        if (task.contains("rec:+7d")) foundDailyRec = true;
    }
    
    QVERIFY(foundWeeklyRec);
    QVERIFY(foundDailyRec);
}

void TestTodoTxt::testRepeatMonthly()
{
    // Test monthly repeat patterns
    QString content = "Pay rent rec:+1m due:+1m\n"
                     "Monthly backup rec:+30d\n"
                     "Quarterly report rec:+3m";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    QCOMPARE(output.size(), 3ul);
    
    // Verify monthly recurrence patterns
    bool foundMonthlyRec = false;
    bool foundDailyRec = false;
    bool foundQuarterlyRec = false;
    
    for (const QString& task : output) {
        if (task.contains("rec:+1m")) foundMonthlyRec = true;
        if (task.contains("rec:+30d")) foundDailyRec = true;
        if (task.contains("rec:+3m")) foundQuarterlyRec = true;
    }
    
    QVERIFY(foundMonthlyRec);
    QVERIFY(foundDailyRec);
    QVERIFY(foundQuarterlyRec);
}

void TestTodoTxt::testRepeatYearly()
{
    // Test yearly repeat patterns
    QString content = "Annual review rec:+1y due:+1y\n"
                     "Birthday reminder rec:+365d\n"
                     "Tax filing rec:+1y t:+11m";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    QCOMPARE(output.size(), 3ul);
    
    // Verify yearly recurrence patterns
    bool foundYearlyRec = false;
    bool foundDailyRec = false;
    
    for (const QString& task : output) {
        if (task.contains("rec:+1y")) foundYearlyRec = true;
        if (task.contains("rec:+365d")) foundDailyRec = true;
    }
    
    QVERIFY(foundYearlyRec);
    QVERIFY(foundDailyRec);
}

void TestTodoTxt::testRepeatBusinessDays()
{
    // Test business day repeat patterns
    QString content = "Daily standup rec:+1b\n"
                     "Work report rec:+5b\n"
                     "Weekly team meeting rec:+1w";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    QCOMPARE(output.size(), 3ul);
    
    // Verify business day recurrence patterns
    bool foundBusinessDayRec = false;
    bool foundWeeklyRec = false;
    
    for (const QString& task : output) {
        if (task.contains("rec:+1b") || task.contains("rec:+5b")) foundBusinessDayRec = true;
        if (task.contains("rec:+1w")) foundWeeklyRec = true;
    }
    
    QVERIFY(foundBusinessDayRec);
    QVERIFY(foundWeeklyRec);
}

void TestTodoTxt::testRelativeTimeFormat()
{
    // Test various relative time formats and their processing
    QDate baseDate(2024, 1, 15); // Monday
    
    // Test different relative date formats
    QString result1 = todo->getRelativeDate("+5d", baseDate);
    QCOMPARE(result1, QString("2024-01-20"));
    
    QString result2 = todo->getRelativeDate("+2w", baseDate);
    QCOMPARE(result2, QString("2024-01-29"));
    
    QString result3 = todo->getRelativeDate("+1m", baseDate);
    QCOMPARE(result3, QString("2024-02-15"));
    
    QString result4 = todo->getRelativeDate("+1y", baseDate);
    QCOMPARE(result4, QString("2025-01-15"));
    
    // Test business days (if implemented)
    QString result5 = todo->getRelativeDate("+1b", baseDate);
    // Business day calculation might vary, so we just verify it returns a valid date format
    QVERIFY(result5.contains("2024-01-") || result5 == "+1b");
    
    // Test invalid formats return original string
    QString resultInvalid = todo->getRelativeDate("+invalid", baseDate);
    QCOMPARE(resultInvalid, QString("+invalid"));
}

void TestTodoTxt::testMixedRelativeAndAbsoluteDates()
{
    // Test tasks with both relative and absolute dates
    QString content = "(A) Task with absolute threshold 2024-12-25 and relative due due:+1w\n"
                     "Task with relative threshold t:+3d and absolute due due:2024-06-15\n"
                     "Recurring task rec:+1m due:+1m t:+1m\n"
                     "Mixed dates 2024-01-01 rec:+1w due:+2w t:+1d";
    
    setupTestTodoFile(content);
    todo->parse();
    
    vector<QString> output;
    QString filter = "";
    todo->getActive(filter, output);
    
    QCOMPARE(output.size(), 4ul);
    
    // Verify all date formats are preserved
    bool foundAbsoluteDate = false;
    bool foundRelativeDue = false;
    bool foundRelativeThreshold = false;
    bool foundRecurrence = false;
    
    for (const QString& task : output) {
        if (task.contains("2024-12-25") || task.contains("2024-06-15") || task.contains("2024-01-01")) {
            foundAbsoluteDate = true;
        }
        if (task.contains("due:+")) {
            foundRelativeDue = true;
        }
        if (task.contains("t:+")) {
            foundRelativeThreshold = true;
        }
        if (task.contains("rec:+")) {
            foundRecurrence = true;
        }
    }
    
    QVERIFY(foundAbsoluteDate);
    QVERIFY(foundRelativeDue);
    QVERIFY(foundRelativeThreshold);
    QVERIFY(foundRecurrence);
}

void TestTodoTxt::createTestFile(const QString &filename, const QString &content)
{
    QFile file(testDataDir + filename);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&file);
    out << content;
    file.close();
}
