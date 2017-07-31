/******************************************************************************
 * Copyright (C) 2017 Dream Chip Technologies GmbH
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/**
 * @file    dct_widgets.h
 *
 * @brief   Definition of dct designer widgets 
 *
 *****************************************************************************/
#ifndef __DCT_WIDGETS_H__
#define __DCT_WIDGETS_H__

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

// simple widgets
#include "fmtspinbox.h"
#include "skinneddial.h"
#include "switchbutton.h"
#include "singlechannelknobbox.h"
#include "tripplechannelsliderbox.h"
#include "tripplechannelknobbox.h"
#include "btnarraybox.h"
#include "piesegmentselect.h"
#include "huesegmentselect.h"
#include "mccslider.h"
#include "qcustomplot.h"

// dialog widgets
#include "wbbox.h"
#include "mccbox.h"
#include "mcceqbox.h"
#include "infobox.h"
#include "blacklevelbox.h"
#include "gammabox.h"
#include "lutbox.h"
#include "inoutbox.h"
#include "outbox.h"
#include "fltbox.h"
#include "updatebox.h"
#include "playbackbox.h"
#include "fpncbox.h"
#include "kneebox.h"
#include "dpccbox.h"

#define DCT_BASIC_WIDGET_GROUP_NAME         ( "DCT Basic Widgets" )
#define DCT_DIALOG_WIDGET_GROUP_NAME        ( "DCT Dialog Widgets" )

// CustomWidgetInterface
class CustomWidgetInterface: public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit CustomWidgetInterface( QObject * parent = 0);

    bool isContainer() const Q_DECL_OVERRIDE;
    bool isInitialized() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;
    QString domXml() const Q_DECL_OVERRIDE;
    QString group() const Q_DECL_OVERRIDE;
    QString includeFile() const Q_DECL_OVERRIDE;
    QString name() const Q_DECL_OVERRIDE;
    QString toolTip() const Q_DECL_OVERRIDE;
    QString whatsThis() const Q_DECL_OVERRIDE;
    void initialize( QDesignerFormEditorInterface * ) Q_DECL_OVERRIDE;

protected:
    QString d_name;
    QString d_include;
    QString d_toolTip;
    QString d_whatsThis;
    QString d_domXml;
    QString d_codeTemplate;
    QIcon d_icon;

private:
    bool initialized;
};

// CustomWidgetCollectionInterface
class CustomWidgetCollectionInterface: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
    Q_INTERFACES( QDesignerCustomWidgetCollectionInterface )

public:
    CustomWidgetCollectionInterface( QObject * parent = 0 );
    
    QList<QDesignerCustomWidgetInterface *> customWidgets() const Q_DECL_OVERRIDE;

private:
    QList<QDesignerCustomWidgetInterface *> plugins;
};

// FmtSpinBoxPlugin
class FmtSpinBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit FmtSpinBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// SkinnedDialPlugin
class SkinnedDialPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit SkinnedDialPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// SwitchButtonPlugin
class SwitchButtonPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit SwitchButtonPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// SwitchButtonPlugin
class SingleChannelKnobBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit SingleChannelKnobBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// TrippleChannelSliderBoxPlugin
class TrippleChannelSliderBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit TrippleChannelSliderBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// TrippleChannelKnobBoxPlugin
class TrippleChannelKnobBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit TrippleChannelKnobBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// ButtonArrayBoxPlugin
class ButtonArrayBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit ButtonArrayBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// PieSegmentSelectPlugin
class PieSegmentSelectPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit PieSegmentSelectPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// HueSegmentSelectPlugin
class HueSegmentSelectPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit HueSegmentSelectPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// MccSliderPlugin
class MccSliderPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit MccSliderPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// QCustomPlotPlugin
class QCustomPlotPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit QCustomPlotPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
};

// WbBoxPlugin
class WbBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit WbBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// MccBoxPlugin
class MccBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit MccBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// MccEqBoxPlugin
class MccEqBoxPlugin : public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit MccEqBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;

    QString group() const Q_DECL_OVERRIDE;
};

// InfoBoxPlugin
class InfoBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit InfoBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// BlackLevelBoxPlugin
class BlackLevelBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit BlackLevelBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// GammaBoxPlugin
class GammaBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit GammaBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// GammaBoxPlugin
class LutBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit LutBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// InOutBoxPlugin
class InOutBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit InOutBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// OutBoxPlugin
class OutBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit OutBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// FltBoxPlugin
class FltBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit FltBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// UpdateBoxPlugin
class UpdateBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit UpdateBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// PlayBackBoxPlugin
class PlayBackBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit PlayBackBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// FpncBoxPlugin
class FpncBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit FpncBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// KneeBoxPlugin
class KneeBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit KneeBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;
    
    QString group() const Q_DECL_OVERRIDE;
};

// DpccBoxPlugin
class DpccBoxPlugin: public CustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES( QDesignerCustomWidgetInterface )

public:
    explicit DpccBoxPlugin( QObject * parent = 0 );
    QWidget * createWidget( QWidget *parent ) Q_DECL_OVERRIDE;

    QString group() const Q_DECL_OVERRIDE;
};

#endif // __DCT_WIDGETS_H__

