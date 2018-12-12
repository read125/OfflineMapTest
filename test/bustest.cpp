﻿#include "bustest.h"
#ifndef BUS_H
#include "src/bus.h"
#endif
BusTest::BusTest()
{
    bus_test_=NULL;
    bus_test_=new Bus();
}
BusTest::~BusTest()
{
    if(bus_test_!=NULL)
    {
        delete[] bus_test_;
        bus_test_=NULL;
    }
}
void BusTest::ShowTest(QDeclarativeGeoMap *qMap)
{
    tool.TestNoteTool("ShowTest",0);
    if(bus_test_==NULL)
    {
        bus_test_=new Bus(tool.WPS84ToGCJ02(30.5563134000,103.9938400000));
    };
    qMap->addMapItem(bus_test_);
    qDebug()<<bus_test_->parentItem();
    auto *parent1=bus_test_->quickMap();
    qDebug()<<parent1->fromCoordinate(parent1->center());
    tool.TestNoteTool("ShowTest",1);
}
void BusTest::MoveTest()
{
    tool.TestNoteTool("MoveTest",0);
    if(bus_test_->parentItem())
    {
       bus_test_->setCoordinate(tool.WPS84ToGCJ02(30.5563134000,103.9938400000));
    }
    tool.TestNoteTool("MoveTest",1);
}
void BusTest::UpdataPositionTest()
{
    tool.TestNoteTool("UpdataPositionTest",0);
    bus_test_->UpdataPosition();
    tool.TestNoteTool("UpdataPositionTest",1);
}
void BusTest::LuShuTest()
{
    tool.TestNoteTool("LuShuTest",0);
    bus_test_->LuShu();
    tool.TestNoteTool("LuShuTest",1);
}
void  BusTest::MainTest()
{

}