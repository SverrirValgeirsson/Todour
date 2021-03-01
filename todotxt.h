/* The Todo.txt master class.
  It handles the todo.txt and parses out information from it as well as writing it back.
  Also done.txt is handled here if needed
  */

#ifndef TODOTXT_H
#define TODOTXT_H

#define TODOFILE "todo.txt"
#define DONEFILE "done.txt"
#define DELETEDFILE "deleted.txt"

#include <vector>
#include <set>
#include <QString>
#include <QDate>
#include <QTemporaryDir>

using namespace std;

class todotxt
{
protected:
    QString filedirectory;
    vector<QString> todo;
    vector<QString> done;
    set<QString> active_projects;
    set<QString> active_contexts;
    static bool lessThan(QString &,QString &);
    bool threshold_hide(QString &);
    QTemporaryDir *undoDir;

public:
    todotxt();
    ~todotxt();
    void setdirectory(QString &dir);
    void parse(); // Parses the files in the directory
    void getActive(QString& filter,vector<QString> &output);
    void getAll(QString& filter,vector<QString> &output);
    Qt::CheckState getState(QString& row);
    static QString prettyPrint(QString& row,bool forEdit=false);
    void update(QString& row,bool checked,QString& newrow);
    void write(QString& filename,vector<QString>&  content);
    void slurp(QString& filename,vector<QString>&  content);
    QString getURL(QString &line);
    void remove(QString line);
    void archive();
    void refresh();
    bool isInactive(QString& text);
    int  dueIn(QString& text);
    QString getToday();
    QString getTodoFilePath();
    QString getDoneFilePath();
    QString getDeletedFilePath();
    QString getRelativeDate(QString shortform,QDate d=QDate::currentDate());

    // Undo and Redo
public:
    bool undo();   // Go backwards in the undo buffer without adding to it
    bool redo();  // go forward in the undo buffor without adding to it
    bool undoPossible(); // Say if undo is possible or not
    bool redoPossible(); // Say if redo is possible or not

protected:
    QString getUndoDir(); // get the directory where we save undo stuff
    QString getNewUndoNameDirAndPrefix(); // get a new prefix to be used for creating new undo files
    void    cleanupUndoDir(); // Remove old files in the undo directory (not accessed for a while?)
    void    saveToUndo();      // Adds the current changes to the undo buffer. Also moves the undo pointer to the last item (cementing whatever changes have been done with undoredo)
    bool    checkNeedForUndo();
    void    restoreFiles(QString);

    vector<QString> undoBuffer; // A buffer with base filenames for undos
    int undoPointer = 0; // Pointer into the undo buffer for undo and redo. Generally should be 0

    struct todoline{
        QString createdDate;
        QString closedDate;
        bool checked;
        QString priority;
        QString text; // The rest of the text
    };

    static void String2Todo(QString &line,todoline &t);
    static QString Todo2String(todoline &t);


};

#endif // TODOTXT_H
