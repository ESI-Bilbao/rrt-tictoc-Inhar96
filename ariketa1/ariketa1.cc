//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "paketea_m.h"

using namespace omnetpp;

/**
 * Let us take a step back, and remove random delaying from the code.
 * We'll leave in, however, losing the packet with a small probability.
 * And, we'll we do something very common in telecommunication networks:
 * if the packet doesn't arrive within a certain period, we'll assume it
 * was lost and create another one. The timeout will be handled using
 * (what else?) a self-message.
 */
class Tic : public cSimpleModule
{
  private:
    double lonmedia=1000;
    double meanTime=0.5;
    cMessage *event;

    /* simtime_t timeout;  // timeout
    cMessage *timeoutEvent;  // holds pointer to the timeout self-message

  public:
    Tic8();
    virtual ~Tic8();*/

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual paketea* paketeaSortu();
    virtual void tratarPaqueteNodo(paketea *p);

};

Define_Module(Tic);

/*Tic8::Tic8()
{
    timeoutEvent = nullptr;
}

Tic8::~Tic8()
{
    cancelAndDelete(timeoutEvent);
}*/

void Tic::initialize()
{
    event=new cMessage("event");
    double t=exponential(meanTime, 0);
    EV << "\nInitial time: " << t;
    scheduleAt(simTime()+t, event);


   /* // Initialize variables.
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    // Generate and send initial message.
    EV << "Sending initial message\n";
    cMessage *msg = new cMessage("tictocMsg");
    send(msg, "out");
    scheduleAt(simTime()+timeout, timeoutEvent);*/
}

void Tic::handleMessage(cMessage *msg)
{
        EV << "\nSending packet from Source " << getIndex() << " At time " << simTime();
        paketea* pak=paketeaSortu();

        send(pak,"out");
        double t=exponential(0.5, 0);
        scheduleAt(simTime()+t, event);

        paketea* p=check_and_cast<paketea*>(msg);
           EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getIndex();
           EV << "\n If de paquetes llegados de nodo";
           int type=p->getType();
           EV << "\nPaquete recibido";
              // int gateIndex=p->getArrivalGate()->getIndex();

               switch (type){
                       case 0://Mensaje normal
                           EV << "\nPaquete recibido";
                           tratarPaqueteNodo(p);
                           break;
                       case 1://ACK
                           EV << "\nACK recibido";
                           break;
                       case 2://NAK
                           EV << "\nNAK recibido";
                           break;
                   }

   /* if (msg == timeoutEvent) {
        // If we receive the timeout event, that means the packet hasn't
        // arrived in time and we have to re-send it.
        EV << "Timeout expired, resending message and restarting timer\n";
        cMessage *newMsg = new cMessage("tictocMsg");
        send(newMsg, "out");
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    else {  // message arrived
            // Acknowledgement received -- delete the received message and cancel
            // the timeout event.
        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);
        delete msg;

        // Ready to send another one.
        cMessage *newMsg = new cMessage("tictocMsg");
        send(newMsg, "out");
        scheduleAt(simTime()+timeout, timeoutEvent);
    }*/
}

paketea* Tic::paketeaSortu(){
    char izena[20];
    sprintf(izena,"Source%dPacket",getIndex());

    paketea* p=new paketea(izena);
    p->setType(0);
    unsigned int types=p->getType();
    EV << "\nPacket type: " << types;
    double l=exponential(lonmedia, 0);
    EV << "\nPacket length: " << l;
    p->setBitLength(l);
    return p;
}

/**
 * Sends back an acknowledgement -- or not.
 */
class Toc : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Toc);

void Toc::handleMessage(cMessage *msg)
{
    paketea* p=check_and_cast<paketea*>(msg);

       EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getIndex();

       int type=p->getType();
       if(type==0){
           EV << "\nLehenengo IF-ean sartu da";
           int gateIndex=p->getArrivalGate()->getIndex();

           if(p->hasBitError()){
               //SEND NAK
               paketea* nak=new paketea("NAK");

               nak->setType(2);

               send(nak,"out",gateIndex);
               EV << "\nRespuesta " << nak->getName() << " enviada por enlace " << gateIndex;

           }else{
               //SEND ACK
               paketea* ack=new paketea("ACK");

               ack->setType(1);

               send(ack,"out",gateIndex);
               EV << "\nRespuesta " << ack->getName() << " enviada por enlace " << gateIndex;
               EV << "\nPaquete llegado a destino sin errores";


           }
       }
}



