//
// atdxMessageBox - wrap the Qt QMessageBox class to give a more platform
// neutral and functional interface and translation of buttons
//
//Created by Arvo, ES1JA 2020

#include "atdxMessageBox.hpp"

#include <QDialogButtonBox>
#include <QPushButton>

#include "revision_utils.hpp"

atdxMessageBox::atdxMessageBox (QWidget * parent)
  : QMessageBox {parent}
{
  setWindowTitle ("atdx");
}

atdxMessageBox::atdxMessageBox (Icon icon, QString const& text, StandardButtons buttons
                        , QWidget * parent, Qt::WindowFlags flags)
  : QMessageBox {icon, "atdx", text, buttons, parent, flags}
{
}

void atdxMessageBox::about_message (QWidget * parent, QString const& text)
{
  QMessageBox::about (parent, "atdx", text);
}

void atdxMessageBox::about_Qt_message (QWidget * parent)
{
  QMessageBox::aboutQt (parent, "atdx");
}

void atdxMessageBox::translate_buttons()
{
    StandardButtons buttons = standardButtons();
    if (buttons & atdxMessageBox::Ok) button(atdxMessageBox::Ok)->setText(tr("&OK"));
    if (buttons & atdxMessageBox::Save) button(atdxMessageBox::Save)->setText(tr("Save"));
    if (buttons & atdxMessageBox::SaveAll) button(atdxMessageBox::SaveAll)->setText(tr("Save All"));
    if (buttons & atdxMessageBox::Open) button(atdxMessageBox::Open)->setText(tr("Open"));
    if (buttons & atdxMessageBox::Yes) button(atdxMessageBox::Yes)->setText(tr("&Yes"));
    if (buttons & atdxMessageBox::YesToAll) button(atdxMessageBox::YesToAll)->setText(tr("Yes to &All"));
    if (buttons & atdxMessageBox::No) button(atdxMessageBox::No)->setText(tr("&No"));
    if (buttons & atdxMessageBox::NoToAll) button(atdxMessageBox::NoToAll)->setText(tr("N&o to All"));
    if (buttons & atdxMessageBox::Abort) button(atdxMessageBox::Abort)->setText(tr("Abort"));
    if (buttons & atdxMessageBox::Retry) button(atdxMessageBox::Retry)->setText(tr("&Retry"));
    if (buttons & atdxMessageBox::Ignore) button(atdxMessageBox::Ignore)->setText(tr("Ignore"));
    if (buttons & atdxMessageBox::Close) button(atdxMessageBox::Close)->setText(tr("Close"));
    if (buttons & atdxMessageBox::Cancel) button(atdxMessageBox::Cancel)->setText(tr("&Cancel"));
    if (buttons & atdxMessageBox::Discard) button(atdxMessageBox::Discard)->setText(tr("Discard"));
    if (buttons & atdxMessageBox::Help) button(atdxMessageBox::Help)->setText(tr("Help"));
    if (buttons & atdxMessageBox::Apply) button(atdxMessageBox::Apply)->setText(tr("Apply"));
    if (buttons & atdxMessageBox::Reset) button(atdxMessageBox::Reset)->setText(tr("Reset"));
    if (buttons & atdxMessageBox::RestoreDefaults) button(atdxMessageBox::RestoreDefaults)->setText(tr("Restore Defaults"));
    if (buttons & atdxMessageBox::NoButton) button(atdxMessageBox::NoButton)->setText("");
}  

atdxMessageBox::StandardButton atdxMessageBox::show_it (QWidget * parent, atdxMessageBox::Icon icon
                                       , QString const& title
                                       , QString const& text
                                       , QString const& informative
                                       , QString const& detail
                                       , atdxMessageBox::StandardButtons buttons
                                       , atdxMessageBox::StandardButton default_button)
  {
    atdxMessageBox mb {icon, text, atdxMessageBox::NoButton, parent};
    if (!title.isEmpty()) mb.setWindowTitle(title);
    QDialogButtonBox * button_box = mb.findChild<QDialogButtonBox *> ();
    Q_ASSERT (button_box);

    uint mask = atdxMessageBox::FirstButton;
    while (mask <= atdxMessageBox::LastButton) {
      uint sb = buttons & mask;
      mask <<= 1;
      if (!sb)
        continue;
      QPushButton * button = mb.addButton (static_cast<atdxMessageBox::StandardButton> (sb));
      // Choose the first accept role as the default
      if (mb.defaultButton ())
        continue;
      if ((default_button == atdxMessageBox::NoButton
           && button_box->buttonRole (button) == QDialogButtonBox::AcceptRole)
          || (default_button != atdxMessageBox::NoButton
              && sb == static_cast<uint> (default_button)))
        mb.setDefaultButton (button);
    }
    mb.setInformativeText (informative);
    mb.setDetailedText (detail);
    QMessageBox::tr("Show Details...");
    QMessageBox::tr("Hide Details...");
    mb.translate_buttons();
    if (mb.exec() == -1)
      return atdxMessageBox::Cancel;
    return mb.standardButton (mb.clickedButton ());
  }

auto atdxMessageBox::information_message (QWidget * parent, QString const& title
                                      , QString const& text
                                      , QString const& informative
                                      , QString const& detail
                                      , StandardButtons buttons
                                      , StandardButton default_button) -> StandardButton
{
  return show_it (parent, Information, title, text, informative, detail, buttons, default_button);
}

auto atdxMessageBox::query_message (QWidget * parent, QString const& title
                                , QString const& text
                                , QString const& informative
                                , QString const& detail
                                , StandardButtons buttons
                                , StandardButton default_button) -> StandardButton
{
  return show_it (parent, Question, title, text, informative, detail, buttons, default_button);
}

auto atdxMessageBox::warning_message (QWidget * parent, QString const& title
                                  , QString const& text
                                  , QString const& informative
                                  , QString const& detail
                                  , StandardButtons buttons
                                  , StandardButton default_button) -> StandardButton
{
  return show_it (parent, Warning, title, text, informative, detail, buttons, default_button);
}

auto atdxMessageBox::critical_message (QWidget * parent, QString const& title
                                   , QString const& text
                                   , QString const& informative
                                   , QString const& detail
                                   , StandardButtons buttons
                                   , StandardButton default_button) -> StandardButton
{
  return show_it (parent, Critical, title, text, informative, detail, buttons, default_button);
}
