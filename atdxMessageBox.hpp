#ifndef atdxMESSAGE_BOX_HPP__
#define atdxMESSAGE_BOX_HPP__

#include <QObject>
#include <QMessageBox>

//
// atdxMessageBox - wrap the Qt QMessageBox class to give a more platform
// neutral and functional interface and translation of buttons
//
//Created by Arvo, ES1JA 2020

class atdxMessageBox : public QMessageBox
{
Q_OBJECT;
 public:
  explicit atdxMessageBox (QWidget * parent = nullptr);
  explicit atdxMessageBox (Icon,  QString const& text, StandardButtons = NoButton
                       , QWidget * parent = nullptr
                       , Qt::WindowFlags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
         void translate_buttons();     
  static void about_message (QWidget * parent, QString const& text);
  static void about_Qt_message (QWidget * parent);
  static StandardButton information_message (QWidget * parent, QString const& title
                                             , QString const& text
                                             , QString const& informative = QString {}
                                             , QString const& detail = QString {}
                                             , StandardButtons buttons = Ok
                                             , StandardButton default_button = NoButton);
  static StandardButton query_message (QWidget * parent, QString const& title
                                       , QString const& text
                                       , QString const& informative = QString {}
                                       , QString const& detail = QString {}
                                       , StandardButtons buttons = Yes | No
                                       , StandardButton default_button = NoButton);
  static StandardButton warning_message (QWidget * parent, QString const& title
                                         , QString const& text
                                         , QString const& informative = QString {}
                                         , QString const& detail = QString {}
                                         , StandardButtons buttons = Ok
                                         , StandardButton default_button = NoButton);
  static StandardButton critical_message (QWidget * parent, QString const& title
                                          , QString const& text
                                          , QString const& informative = QString {}
                                          , QString const& detail = QString {}
                                          , StandardButtons buttons = Ok
                                          , StandardButton default_button = NoButton);
private:

  static StandardButton show_it (QWidget * parent, atdxMessageBox::Icon icon
                                       , QString const& title
                                       , QString const& text
                                       , QString const& informative
                                       , QString const& detail
                                       , atdxMessageBox::StandardButtons buttons
                                       , atdxMessageBox::StandardButton default_button);
 
  // hide the parent static functions so that users use our versions
  // above that are correctly branded and have better platform
  // independence
  using QMessageBox::about;
  using QMessageBox::aboutQt;
  using QMessageBox::information;
  using QMessageBox::question;
  using QMessageBox::warning;
  using QMessageBox::critical;
};

#endif
