/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "moonphasewindow.h"

static vector<MoonPhase*> moonPhases;
static guint yearNumber;

MoonPhaseWindow::MoonPhaseWindow()
: m_VBox(Gtk::ORIENTATION_VERTICAL, 5),
m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL),
m_Label(),
m_Text(),
m_ScrolledWindow(),
m_HBoxBottom(Gtk::ORIENTATION_HORIZONTAL),
m_SaveButtonBox(Gtk::ORIENTATION_HORIZONTAL),
m_ButtonSave(_("Save"), true),
m_ButtonBox(Gtk::ORIENTATION_HORIZONTAL),
m_ButtonQuit(_("Quit"), true),
m_StatusFrame(),
m_Statusbar()
{
    set_title(_("Gtk::MoonPhaseView"));
    set_border_width(5);
    set_default_size(640, 640);

    // fill vertical box
    m_Label.set_markup(_("<b>Year number: </b>"));
    m_Label.set_vexpand(false);
    m_Label.set_hexpand(false);
    m_Label.set_halign(Gtk::ALIGN_START);
    m_Text.set_vexpand(false);
    m_Text.set_hexpand(false);
    m_Text.set_halign(Gtk::ALIGN_END);
    m_Text.set_text("2020");
    // Label, then TextEntry left to right in horizontal row
    m_HBoxTop.pack_start(m_Label, Gtk::PACK_SHRINK, 16);
    m_HBoxTop.pack_start(m_Text, Gtk::PACK_SHRINK);
    m_Text.signal_activate().connect(sigc::mem_fun(*this, &MoonPhaseWindow::on_activated));
    m_Text.signal_changed().connect(sigc::mem_fun(*this, &MoonPhaseWindow::on_entry_insert));
    m_VBox.pack_start(m_HBoxTop, Gtk::PACK_SHRINK);

    // Add the TreeView, inside a ScrolledWindow, with the button underneath:
    m_ScrolledWindow.add(m_TreeView);
    // Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_VBox.pack_start(m_ScrolledWindow);
    m_VBox.set_border_width(8);
    // SaveButtonBox to the left end of vertical Box
    m_HBoxBottom.pack_start(m_SaveButtonBox, Gtk::PACK_SHRINK);
    m_SaveButtonBox.pack_start(m_ButtonSave, Gtk::PACK_SHRINK);
    m_SaveButtonBox.set_border_width(2);
    m_SaveButtonBox.set_layout(Gtk::BUTTONBOX_START);
    gpointer yearPtr = GUINT_TO_POINTER(yearNumber);
    m_ButtonSave.signal_clicked().connect(sigc::bind<guint*>(sigc::mem_fun(*this,
            &MoonPhaseWindow::save_moonphases_clicked), &yearNumber));
    // ButtonBox to the right end of vertical box
    m_HBoxBottom.pack_end(m_ButtonBox, Gtk::PACK_SHRINK);
    m_HBoxBottom.set_border_width(8);
    m_ButtonBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK);
    m_ButtonBox.set_border_width(2);
    m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
    m_ButtonQuit.signal_clicked().connect(sigc::mem_fun(*this,
            &MoonPhaseWindow::on_button_quit));
    m_VBox.pack_start(m_HBoxBottom, Gtk::PACK_SHRINK);

    // Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_TreeView.set_model(m_refTreeModel);

    //Add the TreeView's view columns:
    //This number will be shown with the default numeric formatting.
    m_TreeView.append_column("ID", m_Columns.m_col_id);
    // 13 digits plus decimal point
    m_TreeView.append_column(_("Begin of lunar phase"), m_Columns.m_date_string);
    m_TreeView.append_column(_("Name of lunar phase"), m_Columns.m_name_string);

    //Make all the columns reorderable and resizable:
    //This is not necessary, but it's nice to show the feature.
    //You can use TreeView::set_column_drag_function() to more
    //finely control column drag and drop.
    for (guint i = 0; i < 3; i++) {
        auto column = m_TreeView.get_column(i);
        column->set_reorderable();
        column->set_resizable(true);
        //column->set_alignment(Gtk::ALIGN_FILL);
    }
    m_Statusbar.set_halign(Gtk::ALIGN_CENTER);
    m_StatusFrame.add(m_Statusbar);
    m_VBox.pack_end(m_StatusFrame, Gtk::PACK_SHRINK);
    m_ContextId = m_Statusbar.push(_("Insert number for year, press Enter"), m_ContextId);

    //Add the vertical Box to the window.
    add(m_VBox);

    show_all_children();
}

MoonPhaseWindow::~MoonPhaseWindow() {
}

// read yearnumber from entry
guint MoonPhaseWindow::get_yearnumber() {
    guint year = 0;
    // This code converts from string to number safely.
    stringstream numberStream(m_Text.get_text());
    if (numberStream >> year)
        return year;
    m_ContextId = m_Statusbar.push(_("Invalid number, please try again"), m_ContextId);
    return 0;
}

