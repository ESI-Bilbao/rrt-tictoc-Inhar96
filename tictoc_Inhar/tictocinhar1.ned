//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
import ned.DatarateChannel;

simple Tic8
{
    parameters:
        @display("i=block/routing");
    gates:
        input in[];
        output out[];
}

simple Toc8
{
    parameters:
        @display("i=block/process");
    gates:
        input in[];
        output out[];
}
simple source
{
    parameters:
        @display("i=block/source");
    gates:
        output out;
}

network Tictoc8
{
    @display("bgb=337,229");
    types:
        channel C extends DatarateChannel
        {
            datarate = 1000bps;
            per = 0.5;
            delay = 0.1ms;
        }
    submodules:
        tic: Tic8 {
            parameters:
                @display("i=,cyan;p=105,154");
        }
        toc: Toc8 {
            parameters:
                @display("i=,gold;p=247,154");
        }
        itu: source {
            @display("p=66,77");
        }
    connections:
        itu.out --> tic.in++;
        tic.out++ --> C --> toc.in++;
        tic.in++ <-- C <-- toc.out++;
        
        
}
