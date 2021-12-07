#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "paketea_m.h"

using namespace omnetpp;

class source : public cSimpleModule{
    private:
        double lonmedia=1000;
        double meanTime=2;
        int seq=0;
        cMessage *event;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual paketea* paketeaSortu();
};

Define_Module(source);

void source::initialize(){
    event=new cMessage("event");

    double t=exponential(meanTime, 0);
    EV << "\nInitial time: " << t;
    scheduleAt(simTime()+t, event);
}

void source::handleMessage(cMessage *msg){
    EV << "\nSending packet from source " << getIndex() << " At time " << simTime();
    paketea* paquete=paketeaSortu();

    send(paquete,"out");
    double t=exponential(2, 0);
    scheduleAt(simTime()+t, event);

}

paketea* source::paketeaSortu(){
    char packetName[20];
    sprintf(packetName,"source%dPacket%d",getIndex(),seq);

    paketea* p=new paketea(packetName);

    p->setType(0);

    double l=exponential(lonmedia, 0);
    EV << "\nPacket length: " << l;
    p->setBitLength(l);
    seq++;
    return p;
}
