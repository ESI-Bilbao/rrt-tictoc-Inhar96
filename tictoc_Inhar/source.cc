#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "paketea_m.h"

using namespace omnetpp;

class source : public cSimpleModule{
    private:
        double lonmedia=1000;
        double meanTime=0.5;
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

    paketea* paquete=paketeaSortu();
    EV << "\nPaketea bidali da " << getName() << " iturritik. Time: " << simTime();
    send(paquete,"out");
    double t=exponential(meanTime, 0);
    scheduleAt(simTime()+t, event);

}

paketea* source::paketeaSortu(){
    char izena[20];
    sprintf(izena,"%s Packet%d",getName(),seq);
    paketea* p=new paketea(izena);
    p->setType(0);
    p->setSeq(seq);
    p->setSource(getId());
    double l=exponential(lonmedia, 0);
    EV << "\nPacket length: " << l << " Seq: " << seq;
    p->setBitLength(l);
    seq++;
    return p;
}
