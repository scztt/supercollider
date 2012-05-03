/************************************************************************
*
* Copyright 2010-2012 Jakob Leben (jakob.leben@gmail.com)
*
* This file is part of SuperCollider Qt GUI.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/

#include "../QcHelper.h"
#include <QPushButton>

class QcButton : public QPushButton, public QcHelper
{
  Q_OBJECT
  Q_PROPERTY( VariantList states READ dummyVariantList WRITE setStates );
  Q_PROPERTY( int value READ getValue WRITE setValue );
  public:
    QcButton();
  Q_SIGNALS:
    void action(int);
  protected:
#ifdef Q_WS_MAC
    bool hitButton( const QPoint & ) const;
#endif
  private Q_SLOTS:
    void doAction();
  private:
    struct State {
      QString text;
      QColor textColor;
      QColor buttonColor;
    };
    void setStates( const VariantList & );
    void setValue( int val ) { setState( val ); }
    int getValue() const { return currentState; }
    void setState( int );
    void cycleStates();
    QList<State> states;
    int currentState;
    QPalette defaultPalette;
};