void MoonPhaseWindow::on_activated() {
    double julianDay = 0.0;
    int year = get_yearnumber();
    if (year != yearNumber) {
        if (moonPhases.size() > 0) {
            moonPhases.clear();
            m_refTreeModel->clear();
        }
        yearNumber = year;
        if (yearNumber < 1970) {
            cout << _("Check input: year ") << yearNumber << _(" should be greater than 1970") << endl;
            yearNumber = 1970;
        }
        stringstream yearBegin;
        std::tm date;
        yearBegin << yearNumber << "-01-01T00:00:00";
        yearBegin >> get_time(&date, "%Y-%m-%dT%H:%M:%S");
        // subtract local timezone offset to get UTC
        time_t startSeconds = mktime(&date) - timezone;
        //cout << "Sekunden: " << startTime << endl;
        stringstream yearEnd;
        yearEnd << yearNumber << "-12-31T23:59:59";
        yearEnd >> get_time(&date, "%Y-%m-%dT%H:%M:%S");
        time_t endSeconds = mktime(&date);
        //cout << "Endsekunden: " << endTime << endl;
        // julian day (days since January 1, 4713 BC without a year 0)
        // 2440587.5 julian day for January 1, 1970 (unix epoch date)
        julianDay = 2440587.5 + startSeconds / 86400;
        //cout << "julianisches Datum: " << fixed << julianDay << endl;
        // calculate time of lunations in year from 0...13 and Phases of 0...3 
        for (int l = 0; l < 14; l++) {
            for (int p = 0; p < 4; p++) {
                MoonPhase *nextPhase = new MoonPhase(l, p);
                nextPhase->computeEventTimeAfterJulianDay(julianDay - 29.0);
                time_t eventSeconds = (nextPhase->eventTime - 2440587.5) * 86400;
                if (((eventSeconds - startSeconds) < 0) || ((eventSeconds - endSeconds) > 0)) {
                    // event is in previous or next year
                    delete nextPhase;
                    continue;
                }
                moonPhases.push_back(nextPhase);
            }
        }
        // Fill the TreeView's model
        // Add table row for every moonphase object
        for (guint i = 0; i < moonPhases.size(); ++i) {
            Gtk::TreeModel::Row row = *(m_refTreeModel->append());
            row[m_Columns.m_col_id] = i;
            time_t eventSeconds = (moonPhases[i]->eventTime - 2440587.5) * 86400;
            std::tm* dateInfo = localtime(&eventSeconds);
            stringstream dateStream;
            dateStream.imbue(std::locale("de_DE.utf8"));
            dateStream << std::put_time(dateInfo, "%a %d %b %Y %H:%M:%S");
//            cout << std::put_time(gmtime(&eventSeconds), "%Y-%m-%dT%H:%M:%SZ") << endl;
            //Glib::ustring time = std::asctime(gmtime(&eventSeconds));
            //Glib::ustring text = Glib::ustring::format(std::put_time(gmtime(&eventSeconds), "%Y-%m-%dT%H:%M:%SZ"));
            row[m_Columns.m_date_string] = dateStream.str();
            row[m_Columns.m_name_string] = moonPhases[i]->eventName;
        }
        stringstream phaseStream;
        phaseStream << moonPhases.size() << _(" lunar phases calculated for year ") << yearNumber;
        m_ContextId = m_Statusbar.push(phaseStream.str(), m_ContextId);
    }
}

void MoonPhaseWindow::on_button_quit() {
    hide();
}

void MoonPhaseWindow::on_entry_insert() {
    //    cout << "entry changed, now " << m_Text.get_text() << endl;
}

void MoonPhaseWindow::save_moonphases_clicked(guint* year) {
    if (*year > 0) {
        const char* dateFormat = "%Y%m%dT%H%M%SZ";
        // prepare timestamp
        time_t time_now = time(0);
        stringstream timeStream;
        timeStream << put_time(localtime(&time_now), dateFormat);
        string timeStamp = timeStream.str();
        stringstream filename;
        filename << "Mondphasen-" << *year << ".ics";
        // cout << filename.str() << endl;
        ofstream calendarFile;
        calendarFile.open(filename.str());
        calendarFile << "BEGIN:VCALENDAR\n";
        calendarFile << "METHOD:PUBLISH\n";
        calendarFile << "VERSION:2.0\n";
        calendarFile << "X-WR-CALNAME:Moon phases\n";
        calendarFile << "PRODID:-//Apple Inc.//iCal 5.0.0//DE\n";
        for (guint i = 0; i < moonPhases.size(); ++i) {
            // loop through lunar phases in year
            // eventSeconds relative to January 1, 1970
            time_t eventSeconds = (moonPhases[i]->eventTime - 2440587.5) * 86400;
            // Greenwich Mean Time
            std::tm* dateInfo = gmtime(&eventSeconds);
            stringstream dateStream;
            dateStream << std::put_time(dateInfo, dateFormat);
            calendarFile << "BEGIN:VEVENT\n";
            calendarFile << "CREATED: " << timeStamp << "\n";
            // add-on "-kuester-" can be changed at will
            calendarFile << "UID:" << timeStamp << "@kuester-" << i << "\n";
            calendarFile << "SUMMARY: " << moonPhases[i]->eventName << "\n";
            calendarFile << "DESCRIPTION:Moon phases\n";
            calendarFile << "X-MOZILLA-ALARM-DEFAULT-LENGTH:0\n";
            calendarFile << "X-MOZILLA-RECUR-DEFAULT-UNITS:0\n";
            calendarFile << "RRULE:0\n";
            calendarFile << "TRANSP:TRANSPARENT\n";
            calendarFile << "DTSTART;VALUE=DATE:" << dateStream.str() << "\n";
            calendarFile << "DTEND;VALUE=DATE:" << dateStream.str() << "\n";
            calendarFile << "DTSTAMP:" << timeStamp << "\n";
            calendarFile << "LAST-MODIFIED:" << timeStamp << "\n";
            calendarFile << "END:VEVENT\n";
        }
        calendarFile << "END:VCALENDAR\n";
        calendarFile.close();
        timeStream.clear();
        stringstream phaseStream;
        phaseStream << moonPhases.size() << _(" lunar phases stored for year ") << *year;
        m_ContextId = m_Statusbar.push(phaseStream.str(), m_ContextId);
    } else
        m_ContextId = m_Statusbar.push(_("No data for calendar available"), m_ContextId);
}
