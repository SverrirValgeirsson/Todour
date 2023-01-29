# Commands

The different qualifiers available in todour are:


|Qualifyer          | "Standard"  | Description               |
|-------------------|-------------|---------------------------|
|due:YYYY-MM-DD     | Y           | Sets a due date for a task              |
|due:+Xd            | N           | Creates a due date X days in the future |
|due:+Xw            | N           | Creates a due date X weeks in the future |
|due:+Xm            | N           | Creates a due date X months in the future |
|due:+Xb            | N           | Creates a due date X business days in the future |
|due:+Xy            | N           | Creates a due date X years in the future |
|t:YYYY-MM-DD       | Y           | Threshold. Hides a task until the date set |
|t:+Xd              | N           | Set a threshold X days in the future |
|t:+Xw              | N           | Set a threshold X weeks in the future |
|t:+Xm              | N           | Set a threshold X months in the future |
|t:+Xb              | N           | Set a threshold X business days in the future |
|t:+Xy              | N           | Set a threshold X years in the future |
|t:+project         | N           | Threshold. Hide this line until all lines with +project are done |
|t:@context         | N           | Threshold. Hide this line until all lines with @context are done |
|rec:Xd             | N           | Recurrence. To use together with due:YYYY-MM-dd and/or t:YYYY-MM-dd.  When you close this task, create a copy with due and or threshold set X days in the future from today (depending on settings). If there is no due or t, one will be added (depending on settings) |
|rec:Xw             | N           | Recurrence. Same as rec:Xd but for weeks |
|rec:Xm             | N           | Recurrence. Same as rec:Xd but for months |
|rec:Xy             | N           | Recurrence. Same as rec:Xd but for years |
|rec:Xb             | N           | Recurrence. Same as rec:Xd but for business days |
|rec:+Xd            | N           | Recurrence. To use together with due:YYYY-MM-dd and/or t:YYYY-MM-dd.  When you close this task, create a copy with due and or threshold set X days in the future from the date in t: and due: (depending on settings) |
|rec:+Xw             | N           | Recurrence. Same as rec:+Xd but for weeks |
|rec:+Xm             | N           | Recurrence. Same as rec:+Xd but for months |
|rec:+Xy             | N           | Recurrence. Same as rec:+Xd but for years |
|rec:+Xb             | N           | Recurrence. Same as rec:+Xd but for business days |


