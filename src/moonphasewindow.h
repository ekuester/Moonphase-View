/* 
 * File:   moonphase.h
 * Author: kuestere
 *
 * Created on 29. Mai 2018, 16:53
 */

#ifndef MOONPHASEWINDOW_H
#define MOONPHASEWINDOW_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <gtkmm.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include "moonphase.h"

using namespace std;

class MoonPhase; // Say MoonPhase exists without defining it.

class MoonPhaseWindow : public Gtk::Window {
public:
    MoonPhaseWindow();
    virtual ~MoonPhaseWindow();
protected:
    guint get_yearnumber();
    //Signal handlers
    void on_activated();
    void on_entry_insert();
    void on_button_quit();
    void save_moonphases_clicked(guint* year);

    //Three model columns:
    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:

        ModelColumns() {
            add(m_col_id);
            add(m_date_string);
            add(m_name_string);
        }

        Gtk::TreeModelColumn<guint> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_date_string;
        Gtk::TreeModelColumn<Glib::ustring> m_name_string;
    };
    ModelColumns m_Columns;
 
    // Child widgets
    Gtk::Box m_HBoxTop;
    Gtk::Label m_Label;
    Gtk::Entry m_Text;
    
    Gtk::Box m_VBox;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::TreeView m_TreeView;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
    Gtk::Box m_HBoxBottom;
    Gtk::ButtonBox m_SaveButtonBox;
    Gtk::Button m_ButtonSave;
    Gtk::ButtonBox m_ButtonBox;
    Gtk::Button m_ButtonQuit;
    Gtk::Frame m_StatusFrame;
    unsigned m_ContextId;
    Gtk::Statusbar m_Statusbar;
};

#endif /* MOONPHASEWINDOW_H */

