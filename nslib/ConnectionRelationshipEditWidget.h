/*
 * Copyright (c) 2017 GMRV/URJC/UPM.
 *
 * Authors: Juan Jose Garcia Cantero <juanjose.garcia@urjc.es>
*           Iago Calvo Lista <i.calvol@alumnos.urjc.es>
 *
 * This file is part of NeuroScheme
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef __NSLIB_CONNECTION_RELATIONSHIP_EDIT_WIDGET__
#define __NSLIB_CONNECTION_RELATIONSHIP_EDIT_WIDGET__

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <shift/shift.h>
#include <QDockWidget>
#include <QGridLayout>

namespace nslib
{
  class ConnectionRelationshipEditWidget:
    public QFrame
  {
    Q_OBJECT

  public:
    ConnectionRelationshipEditWidget( shift::Entity*  originEntity_,
      shift::Entity* destinationEntity_, QWidget* parentWidget_ = nullptr );
    ~ConnectionRelationshipEditWidget( void );
    static void parentDock( QDockWidget* parentDock_ );
    static QDockWidget* parentDock( void );

  public slots:
    void validateDialog( void );
    void cancelDialog( void );
    void breakDialog( void );
    void refreshSubproperties( void );
    void toggleAutoClose( void );

  private:
    typedef enum { COMBO, LINE_EDIT } TWidgetType;
    typedef enum { WIDGET_TYPE, LABEL, WIDGET } TEditTuple;

    shift::Entity* _originEntity;
    shift::Entity* _destinationEntity;
    std::vector< std::tuple< TWidgetType, QLabel*, QWidget* >> _propParamCont;
    shift::RelationshipProperties* _propObject;
    bool _isNew;
    std::unique_ptr< QLabel > _autoCloseLabel;
    std::unique_ptr< QCheckBox > _autoCloseCheck;
    std::unique_ptr< QPushButton > _validationButton;
    std::unique_ptr< QPushButton > _eraseButton;
    std::unique_ptr< QPushButton > _cancelButton;
    std::unique_ptr< QGridLayout > _gridLayout;

    static QDockWidget* _parentDock;
    static bool _autoCloseChecked;

    void checkClose( );
  };
}
#endif
