/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    dct_widgets.cpp
 *
 * @brief   Implementation of dct designer widgets 
 *
 *****************************************************************************/
#include "dct_widgets.h"

/******************************************************************************
 * class CustomWidgetInterface
 *
 *****************************************************************************/

/******************************************************************************
 * CustomWidgetInterface::CustomWidgetInterface
 *****************************************************************************/
CustomWidgetInterface::CustomWidgetInterface( QObject *parent )
    : QObject( parent )
    , initialized( false )
{
}

/******************************************************************************
 * CustomWidgetInterface::isContainer
 *****************************************************************************/
bool CustomWidgetInterface::isContainer() const
{
    return ( false );
}

/******************************************************************************
 * CustomWidgetInterface::isInitialized
 *****************************************************************************/
bool CustomWidgetInterface::isInitialized() const
{
    return ( initialized );
}

/******************************************************************************
 * CustomWidgetInterface::icon
 *****************************************************************************/
QIcon CustomWidgetInterface::icon() const
{
    return ( d_icon );
}

/******************************************************************************
 * CustomWidgetInterface::domXml
 *****************************************************************************/
QString CustomWidgetInterface::domXml() const
{
    return ( d_domXml );
}

/******************************************************************************
 * CustomWidgetInterface::group
 *****************************************************************************/
QString CustomWidgetInterface::group() const
{
    return ( DCT_BASIC_WIDGET_GROUP_NAME );
}

/******************************************************************************
 * CustomWidgetInterface::includeFile
 *****************************************************************************/
QString CustomWidgetInterface::includeFile() const
{
    return ( d_include );
}

/******************************************************************************
 * CustomWidgetInterface::name
 *****************************************************************************/
QString CustomWidgetInterface::name() const
{
    return ( d_name );
}

/******************************************************************************
 * CustomWidgetInterface::toolTip
 *****************************************************************************/
QString CustomWidgetInterface::toolTip() const
{
    return ( "" );
}

/******************************************************************************
 * CustomWidgetInterface::whatsThis
 *****************************************************************************/
QString CustomWidgetInterface::whatsThis() const
{
    return ( "" );
}

/******************************************************************************
 * CustomWidgetInterface::initialize
 *****************************************************************************/
void CustomWidgetInterface::initialize( QDesignerFormEditorInterface * )
{
    if ( initialized )
    {
        return;
    }
    initialized = true;
}



/******************************************************************************
 * class FmtSpinBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * FmtSpinBoxPlugin::FmtSpinBoxPlugin
 *****************************************************************************/
