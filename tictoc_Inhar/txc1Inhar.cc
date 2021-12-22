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
    virtual void ilarakoPakBidali( int a) ;
    virtual void paketeaKudeatu(paketea* p) ;

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
        EV << "\nPaquete recibido de " << p->getSenderModule()->getClassName() << p->getSenderModule()->getIndex();

        if(strcmp(p->getSenderModule()->getClassName(),"source")==0){ //paketea iturritik dator--> errorea 0

            double aux=((double)rand())/RAND_MAX;
                    if(aux<prob){
                        lotura=0;
                    }else{
                        lotura=1;
                    }

            if(queueOut[lotura]->isEmpty()){
                    queueOut[lotura]->insert(p);
                    EV << "\nPaquete guardado en cola " << lotura;

                    if(channelOut[lotura]->isBusy()==false){
                        ilarakoPakBidali(lotura);
                    }
             }else{ //Si hay paquetes en la cola: solo insertar
                    queueOut[lotura]->insert(p);
                    EV << "\nPaquete guardado en cola " << lotura;
             }
        }
        else{//Paquete viene de otro nodo
                EV << "\n If de paquetes llegados de nodo";

                int type=p->getType();
                EV << "\ntipo: " << type;
                int gateIndex;
                gateIndex=p->getArrivalGate()->getIndex();
                EV << "\ngateIndex: " << gateIndex;
                switch(type){
                        case 0://Mensaje normal
                            EV << "\nPaquete recibido";
                            if(!queueOut[gateIndex]->isEmpty() && !channelOut[gateIndex]->isBusy()){
                            paketeaKudeatu(p);
                            }
                            break;
                        case 1://ACK
                            EV << "\nACK recibido";
                            delete(p);
                            queueOut[gateIndex]->pop();

                            EV << "\nqueue length: " << queueOut[gateIndex]->getLength();

                            if(!queueOut[gateIndex]->isEmpty() && !channelOut[gateIndex]->isBusy()){
                                ilarakoPakBidali(gateIndex);
                            }
                            break;
                        case 2://NAK
                            EV << "\nNAK recibido";
                            delete(p);
                            if(channelOut[gateIndex]->isBusy()==false){
                                ilarakoPakBidali(gateIndex);
                            }
                            break;
                    }
            }
}
void Tic8::ilarakoPakBidali(int lotura){

    paketea* p=check_and_cast<paketea*> (queueOut[lotura]->front());
    send(p->dup(),"out",lotura);

    EV << "\nPaquete enviado por lotura " << lotura;
}

void Tic8::paketeaKudeatu(paketea* p){
    cGate* g=p->getArrivalGate();

    if(p->hasBitError()){
            //NAK BIDALI
            paketea* nak=new paketea("NAK");
            nak->setSeq(seqNak);
            EV << "\n nak sortuta: ";
           // nak->setSource(getIndex());
            nak->setType(2);
            seqNak++;
            send(nak,"out",g->getIndex());
            EV << "\nRespuesta " << nak->getName() << " enviada por lotura " << g->getIndex();


       }else{
        //ACK BIDALI
        paketea* ack=new paketea("ACK");
        ack->setSeq(seqAck);
        ack->setSource(getIndex());
        ack->setType(1);
        seqAck++;

        send(ack,"out",g->getIndex());
        EV << "\nRespuesta " << ack->getName() << " enviada por lotura " << g->getIndex();

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
                        ilarakoPakBidali(lotura);
                    }
                }else{ //Si hay paquetes en la cola: solo insertar
                    queueOut[lotura]->insert(p);
                    EV << "\nPaquete guardado en cola " << lotura;
                }

                EV << "\nPaquete guardado en cola " << lotura;
       }
}


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



