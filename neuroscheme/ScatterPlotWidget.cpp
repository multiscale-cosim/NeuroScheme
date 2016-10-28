/*
 * Copyright (c) 2016 GMRV/URJC/UPM.
 *
 * Authors: Pablo Toharia <pablo.toharia@upm.es>
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
#include "Layout.h"
#include "ScatterPlotWidget.h"

namespace neuroscheme
{
  ScatterPlotWidget::ScatterPlotWidget( Layout* parentLayout_,
                                        QWidget* parent_ )
    : QFrame( parent_ )
    , _parentLayout( parentLayout_ )
  {
    auto layout_ = new QGridLayout;
    layout_->setAlignment( Qt::AlignTop );
    this->setLayout( layout_ );

    _xLabel = new QLabel( "X: " );
    _yLabel = new QLabel( "Y: " );

    _propertyXSelector = new QComboBox( );
    _propertyXSelector->setSizePolicy( QSizePolicy::Expanding,
                                        QSizePolicy::Expanding );
    _propertyXSelector->setMinimumSize( 190, 20 );

    _propertyYSelector = new QComboBox( );
    _propertyYSelector->setSizePolicy( QSizePolicy::Expanding,
                                        QSizePolicy::Expanding );
    _propertyYSelector->setMinimumSize( 190, 20 );

    layout_->addWidget( _xLabel, 0, 0, 1, 1,
                        Qt::AlignCenter | Qt::AlignLeft );
    layout_->addWidget( _propertyXSelector, 0, 1, 1, 1,
                        Qt::AlignCenter | Qt::AlignLeft );
    layout_->addWidget( _yLabel, 1, 0, 1, 1,
                        Qt::AlignCenter | Qt::AlignLeft );
    layout_->addWidget( _propertyYSelector, 1, 1, 1, 1,
                        Qt::AlignCenter | Qt::AlignLeft );

    connect( _propertyXSelector, SIGNAL( currentIndexChanged( int )),
             this, SLOT( _propertiesChanged( )));
    connect( _propertyYSelector, SIGNAL( currentIndexChanged( int )),
             this, SLOT( _propertiesChanged( )));

  }

  void ScatterPlotWidget::_propertiesChanged( void )
  {
    _parentLayout->refresh( true, false );
  }

  ScatterPlotWidget::~ScatterPlotWidget( void )
  {
    delete _propertyXSelector;
    delete _propertyYSelector;
    delete _xLabel;
    delete _yLabel;
  }

}
