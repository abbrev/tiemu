#ifndef __DBG_ALL_H__
#define __DBG_ALL_H__

typedef struct {
    GtkWidget *mem;
    GtkWidget *regs;
    GtkWidget *bkpts;
    GtkWidget *code;
    GtkWidget *pclog;
} DbgWidgets;

void preload_gtk_debugger(void);
int enter_gtk_debugger(int context);

void set_other_windows_sensitivity(int state);

#endif
