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
    cChannel* channelOut[2];
    cQueue* queueOut[2];
    int lotura;
    double prob;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void ilarakoPakBidali() ;
    virtual void paketeaKudeatu(paketea* p, int a) ;

};

Define_Module(Tic8);


void Tic8::initialize()
{
    prob=(double)par("prob1");
    channelOut[0]=gate("out", 0)->getTransmissionChannel();
    channelOut[1]=gate("out", 1)->getTransmissionChannel();

    queueOut[0]=new cQueue("queue0");
    queueOut[1]=new cQueue("queue1");
    seqAck=0;
    seqNak=0;
}

void Tic8::handleMessage(cMessage *msg)
{

        paketea* p=check_and_cast<paketea*>(msg);
        EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getName();

        if(strcmp(p->getSenderModule()->getClassName(),"source")==0){ //paketea iturritik dator--> errorea 0

            double aux=((double)rand())/RAND_MAX;
                    if(aux<prob){
                        lotura=0;
                    }else{
                        lotura=1;
                    }

            if(queueOut[lotura]->isEmpty()){
                    queueOut[lotura]->insert(p);
                    EV << "\nPaketea " << lotura<<". ilaran";

                    if(channelOut[lotura]->isBusy()==false){
                        ilarakoPakBidali();
                    }
             }else{ //Si hay paquetes en la cola: solo insertar
                    queueOut[lotura]->insert(p);
                    EV << "\nPaketea " << lotura<<". ilaran";
             }
        }
        else{//Paquete viene de otro nodo
                int type=p->getType();

                cGate* g=p->getArrivalGate();
                int arrivalGateIndex = g -> getIndex();

                EV << "\n " << getName()<< ": ";
                switch(type){
                        case 0://Mensaje normal
                            EV << "Paketea jaso da";
                            paketeaKudeatu(p,arrivalGateIndex);
                            break;
                        case 1://ACK
                            EV << "ACK jaso da";
                            delete(p);
                            queueOut[arrivalGateIndex]->pop();

                            EV << "\nqueue length: " << queueOut[arrivalGateIndex]->getLength();

                            if(!queueOut[lotura]->isEmpty() && !channelOut[lotura]->isBusy()){
                                ilarakoPakBidali();
                            }
                            break;
                        case 2://NAK
                            EV << "NAK jaso da";
                            delete(p);
                            if(channelOut[lotura]->isBusy()==false){
                                ilarakoPakBidali();
                            }
                            break;
                    }
            }
}
void Tic8::ilarakoPakBidali(){

    paketea* p=check_and_cast<paketea*> (queueOut[lotura]->front());
    send(p->dup(),"out",lotura);
    EV << "\n " << getName();
    EV << ": Paketea " << lotura <<"-tik bidali da";
}

void Tic8::paketeaKudeatu(paketea* p, int a){
//    cGate* g=p->getArrivalGate();
//    int arrivalGateIndex = g -> getIndex();
    if(p->hasBitError()){
            //NAK BIDALI
            EV << "Paketea errorearekin jaso da, bidali NAK\n";
            paketea* nak=new paketea("NAK");
            nak->setSeq(seqNak);
            nak->setSource(getId());
            nak->setType(2);
            seqNak++;
            send(nak,"out",a);
            EV << "\nNAK mezua bidali da hurrengo loturatik: " << a;

       }else{
        //ACK BIDALI
           EV << "Paketea errorerik gabe jaso da, bidali ACK\n";
           paketea* ack=new paketea("ACK");
           ack->setSeq(seqAck);
           ack->setSource(getId());
           ack->setType(1);
           seqAck++;
           send(ack,"out",a);
           EV << "\nACK mezua bidali da hurrengo loturatik: " << a;

        //PAKETEA BIRBIDALI
        double aux=((double)rand())/RAND_MAX;
                if(aux<prob){
                    lotura=0;
                }else{
                    lotura=1;
                }

                if(queueOut[lotura]->isEmpty()){ //Si la cola esta vacia: insertar y enviar
                    queueOut[lotura]->insert(p);
                    EV << "\nPaquete guardado en cola " << lotura;

                    if(channelOut[lotura]->isBusy()==false){
                        ilarakoPakBidali();
                    }
                }else{ //Si hay paquetes en la cola: solo insertar
                    queueOut[lotura]->insert(p);
                    EV << "\nPaquete guardado en cola " << lotura;
                }

                EV << "\nPaquete guardado en cola " << lotura;
       }
}
