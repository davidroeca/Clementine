/* This file is part of Clementine.
   Copyright 2014, David Sansome <me@davidsansome.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "playlistsaveoptionsdialog.h"

#include "ui_playlistsaveoptionsdialog.h"
#include "playlistparsers/parserbase.h"

#include <QSettings>
#include <QString>

const char* PlaylistSaveOptionsDialog::kSettingsGroup =
    "PlaylistSaveOptionsDialog";

PlaylistSaveOptionsDialog::PlaylistSaveOptionsDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::PlaylistSaveOptionsDialog) {
  ui->setupUi(this);

  ui->filePaths->addItem(tr("Automatic"), Playlist::Path_Automatic);
  ui->filePaths->addItem(tr("Relative"), Playlist::Path_Relative);
  ui->filePaths->addItem(tr("Absolute"), Playlist::Path_Absolute);
  ui->filePaths->addItem(tr("Custom"), Playlist::Path_Custom);
}

PlaylistSaveOptionsDialog::~PlaylistSaveOptionsDialog() { delete ui; }

void PlaylistSaveOptionsDialog::accept() {
  if (ui->remember_user_choice->isChecked()) {
    int choice = ui->filePaths->itemData(ui->filePaths->currentIndex()).toInt();
    // Path_Custom is not 3, but it is at index 3
    if (choice == 3) {
      choice = static_cast<int>(Playlist::Path_Custom);
    }
    QSettings s;
    s.beginGroup(Playlist::kSettingsGroup);
    s.setValue(Playlist::kPathType, choice);
    s.setValue(Playlist::kCustomRoot, choice);
  }

  QDialog::accept();
}

QString PlaylistSaveOptionsDialog::custom_root() const {
  // TODO: Define customRoot LineTextEdit in ui file and extract from there
  QString root = ui->customRoot->toPlainText().trimmed();
  if (!root.endsWith("/")) {
    root.append("/");
  }
  return root;
}

Playlist::Path PlaylistSaveOptionsDialog::path_type() const {
  int choice = ui->filePaths->itemData(ui->filePaths->currentIndex()).toInt();
  if (choice == 3) {
    return Playlist::Path_Custom;
  }
  return static_cast<Playlist::Path>(choice);
}
