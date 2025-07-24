# Commands and Qualifiers

Todour supports the standard todo.txt format plus several powerful extensions. This page documents all available qualifiers and their usage.

## Date Qualifiers

### Due Dates (`due:`)
Set when a task should be completed.

| Qualifier | Standard | Description | Example |
|-----------|----------|-------------|---------|
| `due:YYYY-MM-DD` | ✅ | Absolute due date | `due:2024-03-15` |
| `due:+Xd` | ❌ | Due in X days | `due:+3d` |
| `due:+Xw` | ❌ | Due in X weeks | `due:+2w` |
| `due:+Xm` | ❌ | Due in X months | `due:+1m` |
| `due:+Xb` | ❌ | Due in X business days | `due:+5b` |
| `due:+Xy` | ❌ | Due in X years | `due:+1y` |
| `due:+Xp` | ❌ | Due in random 1-X days | `due:+7p` |

_NOTE_: The "Non standard" format will be translated to standard format by the application. Its for input convenience only and WILL NOT be respected if it's not input directly into Todour.

**Examples:**
```
Submit report due:2024-03-15 @work +quarterly
Call dentist due:+2d @phone
Pay bills due:+1w @finance +monthly
```

### Threshold Dates (`t:`)
Hide tasks until a specific date (tasks become "active" on this date).

| Qualifier | Standard | Description | Example |
|-----------|----------|-------------|---------|
| `t:YYYY-MM-DD` | ✅ | Absolute threshold date | `t:2024-03-01` |
| `t:+Xd` | ❌ | Threshold in X days | `t:+30d` |
| `t:+Xw` | ❌ | Threshold in X weeks | `t:+2w` |
| `t:+Xm` | ❌ | Threshold in X months | `t:+3m` |
| `t:+Xb` | ❌ | Threshold in X business days | `t:+10b` |
| `t:+Xy` | ❌ | Threshold in X years | `t:+1y` |
| `t:+Xp` | ❌ | Random threshold 1-X days | `t:+14p` |
| `t:+project` | ❌ | Wait for all +project tasks | `t:+website` |
| `t:@context` | ❌ | Wait for all @context tasks | `t:@calls` |

_NOTE_: The "Non standard" format will be translated to standard format by the application where possible. Its for input convenience only and WILL NOT be respected if it's not input directly into Todour.

**Examples:**
```
Plan vacation t:+30d +personal
Review performance t:2024-02-15 @work +annual
Start diet t:+project +health
Book hotel t:+travel @phone +vacation
```

## Recurrence (`rec:`)
Automatically recreate tasks when completed.

### Basic Recurrence
| Qualifier | Standard | Description | Example |
|-----------|----------|-------------|---------|
| `rec:Xd` | ❌ | Repeat every X days from today | `rec:1d` |
| `rec:Xw` | ❌ | Repeat every X weeks from today | `rec:1w` |
| `rec:Xm` | ❌ | Repeat every X months from today | `rec:1m` |
| `rec:Xy` | ❌ | Repeat every X years from today | `rec:1y` |
| `rec:Xb` | ❌ | Repeat every X business days | `rec:5b` |

### Strict Recurrence
| Qualifier | Standard | Description | Example |
|-----------|----------|-------------|---------|
| `rec:+Xd` | ❌ | Repeat X days from original due date | `rec:+7d` |
| `rec:+Xw` | ❌ | Repeat X weeks from original due date | `rec:+2w` |
| `rec:+Xm` | ❌ | Repeat X months from original due date | `rec:+1m` |
| `rec:+Xy` | ❌ | Repeat X years from original due date | `rec:+1y` |
| `rec:+Xb` | ❌ | Repeat X business days from original date | `rec:+5b` |
| `rec:+Xp` | ❌ | Repeat random 1-X days from original | `rec:+7p` |

**Examples:**
```
Daily standup due:2024-03-15 rec:1d @work +daily
Weekly review due:2024-03-17 rec:+1w @work +weekly
Monthly rent due:2024-04-01 rec:+1m @finance +bills
Take vitamins rec:1d @health +daily
```

## Standard todo.txt Qualifiers

### Priorities
| Format | Description | 
|--------|-------------|
| `(A)` | Highest priority |
| `(B)` | High priority | 
| `(C)` | Medium priority | 
| (none) | Normal priority | 

### Projects and Contexts
| Type | Format | Description | Example |
|------|--------|-------------|---------|
| Project | `+project` | Group related tasks | `+website`, `+vacation` |
| Context | `@context` | Where/how task is done | `@home`, `@phone`, `@work` |

### Dates (Standard)
| Format | Description | Example |
|--------|-------------|---------|
| `YYYY-MM-DD` | Creation date (beginning) | `2024-03-15 Call doctor` |
| `YYYY-MM-DD` | Completion date (after x) | `x 2024-03-15 2024-03-10 Task` |

## Complete Task Examples

### Simple Tasks
```
Call doctor @phone
Buy groceries @errands +shopping
(A) Submit quarterly report @work +important
```

### Tasks with Dates
```
(B) Review presentation due:2024-03-20 @work +quarterly
Plan vacation activities t:+30d +vacation @planning
Pay rent due:2024-04-01 rec:+1m @finance +bills
```

### Complex Tasks
```
(A) Weekly team meeting due:2024-03-18 rec:+1w t:2024-03-11 @work +weekly
Daily workout rec:1d @home +health +daily
Call insurance company due:+3d @phone +tasks t:+1d
Submit expenses due:2024-03-31 t:+project @work +quarterly
```

### Recurring Task Patterns
```
# Daily tasks
Take vitamins rec:1d @health +daily
Check email rec:1b @work +daily

# Weekly tasks  
Grocery shopping due:+6d rec:+1w @errands +weekly
Team meeting due:2024-03-20 rec:+1w @work +meetings

# Monthly tasks
Pay bills due:2024-04-01 rec:+1m @finance +monthly
Review budget due:+30d rec:+1m @finance +planning

# Quarterly tasks
Performance review due:2024-06-15 rec:+3m @work +quarterly
```

## Advanced Usage Patterns

### Project Dependencies
```
# Phase 1 tasks
Research competitors +website @research @phase1
Define requirements +website @planning @phase1

# Phase 2 tasks (wait for Phase 1)
Design mockups t:@phase1 @design +website @phase2
Write content t:@phase1 @writing +website @phase2

# Phase 3 tasks (wait for Phase 2)  
Build website t:@phase1 t:@phase2 @development +website
Test website t:@phase1 t:@phase2 @testing +website
```

### Context-Based Workflows
```
# Batch phone calls
Call dentist @phone +health
Call insurance @phone +insurance  
Call bank @phone +finance

# When all @phone tasks are done
Review call notes t:@phone @review +followup
```

### Time-Based Task Management
```
# Future planning
Plan summer vacation t:2024-05-01 +vacation @planning
Start tax preparation t:2024-02-01 +taxes @finance

# Regular maintenance
Change air filter due:+90d rec:+3m @home +maintenance
Backup computer due:+7d rec:+1w @computer +backup
```

## Best Practices

### Effective Task Writing
- **Be specific**: "Call Dr. Smith for annual checkup" vs "Call doctor"
- **Use contexts**: Group by where/how you'll do the task
- **Set realistic dates**: Don't over-schedule yourself
- **Use recurrence wisely**: For truly recurring tasks, not one-time events

### Organization Tips
- **Consistent naming**: Use standard project and context names
- **Meaningful priorities**: Reserve (A) for truly urgent items
- **Batch similar tasks**: Group @phone calls, @errands, etc.
- **Review regularly**: Use weekly reviews to plan threshold dates


