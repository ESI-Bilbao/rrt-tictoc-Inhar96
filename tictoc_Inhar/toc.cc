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
class Toc8 : public cSimpleModule
{

private:
   int seqAck;
   int seqNak;
   int jasotakoak;


   int lotura;
   cOutVector denborakVector;
   cLongHistogram jauziakHistograma;
   cOutVector jauziakVector;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Toc8);

void Toc8::initialize()
{

    seqAck=0;
    seqNak=0;
    jasotakoak=0;

    WATCH(jasotakoak);
    WATCH(seqAck);
    WATCH(seqNak);

    denborakVector.setName("denborak");
    jauziakHistograma.setName("jauziakHisto");
    jauziakHistograma.setRangeAutoUpper(0, 10, 1.5);
    jauziakVector.setName("jauziak");
}
void Toc8::handleMessage(cMessage *msg)
{
    paketea* p=check_and_cast<paketea*>(msg);
       EV << "\n "<< getName();
       EV << "Paketea jaso da  " << p->getSenderModule()->getName()<<"-etik";

       int type=p->getType();
       if(type==0){ //datu paketea da

           int gateIndex=p->getArrivalGate()->getIndex();
           EV << "\n "<< gateIndex<<" loturatik";

           if(p->hasBitError()){
               //SEND NAK
               EV << "\n paketeak errorea dauka, NAK bidali behar da";
               paketea* nak=new paketea("NAK");
               nak->setSeq(seqNak);
               nak->setSource(getId());
               nak->setType(2);
               seqNak++;
               //if(!queueOut[gateIndex]->isEmpty() && !channelOut[gateIndex]->isBusy()){

                   send(nak,"out",gateIndex);
               //}

               EV << "\nErantzuna " << nak->getName() << " bidali da " << gateIndex <<" loturatik";

           }else{
               //SEND ACK
               paketea* ack=new paketea("ACK");

               ack->setSeq(seqAck);
               ack->setSource(getId());
               ack->setType(1);
               seqAck++;
               //if(!queueOut[gateIndex]->isEmpty() && !channelOut[gateIndex]->isBusy()){

               send(ack,"out",gateIndex);
               //}
               EV << "\nErantzuna " << ack->getName() << " bidali da " << gateIndex <<" loturatik";
              // EV << "\nPaquete llegado a destino sin errores";
               jasotakoak++;
               denborakVector.record(p->getDuration().dbl());
               p->setJauziak(p->getJauziak()+1);
               jauziakVector.record(p->getJauziak());
               jauziakHistograma.collect(p->getJauziak());

           }
       }
}
