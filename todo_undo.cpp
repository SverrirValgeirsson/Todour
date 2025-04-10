#include "todo_undo.h"


AddCommand::AddCommand(task _t, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    static int itemCount = 0;

//    myDiagramItem = new DiagramItem(addType);
//    initialPosition = QPointF((itemCount * 15) % int(scene->width()),
//                              (itemCount * 15) % int(scene->height()));
//    scene->update();
    ++itemCount;
    setText("Text");
    _task = _t;
//        .arg(createCommandString(myDiagramItem, initialPosition)));
}


void AddCommand::undo()
{
//    myGraphicsScene->removeItem(myDiagramItem);
//    myGraphicsScene->update();
}

void AddCommand::redo()
{
//    myGraphicsScene->addItem(myDiagramItem);
//    myDiagramItem->setPos(initialPosition);
//    myGraphicsScene->clearSelection();
//    myGraphicsScene->update();
}



DeleteCommand::DeleteCommand(task _t, QUndoCommand *parent)
    : QUndoCommand(parent)
{
//    QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
//    list.first()->setSelected(false);
//    myDiagramItem = static_cast<DiagramItem *>(list.first());
//    setText(QObject::tr("Delete %1")
//        .arg(createCommandString(myDiagramItem, myDiagramItem->pos())));
	_task = _t;
}

void DeleteCommand::undo()
{
//    myGraphicsScene->addItem(myDiagramItem);
//    myGraphicsScene->update();
}

void DeleteCommand::redo()
{
//    myGraphicsScene->removeItem(myDiagramItem);
}



EditCommand::EditCommand(task _to, task _tn, QUndoCommand *parent)
    : QUndoCommand(parent)
{
//    QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
//    list.first()->setSelected(false);
//    myDiagramItem = static_cast<DiagramItem *>(list.first());
//    setText(QObject::tr("Delete %1")
//        .arg(createCommandString(myDiagramItem, myDiagramItem->pos())));

	_old = _to;
	_new = _tn;

}

void EditCommand::undo()
{
//    myGraphicsScene->addItem(myDiagramItem);
//    myGraphicsScene->update();
}

void EditCommand::redo()
{
//    myGraphicsScene->removeItem(myDiagramItem);
}
