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
class Tic8 : public cSimpleModule
{
  private:
    int seqAck;
    int seqNak;
    cChannel* channelOut[1];
    cQueue* queueOut[1];

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void enviarPaqueteCola();

};

Define_Module(Tic8);


void Tic8::initialize()
{
    channelOut[0]=gate("out", 0)->getTransmissionChannel();
    queueOut[0]=new cQueue("queue1");
}

void Tic8::handleMessage(cMessage *msg)
{
        int enlace=0;
        paketea* p=check_and_cast<paketea*>(msg);
        if(strcmp(p->getSenderModule()->getClassName(),"source")==0){
        EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getIndex();

        if(queueOut[0]->isEmpty()){ //Si la cola esta vacia: insertar y enviar
                    queueOut[enlace]->insert(p);
                    EV << "\nPaquete guardado en cola " << enlace;

                    if(channelOut[enlace]->isBusy()==false){
                        enviarPaqueteCola();
                    }
                }else{ //Si hay paquetes en la cola: solo insertar
                    queueOut[enlace]->insert(p);
                    EV << "\nPaquete guardado en cola " << enlace;
                }
        }
        else{//Paquete viene de otro nodo
                EV << "\n If de paquetes llegados de nodo";

                int type=p->getType();
                EV << "\ntipo: " << type;
                int gateIndex=p->getArrivalGate()->getIndex();

                switch (type){
                        case 0://Mensaje normal
                            EV << "\nPaquete recibido";
                            //tratarPaqueteNodo(p);
                            break;
                        case 1://ACK
                            EV << "\nACK recibido";
                            delete(p);
                            queueOut[0]->pop();

                           EV << "\nqueue length: " << queueOut[0]->getLength();

                            if(!queueOut[enlace]->isEmpty() && !channelOut[enlace]->isBusy()){
                                enviarPaqueteCola();
                            }
                            break;
                        case 2://NAK
                            EV << "\nNAK recibido";
                            delete(p);
                            if(channelOut[0]->isBusy()==false){
                                enviarPaqueteCola();
                            }
                            break;
                    }
            }
}
void Tic8::enviarPaqueteCola(){
    int enlace=0;
    paketea* p=check_and_cast<paketea*> (queueOut[enlace]->front());
    send(p->dup(),"out",enlace);

    EV << "\nPaquete enviado por enlace " << enlace;
}




class Toc8 : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Toc8);

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



