# General Usage

## Adding a Task

### Basic Task Creation
1. **Quick Add**: Type in the text field at the bottom and press Enter
2. **Global Hotkey**: Press `Ctrl+Alt+T` from anywhere to open quick add dialog (if enabled in the settings)
3. **Directly in file**: Edit the file in any editor and just add a line

### Task Format Examples
```
Simple task
(A) High priority task
Call doctor @phone
Buy milk @errands +groceries
(B) Review presentation due:2024-03-15 @work
Weekly meeting rec:1w due:2024-03-10 @work +weekly
```

### Priority Levels
- `(A)` - Highest priority 
- `(B)` - High priority  
- `(C)` - Medium priority
- No priority - Normal priority (default)

## Editing a Task

### Methods to Edit
1. **Double-click** on any task to edit it
2. **Select task** and press `F2`

### Inline Editing
- Click on any task to select it
- Start typing to replace the entire task
- Use arrow keys to position cursor for partial edits
- Press Enter to save, Escape to cancel

## Removing a Task

### Delete Methods
1. **Select and Delete**: Click task and press `Delete` key

### Mark as Complete
1. **Checkbox**: Click the checkbox next to the task
2. **Keyboard**: You can navigate to the checkbox and press `space` key

### Archive Completed Tasks
- Completed tasks can be moved to `done.txt` by pressing the archive button

## Filter

### Search and Filter Options
1. **Text Search**: Type in the search box to filter by any text
2. **Project Filter**: Type `+projectname` to show only that project
3. **Context Filter**: Type `@context` to show only that context

### Advanced Filtering
- **Combine filters**: Use multiple criteria (e.g., `+work @phone`)
- **Negative search**: Use `!` prefix to exclude (e.g., `!@work`) (multiple negations can be added after one another and they will be OR:ed together)
- **Hide completed**: Toggle showing/hiding completed tasks

### Filter Examples
```
@home              # Show only home context tasks
+groceries         # Show only grocery project tasks
due:2024-03       # Show tasks due in March 2024
!@work            # Hide all work tasks
(A)               # Show only A priority tasks
```

## Lock

### Context Lock Feature
- **Purpose**: Lock the current filter to add new tasks with same context/project
- **How to use**: Apply a filter, then click the Lock icon
- **Behavior**: All new tasks automatically get the locked context/project
- **Example**: Filter by `@work`, lock it, then all new tasks get `@work` added

### Benefits
- Consistent tagging when adding multiple related tasks
- Faster task entry for specific contexts
- Maintains focus on current work area
- By not locking, you can have a filter on `@work` but quickly add a `@home` task to the list but it will stay hidden, helping you to keep focus. 

## Undo

### Undo/Redo System
- **Undo**: `Ctrl+Z` or Edit → Undo
- **Redo**: `Ctrl+Y` or Edit → Redo
- **Multiple levels**: Full operation history maintained
- **File-based**: Undo survives application restart

### Undo Limitations
- External file changes (when file is modified outside Todour)
- Settings changes
- Filter changes

## Hotkeys

### Global Hotkeys
| Key Combination | Action |
|-----------------|--------|
| `Ctrl+Alt+T` | Quick add task dialog (works from any application) |

### Application Hotkeys
| Key Combination | Action |
|-----------------|--------|
| `Ctrl+Z` | Undo |
| `Ctrl+Y` | Redo |
| `Ctrl+F` | Focus search box |
| `F2` | Edit selected task |


### Navigation Hotkeys
| Key | Action |
|-----|--------|
| `↑/↓` | Navigate between tasks |
| `Home/End` | Go to first/last task |
| `Page Up/Down` | Scroll page up/down |
| `F2` | Edit selected task |
| `Space` | Toggle task completion (when checkbox is selected)|

### Customization
- Global hotkey can be changed in Settings
- Can be disabled if conflicts with other applications
- Some hotkeys may vary by operating system