FmtSpinBoxPlugin::FmtSpinBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "FmtSpinBox";

    d_include       = "fmtspinbox.h";
    
    d_icon = QPixmap( "" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"FmtSpinBox\" name=\"FmtSpinBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>200</width>\n"
                       "    <height>200</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>FmtSpinBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>FmtSpinBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}

/******************************************************************************
 * FmtSpinBoxPlugin::createWidget
 *****************************************************************************/
QWidget * FmtSpinBoxPlugin::createWidget( QWidget * parent )
{
    return ( new FmtSpinBox( parent ) );
}



/******************************************************************************
 * class SkinnedDialPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * SkinnedDialPlugin::SkinnedDialPlugin
 *****************************************************************************/
SkinnedDialPlugin::SkinnedDialPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "SkinnedDial";

    d_include       = "skinneddial.h";
    
    d_icon = QPixmap( ":/designer/images/skinneddial.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"SkinnedDial\" name=\"SkinnedDial\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>200</width>\n"
                       "    <height>200</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>SkinnedDial</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>SkinnedDial</string>\n"
                       "  </property>\n"
                       "  <property name=\"backgroundImage\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/back/dial-back.png</pixmap>\n"
                       "  </property>\n"
                       "  <property name=\"needleImage\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/needle/dial-pointer.png</pixmap>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}

/******************************************************************************
 * SkinnedDialPlugin::createWidget
 *****************************************************************************/
QWidget * SkinnedDialPlugin::createWidget( QWidget * parent )
{
    return ( new SkinnedDial( parent ) );
}



/******************************************************************************
 * class SwitchButtonPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * SwitchButtonPlugin::SwitchButtonPlugin
 *****************************************************************************/
SwitchButtonPlugin::SwitchButtonPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "SwitchButton";

    d_include       = "switchbutton.h";
    
    d_icon = QPixmap( ":/designer/images/switchbutton.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"SwitchButton\" name=\"SwitchButton\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>60</width>\n"
                       "    <height>30</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>SkinnedDial</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>SkinnedDial</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}

/******************************************************************************
 * SwitchButtonPlugin::createWidget
 *****************************************************************************/
QWidget * SwitchButtonPlugin::createWidget( QWidget * parent )
{
    return ( new SwitchButton( parent ) );
}



/******************************************************************************
 * class SingleChannelKnobBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * SingleChannelKnobBoxPlugin::SingleChannelKnobBoxPlugin
 *****************************************************************************/
SingleChannelKnobBoxPlugin::SingleChannelKnobBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "SingleChannelKnobBox";

    d_include       = "singlechannelknobbox.h";
    
    d_icon = QPixmap( ":/designer/images/singlechannelknobbox.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"SingleChannelKnobBox\" name=\"SingleChannelKnobBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>250</width>\n"
                       "    <height>250</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>SingleChannelKnobBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>SingleChannelKnobBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"maxAngle\" >\n"
                       "   <number>150</number>\n"
                       "  </property>\n"
                       "  <property name=\"needleImage\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/needle/dial-pointer.png</pixmap>\n"
                       "  </property>\n"
                       "  <property name=\"backgroundImage\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/back/dial-back.png</pixmap>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * SingleChannelKnobBoxPlugin::createWidget
 *****************************************************************************/
QWidget * SingleChannelKnobBoxPlugin::createWidget( QWidget * parent )
{
    return ( new SingleChannelKnobBox( parent ) );
}



/******************************************************************************
 * class TrippleChannelSliderBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * TrippleChannelSliderBoxPlugin::TrippleChannelSliderBoxPlugin
 *****************************************************************************/
TrippleChannelSliderBoxPlugin::TrippleChannelSliderBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "TrippleChannelSliderBox";

    d_include       = "tripplechannelsliderbox.h";
    
    d_icon = QPixmap( ":/images/back/dial-back.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"TrippleChannelSliderBox\" name=\"TrippleChannelSliderBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>400</width>\n"
                       "    <height>200</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>TrippleChannelSliderBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>TrippleChannelSliderBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * TrippleChannelSliderBoxPlugin::createWidget
 *****************************************************************************/
QWidget * TrippleChannelSliderBoxPlugin::createWidget( QWidget * parent )
{
    return ( new TrippleChannelSliderBox( parent ) );
}



/******************************************************************************
 * class TrippleChannelKnobBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * TrippleChannelKnobBoxPlugin::TrippleChannelKnobBoxPlugin
 *****************************************************************************/
TrippleChannelKnobBoxPlugin::TrippleChannelKnobBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "TrippleChannelKnobBox";

    d_include       = "tripplechannelknobbox.h";
    
    d_icon = QPixmap( ":/images/back/dial-back.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"TrippleChannelKnobBox\" name=\"TrippleChannelKnobBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>750</width>\n"
                       "    <height>300</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>TrippleChannelKnobBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>TrippleChannelKnobBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"maxAngle\" >\n"
                       "   <number>150</number>\n"
                       "  </property>\n"
                       "  <property name=\"needleImage\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/needle/dial-pointer.png</pixmap>\n"
                       "  </property>\n"
                       "  <property name=\"backgroundImage0\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/back/dial-back-red.png</pixmap>\n"
                       "  </property>\n"
                       "  <property name=\"backgroundImage1\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/back/dial-back-green.png</pixmap>\n"
                       "  </property>\n"
                       "  <property name=\"backgroundImage2\" >\n"
                       "   <pixmap resource=\"ressource.qrc\">:/images/back/dial-back-blue.png</pixmap>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * TrippleChannelKnobBoxPlugin::createWidget
 *****************************************************************************/
QWidget * TrippleChannelKnobBoxPlugin::createWidget( QWidget * parent )
{
    return ( new TrippleChannelKnobBox( parent ) );
}



/******************************************************************************
 * class ButtonArrayBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * ButtonArrayBoxPlugin::ButtonArrayBoxPlugin
 *****************************************************************************/
ButtonArrayBoxPlugin::ButtonArrayBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "ButtonArrayBox";

    d_include       = "btnarraybox.h";
    
    d_icon = QPixmap( ":/images/back/dial-back.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"ButtonArrayBox\" name=\"ButtonArrayBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>400</width>\n"
                       "    <height>200</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>ButtonArrayBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>ButtonArrayBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"maxColumns\" >\n"
                       "   <number>3</number>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * ButtonArrayBoxPlugin::createWidget
 *****************************************************************************/
QWidget * ButtonArrayBoxPlugin::createWidget( QWidget * parent )
{
    return ( new ButtonArrayBox( parent ) );
}




/******************************************************************************
 * class PieSegmentSelectPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * PieSegmentSelectPlugin::PieSegmentSelectPlugin
 *****************************************************************************/
PieSegmentSelectPlugin::PieSegmentSelectPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "PieSegmentSelect";

    d_include       = "piesegmentselect.h";
    
    d_icon = QPixmap( ":/designer/images/piesegmentselect.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"PieSegmentSelect\" name=\"PieSegmentSelect\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>400</width>\n"
                       "    <height>400</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>PieSegmentSelect</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>PieSegmentSelect</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * PieSegmentSelectPlugin::createWidget
 *****************************************************************************/
QWidget * PieSegmentSelectPlugin::createWidget( QWidget * parent )
{
    return ( new PieSegmentSelect( parent ) );
}



/******************************************************************************
 * class HueSegmentSelectPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * HueSegmentSelectPlugin::HueSegmentSelectPlugin
 *****************************************************************************/
HueSegmentSelectPlugin::HueSegmentSelectPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "HueSegmentSelect";

    d_include       = "huesegmentselect.h";
    
    d_icon = QPixmap( ":/designer/images/huesegmentselect.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"HueSegmentSelect\" name=\"HueSegmentSelect\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>400</width>\n"
                       "    <height>400</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>HueSegmentSelect</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>HueSegmentSelect</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * HueSegmentSelectPlugin::createWidget
 *****************************************************************************/
QWidget * HueSegmentSelectPlugin::createWidget( QWidget * parent )
{
    return ( new HueSegmentSelect( parent ) );
}


/******************************************************************************
 * class MccSliderPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * MccSliderPlugin::MccSliderPlugin
 *****************************************************************************/
MccSliderPlugin::MccSliderPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "MccSlider";

    d_include       = "mccslider.h";

    d_icon = QPixmap( "" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"MccSlider\" name=\"MccSlider\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>30</width>\n"
                       "    <height>380</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>MccSlider</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>MccSlider</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * MccSliderPlugin::createWidget
 *****************************************************************************/
QWidget * MccSliderPlugin::createWidget( QWidget * parent )
{
    return ( new MccSlider( 0, "red", 0, 100, 0, 100, 0, 0, 0, 0, parent ) );
}


/******************************************************************************
 * class QCustomPlotPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * QCustomPlotPlugin::QCustomPlotPlugin
 *****************************************************************************/
QCustomPlotPlugin::QCustomPlotPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "QCustomPlot";

    d_include       = "qcustomplot.h";
    
    d_icon = QPixmap( ":/designer/images/huesegmentselect.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"QCustomPlot\" name=\"QCustomPlot\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>200</width>\n"
                       "    <height>200</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>QCustomPlot</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>QCustomPlot</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * QCustomPlotPlugin::createWidget
 *****************************************************************************/
QWidget * QCustomPlotPlugin::createWidget( QWidget * parent )
{
    return ( new QCustomPlot( parent ) );
}



/******************************************************************************
 * class WbBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * WbBoxPlugin::WbBoxPlugin
 *****************************************************************************/
WbBoxPlugin::WbBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "WbBox";

    d_include       = "wbbox.h";
    
    d_icon = QPixmap( ":/images/tab/thermometer.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"WbBox\" name=\"WbBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>WbBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>WbBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * WbBoxPlugin::createWidget
 *****************************************************************************/
QWidget * WbBoxPlugin::createWidget( QWidget * parent )
{
    return ( new WbBox( parent ) );
}

/******************************************************************************
 * WbBoxPlugin::group
 *****************************************************************************/
QString WbBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class MccBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * MccBoxPlugin::MccBoxPlugin
 *****************************************************************************/
MccBoxPlugin::MccBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "MccBox";

    d_include       = "mccbox.h";
    
    d_icon = QPixmap( ":/designer/images/piesegmentselect.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"MccBox\" name=\"MccBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>MccBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>MccBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * MccBoxPlugin::createWidget
 *****************************************************************************/
QWidget * MccBoxPlugin::createWidget( QWidget * parent )
{
    return ( new MccBox( parent ) );
}

/******************************************************************************
 * MccBoxPlugin::group
 *****************************************************************************/
QString MccBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class MccEqBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * MccEqBoxPlugin::MccEqBoxPlugin
 *****************************************************************************/
MccEqBoxPlugin::MccEqBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "MccEqBox";

    d_include       = "mcceqbox.h";

    d_icon = QPixmap( ":/designer/images/piesegmentselect.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"MccEqBox\" name=\"MccEqBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>MccEqBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>MccEqBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * MccEqBoxPlugin::createWidget
 *****************************************************************************/
QWidget * MccEqBoxPlugin::createWidget( QWidget * parent )
{
    return ( new MccEqBox( parent ) );
}

/******************************************************************************
 * MccEqBoxPlugin::group
 *****************************************************************************/
QString MccEqBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class InfoBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * InfoBoxPlugin::InfoBoxPlugin
 *****************************************************************************/
InfoBoxPlugin::InfoBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "InfoBox";

    d_include       = "infobox.h";
    
    d_icon = QPixmap( ":/images/tab/info.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"InfoBox\" name=\"InfoBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>InfoBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>InfoBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}

/******************************************************************************
 * InfoBoxPlugin::createWidget
 *****************************************************************************/
QWidget * InfoBoxPlugin::createWidget( QWidget * parent )
{
    return ( new InfoBox( parent ) );
}

/******************************************************************************
 * InfoBoxPlugin::group
 *****************************************************************************/
QString InfoBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}



/******************************************************************************
 * class BlackLevelBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * BlackLevelBoxPlugin::BlackLevelBoxPlugin
 *****************************************************************************/
BlackLevelBoxPlugin::BlackLevelBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "BlackLevelBox";

    d_include       = "blacklevelbox.h";
    
    d_icon = QPixmap( ":/images/tab/black.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"BlackLevelBox\" name=\"BlackLevelBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>BlackLevelBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>BlackLevelBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * BlackLevelBoxPlugin::createWidget
 *****************************************************************************/
QWidget * BlackLevelBoxPlugin::createWidget( QWidget * parent )
{
    return ( new BlackLevelBox( parent ) );
}

/******************************************************************************
 * BlackLevelBoxPlugin::group
 *****************************************************************************/
QString BlackLevelBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}

/******************************************************************************
 * class GammaBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * GammaBoxPlugin::GammaBoxPlugin
 *****************************************************************************/
GammaBoxPlugin::GammaBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "GammaBox";

    d_include       = "gammabox.h";
    
    d_icon = QPixmap( ":/images/tab/gamma.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"GammaBox\" name=\"GammaBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>GammaBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>GammaBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * GammaBoxPlugin::createWidget
 *****************************************************************************/
QWidget * GammaBoxPlugin::createWidget( QWidget * parent )
{
    return ( new GammaBox( parent ) );
}

/******************************************************************************
 * GammaBoxPlugin::group
 *****************************************************************************/
QString GammaBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}

/******************************************************************************
 * class LutBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * LutBoxPlugin::LutBoxPlugin
 *****************************************************************************/
LutBoxPlugin::LutBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "LutBox";

    d_include       = "lutbox.h";
    
    d_icon = QPixmap( ":/images/tab/gamma.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"LutBox\" name=\"LutBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>LutBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>LutBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * LutBoxPlugin::createWidget
 *****************************************************************************/
QWidget * LutBoxPlugin::createWidget( QWidget * parent )
{
    return ( new LutBox( parent ) );
}

/******************************************************************************
 * LutBoxPlugin::group
 *****************************************************************************/
QString LutBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}

/******************************************************************************
 * class InOutBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * InOutBoxPlugin::InOutBoxPlugin
 *****************************************************************************/
InOutBoxPlugin::InOutBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "InOutBox";

    d_include       = "inoutbox.h";
    
    d_icon = QPixmap( ":/images/tab/inout.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"InOutBox\" name=\"InOutBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>InOutBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>InOutBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * InOutBoxPlugin::createWidget
 *****************************************************************************/
QWidget * InOutBoxPlugin::createWidget( QWidget * parent )
{
    return ( new InOutBox( parent ) );
}

/******************************************************************************
 * InOutBoxPlugin::group
 *****************************************************************************/
QString InOutBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class OutBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * OutBoxPlugin::OutBoxPlugin
 *****************************************************************************/
OutBoxPlugin::OutBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "OutBox";

    d_include       = "outbox.h";
    
    d_icon = QPixmap( ":/images/tab/out.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"OutBox\" name=\"OutBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>OutBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>OutBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * OutBoxPlugin::createWidget
 *****************************************************************************/
QWidget * OutBoxPlugin::createWidget( QWidget * parent )
{
    return ( new OutBox( parent ) );
}

/******************************************************************************
 * OutBoxPlugin::group
 *****************************************************************************/
QString OutBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class FltBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * FltBoxPlugin::FltBoxPlugin
 *****************************************************************************/
FltBoxPlugin::FltBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "FltBox";

    d_include       = "fltbox.h";
    
    d_icon = QPixmap( ":/images/tab/filter.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"FltBox\" name=\"FltBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>FltBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>FltBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * FltBoxPlugin::createWidget
 *****************************************************************************/
QWidget * FltBoxPlugin::createWidget( QWidget * parent )
{
    return ( new FltBox( parent ) );
}

/******************************************************************************
 * FltBoxPlugin::group
 *****************************************************************************/
QString FltBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class UpdateBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * UpdateBoxPlugin::UpdateBoxPlugin
 *****************************************************************************/
UpdateBoxPlugin::UpdateBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "UpdateBox";

    d_include       = "updatebox.h";
    
    d_icon = QPixmap( ":/images/tab/filter.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"UpdateBox\" name=\"UpdateBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>UpdateBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>UpdateBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * UpdateBoxPlugin::createWidget
 *****************************************************************************/
QWidget * UpdateBoxPlugin::createWidget( QWidget * parent )
{
    return ( new UpdateBox( parent ) );
}

/******************************************************************************
 * UpdateBoxPlugin::group
 *****************************************************************************/
QString UpdateBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}

/******************************************************************************
 * class PlayBackBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * PlayBackBoxPlugin::PlayBackBoxPlugin
 *****************************************************************************/
PlayBackBoxPlugin::PlayBackBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "PlayBackBox";

    d_include       = "playbackbox.h";
    
    d_icon = QPixmap( ":/images/tab/filter.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"PlayBackBox\" name=\"PlayBackBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>FltBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>PlayBackBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * PlayBackBoxPlugin::createWidget
 *****************************************************************************/
QWidget * PlayBackBoxPlugin::createWidget( QWidget * parent )
{
    return ( new PlayBackBox( parent ) );
}

/******************************************************************************
 * PlayBackBoxPlugin::group
 *****************************************************************************/
QString PlayBackBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class FpncBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * FpncBoxPlugin::FpncBoxPlugin
 *****************************************************************************/
FpncBoxPlugin::FpncBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "FpncBox";

    d_include       = "fpncbox.h";
    
    d_icon = QPixmap( ":/images/tab/fpn.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"FpncBox\" name=\"FpncBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>FltBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>FpncBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * FpncBoxPlugin::createWidget
 *****************************************************************************/
QWidget * FpncBoxPlugin::createWidget( QWidget * parent )
{
    return ( new FpncBox( parent ) );
}

/******************************************************************************
 * FpncBoxPlugin::group
 *****************************************************************************/
QString FpncBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}


/******************************************************************************
 * class KneeBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * KneeBoxPlugin::KneeBoxPlugin
 *****************************************************************************/
KneeBoxPlugin::KneeBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "KneeBox";

    d_include       = "kneebox.h";
    
    d_icon = QPixmap( ":/images/tab/gamma.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"KneeBox\" name=\"KneeBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>KneeBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>KneeBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}


/******************************************************************************
 * KneeBoxPlugin::createWidget
 *****************************************************************************/
QWidget * KneeBoxPlugin::createWidget( QWidget * parent )
{
    return ( new KneeBox( parent ) );
}

/******************************************************************************
 * KneeBoxPlugin::group
 *****************************************************************************/
QString KneeBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}

/******************************************************************************
 * class DpccBoxPlugin
 *
 *****************************************************************************/

/******************************************************************************
 * DpccBoxPlugin::DpccBoxPlugin
 *****************************************************************************/
DpccBoxPlugin::DpccBoxPlugin( QObject * parent )
    : CustomWidgetInterface( parent )
{
    d_name          = "DpccBox";

    d_include       = "dpccbox.h";

    d_icon = QPixmap( ":/images/tab/dpcc.png" );

    d_domXml        =  "<ui language=\"c++\">\n"
                       " <widget class=\"DpccBox\" name=\"DpccBox\">\n"
                       "  <property name=\"geometry\">\n"
                       "   <rect>\n"
                       "    <x>0</x>\n"
                       "    <y>0</y>\n"
                       "    <width>880</width>\n"
                       "    <height>1000</height>\n"
                       "   </rect>\n"
                       "  </property>\n"
                       "  <property name=\"toolTip\" >\n"
                       "   <string>DpccBox</string>\n"
                       "  </property>\n"
                       "  <property name=\"whatsThis\" >\n"
                       "   <string>DpccBox</string>\n"
                       "  </property>\n"
                       " </widget>\n"
                       "</ui>\n";
}

/******************************************************************************
 * DpccBoxPlugin::createWidget
 *****************************************************************************/
QWidget * DpccBoxPlugin::createWidget( QWidget * parent )
{
    return ( new DpccBox( parent ) );
}

/******************************************************************************
 * DpccBoxPlugin::group
 *****************************************************************************/
QString DpccBoxPlugin::group() const
{
    return ( DCT_DIALOG_WIDGET_GROUP_NAME );
}

/******************************************************************************
 * class CustomWidgetCollectionInterface
 *
 *****************************************************************************/

/******************************************************************************
 * CustomWidgetCollectionInterface::CustomWidgetCollectionInterface
 *****************************************************************************/
CustomWidgetCollectionInterface::CustomWidgetCollectionInterface( QObject * parent )
    : QObject( parent )
{
    // simple widgets
    plugins.append( new FmtSpinBoxPlugin( this ) );
    plugins.append( new SkinnedDialPlugin( this ) );
    plugins.append( new SwitchButtonPlugin( this ) );
    plugins.append( new SingleChannelKnobBoxPlugin( this ) );
    plugins.append( new TrippleChannelSliderBoxPlugin( this ) );
    plugins.append( new TrippleChannelKnobBoxPlugin( this ) );
    plugins.append( new MccSliderPlugin( this ) );
    plugins.append( new ButtonArrayBoxPlugin( this ) );
    plugins.append( new PieSegmentSelectPlugin( this ) );
    plugins.append( new HueSegmentSelectPlugin( this ) );
    plugins.append( new QCustomPlotPlugin( this ) );
    
    // dialog widgets
    plugins.append( new InfoBoxPlugin( this ) );
    plugins.append( new BlackLevelBoxPlugin( this ) );
    plugins.append( new WbBoxPlugin( this ) );
    plugins.append( new MccBoxPlugin( this ) );
    plugins.append( new MccEqBoxPlugin( this ) );
    plugins.append( new GammaBoxPlugin( this ) );
    plugins.append( new LutBoxPlugin( this ) );
    plugins.append( new InOutBoxPlugin( this ) );
    plugins.append( new OutBoxPlugin( this ) );
    plugins.append( new FltBoxPlugin( this ) );
    plugins.append( new UpdateBoxPlugin( this ) );
    plugins.append( new PlayBackBoxPlugin( this ) );
    plugins.append( new FpncBoxPlugin( this ) );
    plugins.append( new KneeBoxPlugin( this ) );
    plugins.append( new DpccBoxPlugin( this ) );
}

/******************************************************************************
 * CustomWidgetCollectionInterface::CustomWidgetCollectionInterface
 *****************************************************************************/
QList<QDesignerCustomWidgetInterface *> CustomWidgetCollectionInterface::customWidgets( void ) const
{
    return ( plugins );
}


