

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
   cChannel* channelOut[2];
   cQueue* queueOut[2];
   int lotura;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Toc8);

void Toc8::initialize()
{
    channelOut[0]=gate("out", 0)->getTransmissionChannel();
    channelOut[1]=gate("out", 1)->getTransmissionChannel();

    queueOut[0]=new cQueue("queue0");
    queueOut[1]=new cQueue("queue1");
    seqAck=0;
    seqNak=0;
}
void Toc8::handleMessage(cMessage *msg)
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
               if(!queueOut[gateIndex]->isEmpty() && !channelOut[gateIndex]->isBusy()){

               nak->setType(2);
               }
                   send(nak,"out",gateIndex);

               EV << "\nRespuesta " << nak->getName() << " enviada por enlace " << gateIndex;

           }else{
               //SEND ACK
               paketea* ack=new paketea("ACK");

               ack->setType(1);
               if(!queueOut[gateIndex]->isEmpty() && !channelOut[gateIndex]->isBusy()){

                   send(ack,"out",gateIndex);
               }
               EV << "\nRespuesta " << ack->getName() << " enviada por enlace " << gateIndex;
               EV << "\nPaquete llegado a destino sin errores";


           }
       }
}

